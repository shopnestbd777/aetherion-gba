#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include "game_types.h"
#include "characters.h"
#include "bn_array.h"

namespace aetherion {

constexpr uint32_t SAVE_MAGIC_HEADER = 0x41455431; // "AET1"

struct SaveData {
    uint32_t magic = SAVE_MAGIC_HEADER;
    uint16_t version = 1;

    // World & Position
    WorldLayer current_layer = WorldLayer::FLOATING_ISLES;
    uint16_t map_id = 0;
    int32_t player_x = 120;
    int32_t player_y = 80;

    // Currency & Progression
    uint32_t jade_taels = 150;
    uint16_t max_stamina = 100;
    uint8_t unlocked_party_slots = 1;

    // Story & World Flags (256 bits for Main Story, Side Quests, Chests, Bosses)
    bn::array<uint32_t, 8> story_flags = {};

    // Character Roster Save State
    bn::array<CharacterRuntimeState, MAX_ROSTER_SIZE> roster = {};
};

} // namespace aetherion

#endif
