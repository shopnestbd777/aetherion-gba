#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_font.h"
#include "bn_vector.h"
#include "bn_string.h"
#include "bn_math.h"

#include "bn_sprite_items_hero_sprite.h"
#include "bn_sprite_items_enemy_sprite.h"
#include "bn_sprite_items_npc_sprite.h"
#include "bn_sprite_items_font_8x8.h"

#include "party_manager.h"
#include "reactions.h"
#include "save_data.h"
#include "world_engine.h"
#include "quest_dialogue.h"

using namespace aetherion;

namespace {

bn::string_view rank_label(CharRank r) {
    switch (r) {
        case CharRank::S_RANK: return "S";
        case CharRank::A_RANK: return "A";
        case CharRank::B_RANK: return "B";
        case CharRank::C_RANK: return "C";
    }
    return "C";
}

bn::string_view essence_label(Essence e) {
    switch (e) {
        case Essence::BLAZE:   return "BLAZE";
        case Essence::TIDE:    return "TIDE";
        case Essence::TEMPEST: return "TEMPST";
        case Essence::FROST:   return "FROST";
        case Essence::GALE:    return "GALE";
        case Essence::TERRA:   return "TERRA";
        case Essence::ASTRAL:  return "ASTRAL";
        case Essence::NONE:    return "NONE";
    }
    return "NONE";
}

bn::string_view reaction_label(ReactionType rt) {
    switch (rt) {
        case ReactionType::SCALD:         return "SCALD 2X!";
        case ReactionType::THERMAL_SHOCK: return "THERMAL SHOCK!";
        case ReactionType::ARC_SURGE:     return "ARC SURGE!";
        case ReactionType::GLACIER_BIND:  return "GLACIER BIND!";
        case ReactionType::DETONATION:    return "DETONATION!";
        case ReactionType::RIME_BOLT:     return "RIME BOLT!";
        case ReactionType::VORTEX_SPREAD: return "VORTEX SPREAD!";
        case ReactionType::JADE_WARD:     return "JADE WARD!";
        case ReactionType::NONE:          return "";
    }
    return "";
}

} // namespace

int main() {
    bn::core::init();

    // Custom 8x8 ASCII Font
    bn::sprite_font ascii_font(bn::sprite_items::font_8x8);
    bn::sprite_text_generator text_gen(ascii_font);
    text_gen.set_bg_priority(0);

    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    PartyManager party;
    SaveData save;
    WorldEngine world;
    QuestDialogueManager quests;

    world.load_region(RegionID::SKY_ISLET, camera);

    // Create World Sprites
    bn::fixed_point player_pos(0, -16);
    bn::sprite_ptr hero_spr = bn::sprite_items::hero_sprite.create_sprite(player_pos);
    hero_spr.set_camera(camera);

    bn::fixed_point npc_pos(0, -48);
    bn::sprite_ptr npc_spr = bn::sprite_items::npc_sprite.create_sprite(npc_pos);
    npc_spr.set_camera(camera);

    bn::fixed_point enemy_pos(36, 12);
    bn::sprite_ptr enemy_spr = bn::sprite_items::enemy_sprite.create_sprite(enemy_pos);
    enemy_spr.set_camera(camera);

    int enemy_hp = 120;
    int enemy_respawn_timer = 0;
    EssenceAura enemy_aura;

    bn::fixed stamina = save.max_stamina;
    bn::string_view popup_text = "";
    int popup_timer = 0;

    bn::vector<bn::sprite_ptr, 64> hud_sprites;

    while (true) {
        party.update_cooldowns();

        // Handle Dialogue Box Dismissal
        if (quests.is_dialogue_open()) {
            if (bn::keypad::a_pressed() || bn::keypad::b_pressed()) {
                quests.close_dialogue();
            }
        } else {
            // 1. Movement & Stamina Dash (R Button)
            bn::fixed speed = 1.4;
            if (bn::keypad::r_held() && stamina > 1) {
                speed = 2.5;
                stamina -= 0.7;
            } else if (stamina < save.max_stamina) {
                stamina += 0.4;
            }

            bn::fixed_point next_pos = player_pos;
            if (bn::keypad::left_held())  { next_pos.set_x(next_pos.x() - speed); hero_spr.set_horizontal_flip(true); }
            if (bn::keypad::right_held()) { next_pos.set_x(next_pos.x() + speed); hero_spr.set_horizontal_flip(false); }
            if (bn::keypad::up_held())    { next_pos.set_y(next_pos.y() - speed); }
            if (bn::keypad::down_held())  { next_pos.set_y(next_pos.y() + speed); }

            if (world.is_walkable(next_pos.x(), next_pos.y())) {
                player_pos = next_pos;
            }

            // Check Region Portal (Floating Island <-> Bamboo Valley <-> Underwater)
            RegionID target_region;
            bn::fixed_point spawn_pos;
            if (world.check_region_portal(player_pos.x(), player_pos.y(), target_region, spawn_pos)) {
                world.load_region(target_region, camera);
                player_pos = spawn_pos;
                enemy_hp = 140;
                enemy_aura = EssenceAura{};
                enemy_pos = bn::fixed_point(32, 16);
                enemy_spr.set_visible(true);
                if (target_region == RegionID::BAMBOO_VALLEY) {
                    quests.notify_entered_bamboo_valley(party, save);
                }
            }

            // 2. Fast-Swap Active Party Member (L Button)
            if (bn::keypad::l_pressed()) {
                if (party.swap_next_member()) {
                    const auto& new_tpl = get_character_template(party.active_character_id());
                    popup_text = new_tpl.name;
                    popup_timer = 60;
                }
            }

            // 3. Interact / Talk (A near NPC) OR Essence Attack (A in combat)
            if (bn::keypad::a_pressed()) {
                bn::fixed dx_npc = bn::abs(player_pos.x() - npc_pos.x());
                bn::fixed dy_npc = bn::abs(player_pos.y() - npc_pos.y());

                if (dx_npc < 22 && dy_npc < 22) {
                    quests.interact_with_npc(party, save);
                } else if (enemy_hp > 0) {
                    bn::fixed dx_en = bn::abs(player_pos.x() - enemy_pos.x());
                    bn::fixed dy_en = bn::abs(player_pos.y() - enemy_pos.y());

                    if (dx_en < 32 && dy_en < 32) {
                        auto& active_state = party.active_character_state();
                        const auto& tpl = get_character_template(party.active_character_id());

                        ReactionResult res = evaluate_essence_hit(enemy_aura, active_state.active_essence);
                        int dmg = (bn::fixed(tpl.base_atk / 3) * res.damage_multiplier).integer();
                        enemy_hp -= dmg;

                        if (res.type != ReactionType::NONE) {
                            popup_text = reaction_label(res.type);
                            popup_timer = 75;
                        } else {
                            popup_text = essence_label(active_state.active_essence);
                            popup_timer = 45;
                        }

                        if (enemy_hp <= 0) {
                            enemy_hp = 0;
                            enemy_spr.set_visible(false);
                            enemy_respawn_timer = 240; // Respawns in 4 seconds
                            quests.notify_enemy_defeated(party, save);
                        }
                    }
                }
            }
        }

        // Enemy Respawn & Aura Tick
        if (enemy_hp <= 0 && enemy_respawn_timer > 0) {
            --enemy_respawn_timer;
            if (enemy_respawn_timer == 0) {
                enemy_hp = 120;
                enemy_aura = EssenceAura{};
                enemy_spr.set_visible(true);
            }
        }
        if (enemy_aura.remaining_frames > 0) {
            --enemy_aura.remaining_frames;
            if (enemy_aura.remaining_frames == 0) {
                enemy_aura.element = Essence::NONE;
            }
        }
        if (popup_timer > 0) {
            --popup_timer;
        }

        // Update Camera & Sprite Positions
        camera.set_position(player_pos);
        hero_spr.set_position(player_pos);
        npc_spr.set_position(npc_pos);
        enemy_spr.set_position(enemy_pos);

        // Render Crisp GBA Top & Bottom HUD
        hud_sprites.clear();
        const auto& active_tpl = get_character_template(party.active_character_id());
        const auto& active_st = party.active_character_state();

        bn::string<40> top_line1;
        top_line1.append(active_tpl.name);
        top_line1.append(" [");
        top_line1.append(rank_label(active_tpl.rank));
        top_line1.append("] ");
        top_line1.append(essence_label(active_st.active_essence));
        text_gen.generate(-114, -72, top_line1, hud_sprites);

        bn::string<40> top_line2;
        top_line2.append("HP:");
        top_line2.append(bn::to_string<8>(active_st.current_hp));
        top_line2.append(" STM:");
        top_line2.append(bn::to_string<8>(stamina.integer()));
        top_line2.append(" $:");
        top_line2.append(bn::to_string<8>(save.jade_taels));
        text_gen.generate(-114, -62, top_line2, hud_sprites);

        bn::string<40> region_line;
        region_line.append(world.current_region_name());
        if (enemy_hp > 0 && enemy_aura.element != Essence::NONE) {
            region_line.append(" [FOE:");
            region_line.append(essence_label(enemy_aura.element));
            region_line.append("]");
        }
        text_gen.generate(-114, -52, region_line, hud_sprites);

        if (popup_timer > 0) {
            text_gen.generate(-44, -22, popup_text, hud_sprites);
        }

        if (quests.is_dialogue_open()) {
            text_gen.generate(-114, 54, quests.current_speaker(), hud_sprites);
            text_gen.generate(-114, 66, quests.current_line(), hud_sprites);
        } else {
            text_gen.generate(-114, 66, quests.active_quest_title(), hud_sprites);
        }

        bn::core::update();
    }
}
