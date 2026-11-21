#include "party_manager.h"

namespace aetherion {

PartyManager::PartyManager() {
    for (int i = 0; i < static_cast<int>(CharacterID::COUNT); ++i) {
        CharacterID id = static_cast<CharacterID>(i);
        const auto& tpl = get_character_template(id);
        _roster[i].unlocked = (tpl.unlock_method == UnlockMethod::STARTER);
        _roster[i].level = 1;
        _roster[i].max_hp = tpl.base_hp;
        _roster[i].current_hp = tpl.base_hp;
        _roster[i].active_essence = tpl.default_essence;
    }

    // Prologue starts with ONLY Ren (S-Rank Special) in Slot 0
    _unlocked_slots = 1;
    _active_slots[0] = CharacterID::REN;
    _active_slots[1] = CharacterID::REN;
    _active_slots[2] = CharacterID::REN;
    _active_slot_idx = 0;
}

void PartyManager::unlock_character(CharacterID id) {
    int idx = static_cast<int>(id);
    if (idx >= 0 && idx < MAX_ROSTER_SIZE) {
        _roster[idx].unlocked = true;
    }
}

void PartyManager::set_max_party_slots(uint8_t slots) {
    if (slots >= 1 && slots <= MAX_ACTIVE_PARTY) {
        _unlocked_slots = slots;
    }
}

bool PartyManager::assign_slot(uint8_t slot_index, CharacterID id) {
    if (slot_index >= _unlocked_slots) {
        return false;
    }
    if (!_roster[static_cast<int>(id)].unlocked) {
        return false;
    }
    _active_slots[slot_index] = id;
    return true;
}

bool PartyManager::swap_next_member() {
    if (_unlocked_slots <= 1 || _swap_cd > 0) {
        return false;
    }

    for (uint8_t offset = 1; offset < _unlocked_slots; ++offset) {
        uint8_t candidate = (_active_slot_idx + offset) % _unlocked_slots;
        CharacterID cid = _active_slots[candidate];
        if (_roster[static_cast<int>(cid)].current_hp > 0) {
            _active_slot_idx = candidate;
            _swap_cd = 90; // 1.5 second fast-swap cooldown
            return true;
        }
    }
    return false;
}

CharacterID PartyManager::active_character_id() const {
    return _active_slots[_active_slot_idx];
}

CharacterRuntimeState& PartyManager::active_character_state() {
    return _roster[static_cast<int>(active_character_id())];
}

const CharacterRuntimeState& PartyManager::character_state(CharacterID id) const {
    return _roster[static_cast<int>(id)];
}

CharacterRuntimeState& PartyManager::character_state_mut(CharacterID id) {
    return _roster[static_cast<int>(id)];
}

void PartyManager::update_cooldowns() {
    if (_swap_cd > 0) {
        --_swap_cd;
    }
    for (int i = 0; i < static_cast<int>(CharacterID::COUNT); ++i) {
        if (_roster[i].skill_cd_remaining > 0) {
            --_roster[i].skill_cd_remaining;
        }
    }
}

} // namespace aetherion
