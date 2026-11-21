#include "quest_dialogue.h"

namespace aetherion {

QuestDialogueManager::QuestDialogueManager() {
    open_dialogue("ELDER LIN", "SKYBOUND! PRESS A TO FIGHT SLIME");
}

bn::string_view QuestDialogueManager::active_quest_title() const {
    switch (_stage) {
        case StoryStage::PROLOGUE_AWAKEN:      return "Q1:TALK TO ELDER";
        case StoryStage::PROLOGUE_SLIME_TRIAL: return "Q1:SLAY BLAZE SLIME";
        case StoryStage::ACT1_DESCEND_VALLEY:  return "Q2:STEP ON RED GATE";
        case StoryStage::ACT1_MEIHUA_JOINED:   return "Q3:PRESS L TO SWAP!";
    }
    return "EXPLORE AETHERION";
}

void QuestDialogueManager::open_dialogue(bn::string_view speaker, bn::string_view line) {
    _dialogue_active = true;
    _speaker = speaker;
    _line = line;
}

void QuestDialogueManager::close_dialogue() {
    _dialogue_active = false;
}

void QuestDialogueManager::interact_with_npc(PartyManager& party, SaveData& save) {
    if (_stage == StoryStage::PROLOGUE_AWAKEN) {
        _stage = StoryStage::PROLOGUE_SLIME_TRIAL;
        open_dialogue("ELDER LIN", "SLAY THE SLIME WITH A! [GALE]");
    } else if (_stage == StoryStage::PROLOGUE_SLIME_TRIAL) {
        open_dialogue("ELDER LIN", "PRESS A NEAR THE RED SLIME!");
    } else if (_stage == StoryStage::ACT1_DESCEND_VALLEY) {
        open_dialogue("ELDER LIN", "WALK SOUTH TO THE RED PORTAL!");
    } else {
        // Heal party at Shrine Elder
        auto& active = party.active_character_state();
        active.current_hp = active.max_hp;
        save.jade_taels += 50;
        open_dialogue("MEI-HUA [C]", "MY BLAZE + YOUR GALE = VORTEX!");
    }
}

void QuestDialogueManager::notify_enemy_defeated(PartyManager& party, SaveData& save) {
    save.jade_taels += 120;
    if (_stage == StoryStage::PROLOGUE_SLIME_TRIAL) {
        _stage = StoryStage::ACT1_DESCEND_VALLEY;
        open_dialogue("QUEST CLEAR", "+120 TAELS! ENTER SOUTH GATE");
    }
}

void QuestDialogueManager::notify_entered_bamboo_valley(PartyManager& party, SaveData& save) {
    if (_stage == StoryStage::ACT1_DESCEND_VALLEY) {
        _stage = StoryStage::ACT1_MEIHUA_JOINED;
        // Unlock Party Slot 2 and recruit Mei-Hua (C-Rank Blaze)!
        party.set_max_party_slots(2);
        party.unlock_character(CharacterID::MEI_HUA);
        party.assign_slot(1, CharacterID::MEI_HUA);
        save.unlocked_party_slots = 2;
        open_dialogue("MEI-HUA JOINED!", "RANK C BLAZE! PRESS L TO SWAP");
    }
}

} // namespace aetherion
