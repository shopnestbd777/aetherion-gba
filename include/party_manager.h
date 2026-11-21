#ifndef PARTY_MANAGER_H
#define PARTY_MANAGER_H

#include "characters.h"
#include "bn_array.h"

namespace aetherion {

constexpr int MAX_ACTIVE_PARTY = 3; // 3-character fast swap with L button

class PartyManager {
public:
    PartyManager();

    void unlock_character(CharacterID id);
    void set_max_party_slots(uint8_t slots); // Starts at 1, expands to 2, then 3
    uint8_t max_party_slots() const { return _unlocked_slots; }

    bool assign_slot(uint8_t slot_index, CharacterID id);
    bool swap_next_member(); // Triggered by L button

    CharacterID active_character_id() const;
    CharacterRuntimeState& active_character_state();
    const CharacterRuntimeState& character_state(CharacterID id) const;
    CharacterRuntimeState& character_state_mut(CharacterID id);

    void update_cooldowns();
    uint16_t swap_cooldown_frames() const { return _swap_cd; }

private:
    bn::array<CharacterRuntimeState, MAX_ROSTER_SIZE> _roster;
    bn::array<CharacterID, MAX_ACTIVE_PARTY> _active_slots;
    uint8_t _unlocked_slots = 1; // Starts solo with S-Rank Ren!
    uint8_t _active_slot_idx = 0;
    uint16_t _swap_cd = 0;
};

} // namespace aetherion

#endif
