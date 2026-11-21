#ifndef QUEST_DIALOGUE_H
#define QUEST_DIALOGUE_H

#include "party_manager.h"
#include "save_data.h"
#include "bn_string_view.h"

namespace aetherion {

enum class StoryStage : uint8_t {
    PROLOGUE_AWAKEN = 0,     // Speak to Shrine Elder on Floating Islet
    PROLOGUE_SLIME_TRIAL = 1,// Defeat the Corrupted Blaze Slime
    ACT1_DESCEND_VALLEY = 2, // Use Sky Portal to descend to Bamboo Valley
    ACT1_MEIHUA_JOINED = 3   // Mei-Hua (C-Rank Blaze) joins Party Slot 2!
};

class QuestDialogueManager {
public:
    QuestDialogueManager();

    StoryStage stage() const { return _stage; }
    bn::string_view active_quest_title() const;

    bool is_dialogue_open() const { return _dialogue_active; }
    bn::string_view current_speaker() const { return _speaker; }
    bn::string_view current_line() const { return _line; }

    void interact_with_npc(PartyManager& party, SaveData& save);
    void notify_enemy_defeated(PartyManager& party, SaveData& save);
    void notify_entered_bamboo_valley(PartyManager& party, SaveData& save);
    void close_dialogue();

private:
    void open_dialogue(bn::string_view speaker, bn::string_view line);

    StoryStage _stage = StoryStage::PROLOGUE_AWAKEN;
    bool _dialogue_active = false;
    bn::string_view _speaker;
    bn::string_view _line;
};

} // namespace aetherion

#endif
