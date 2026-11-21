#include "reactions.h"

namespace aetherion {

ReactionResult evaluate_essence_hit(EssenceAura& target_aura, Essence incoming) {
    ReactionResult result;
    if (incoming == Essence::NONE) {
        return result;
    }

    // If target has no active Essence, apply the incoming Essence for 8 seconds
    if (target_aura.element == Essence::NONE || target_aura.remaining_frames == 0) {
        target_aura.element = incoming;
        target_aura.remaining_frames = 480;
        return result;
    }

    // Same element refreshes duration
    if (target_aura.element == incoming) {
        target_aura.remaining_frames = 480;
        return result;
    }

    Essence existing = target_aura.element;

    // 1. SCALD (Blaze + Tide)
    if ((existing == Essence::BLAZE && incoming == Essence::TIDE) ||
        (existing == Essence::TIDE && incoming == Essence::BLAZE)) {
        result.type = ReactionType::SCALD;
        result.damage_multiplier = (incoming == Essence::TIDE) ? bn::fixed(2.0) : bn::fixed(1.5);
        target_aura = EssenceAura{};
        return result;
    }

    // 2. THERMAL SHOCK (Blaze + Frost)
    if ((existing == Essence::BLAZE && incoming == Essence::FROST) ||
        (existing == Essence::FROST && incoming == Essence::BLAZE)) {
        result.type = ReactionType::THERMAL_SHOCK;
        result.damage_multiplier = (incoming == Essence::BLAZE) ? bn::fixed(2.0) : bn::fixed(1.5);
        target_aura = EssenceAura{};
        return result;
    }

    // 3. ARC SURGE (Tide + Tempest)
    if ((existing == Essence::TIDE && incoming == Essence::TEMPEST) ||
        (existing == Essence::TEMPEST && incoming == Essence::TIDE)) {
        result.type = ReactionType::ARC_SURGE;
        result.damage_multiplier = bn::fixed(1.4);
        result.spreads_aoe = true;
        target_aura = EssenceAura{};
        return result;
    }

    // 4. GLACIER BIND (Tide + Frost)
    if ((existing == Essence::TIDE && incoming == Essence::FROST) ||
        (existing == Essence::FROST && incoming == Essence::TIDE)) {
        result.type = ReactionType::GLACIER_BIND;
        result.damage_multiplier = bn::fixed(1.2);
        result.applies_freeze = true;
        target_aura = EssenceAura{};
        return result;
    }

    // 5. DETONATION (Blaze + Tempest)
    if ((existing == Essence::BLAZE && incoming == Essence::TEMPEST) ||
        (existing == Essence::TEMPEST && incoming == Essence::BLAZE)) {
        result.type = ReactionType::DETONATION;
        result.damage_multiplier = bn::fixed(1.8);
        result.spreads_aoe = true;
        target_aura = EssenceAura{};
        return result;
    }

    // 6. RIME BOLT (Frost + Tempest)
    if ((existing == Essence::FROST && incoming == Essence::TEMPEST) ||
        (existing == Essence::TEMPEST && incoming == Essence::FROST)) {
        result.type = ReactionType::RIME_BOLT;
        result.damage_multiplier = bn::fixed(1.3);
        target_aura = EssenceAura{};
        return result;
    }

    // 7. VORTEX SPREAD (Gale + Blaze/Tide/Tempest/Frost)
    if (incoming == Essence::GALE &&
        (existing == Essence::BLAZE || existing == Essence::TIDE ||
         existing == Essence::TEMPEST || existing == Essence::FROST)) {
        result.type = ReactionType::VORTEX_SPREAD;
        result.damage_multiplier = bn::fixed(1.35);
        result.spreads_aoe = true;
        target_aura = EssenceAura{};
        return result;
    }

    // 8. JADE WARD (Terra + Blaze/Tide/Tempest/Frost)
    if (incoming == Essence::TERRA &&
        (existing == Essence::BLAZE || existing == Essence::TIDE ||
         existing == Essence::TEMPEST || existing == Essence::FROST)) {
        result.type = ReactionType::JADE_WARD;
        result.damage_multiplier = bn::fixed(1.1);
        result.drops_jade_ward = true;
        result.ward_essence = existing;
        target_aura = EssenceAura{};
        return result;
    }

    return result;
}

} // namespace aetherion
