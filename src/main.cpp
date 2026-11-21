#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_fixed_point.h"
#include "bn_bg_palettes.h"
#include "bn_color.h"

#include "party_manager.h"
#include "reactions.h"
#include "save_data.h"

using namespace aetherion;

int main() {
    bn::core::init();

    // Sky-island jade backdrop color until tilemaps are loaded in Phase 3
    bn::bg_palettes::set_transparent_color(bn::color(8, 18, 22));

    PartyManager party;
    SaveData save;

    bn::fixed_point player_pos(0, 0);
    bn::fixed stamina = save.max_stamina;
    EssenceAura dummy_enemy_aura;

    while (true) {
        party.update_cooldowns();

        // 1. Movement & Stamina Dash (R Button)
        bn::fixed move_speed = 1.4;
        if (bn::keypad::r_held() && stamina > 1) {
            move_speed = 2.6;
            stamina -= 0.8;
        } else if (stamina < save.max_stamina) {
            stamina += 0.4;
        }

        if (bn::keypad::left_held())  player_pos.set_x(player_pos.x() - move_speed);
        if (bn::keypad::right_held()) player_pos.set_x(player_pos.x() + move_speed);
        if (bn::keypad::up_held())    player_pos.set_y(player_pos.y() - move_speed);
        if (bn::keypad::down_held())  player_pos.set_y(player_pos.y() + move_speed);

        // 2. Fast-Swap Active Party Member (L Button)
        if (bn::keypad::l_pressed()) {
            party.swap_next_member();
        }

        // 3. Essence Skill Attack (A Button)
        if (bn::keypad::a_pressed()) {
            auto& active_state = party.active_character_state();
            const auto& tpl = get_character_template(party.active_character_id());

            if (active_state.skill_cd_remaining == 0) {
                active_state.skill_cd_remaining = tpl.skill_cooldown_frames;
                ReactionResult reaction = evaluate_essence_hit(
                    dummy_enemy_aura,
                    active_state.active_essence
                );
                (void)reaction; // Hooked into floating damage & VFX in Phase 3
            }
        }

        if (dummy_enemy_aura.remaining_frames > 0) {
            --dummy_enemy_aura.remaining_frames;
        }

        bn::core::update();
    }
}
