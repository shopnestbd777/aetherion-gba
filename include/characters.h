#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "game_types.h"
#include "bn_string_view.h"

namespace aetherion {

constexpr int MAX_ROSTER_SIZE = 32; // Room for future character updates

enum class CharacterID : uint8_t {
    REN = 0,        // S-Rank (Special) - Gale - Starter
    MEI_HUA = 1,    // C-Rank - Blaze - Main Quest Ch.1
    KOJI = 2,       // C-Rank - Terra - Side Quest
    SORA = 3,       // B-Rank - Tide - Main Quest Ch.2
    SHUN_LEI = 4,   // A-Rank - Tempest - Guild Shop (2,500 Jade Taels)
    YUEYING = 5,    // S-Rank - Frost - Special Trial: Frost Moon Peak
    COUNT
};

struct CharacterTemplate {
    CharacterID id;
    bn::string_view name;
    bn::string_view title;
    CharRank rank;
    Essence default_essence;
    WeaponType weapon;
    UnlockMethod unlock_method;
    uint16_t unlock_cost_taels;
    uint16_t base_hp;
    uint16_t base_atk;
    uint16_t base_def;
    uint16_t skill_cooldown_frames; // 60 frames = 1 second on GBA
    bool is_special_anomaly;        // True for S-Rank special protagonists/mythics
};

struct CharacterRuntimeState {
    bool unlocked = false;
    uint8_t level = 1;
    uint16_t exp = 0;
    uint16_t current_hp = 100;
    uint16_t max_hp = 100;
    uint16_t skill_cd_remaining = 0;
    Essence active_essence; // Allows Ren to resonate with other Essences at shrines
};

const CharacterTemplate& get_character_template(CharacterID id);

} // namespace aetherion

#endif
