#ifndef REACTIONS_H
#define REACTIONS_H

#include "game_types.h"

namespace aetherion {

struct EssenceAura {
    Essence element = Essence::NONE;
    uint16_t remaining_frames = 0; // 480 frames = 8 seconds
};

struct ReactionResult {
    ReactionType type = ReactionType::NONE;
    bn::fixed damage_multiplier = 1.0;
    bool applies_freeze = false;
    bool spreads_aoe = false;
    bool drops_jade_ward = false;
    Essence ward_essence = Essence::NONE;
};

// Evaluates incoming Essence against an enemy's existing EssenceAura
ReactionResult evaluate_essence_hit(EssenceAura& target_aura, Essence incoming_essence);

} // namespace aetherion

#endif
