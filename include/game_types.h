#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include "bn_common.h"
#include "bn_fixed.h"

namespace aetherion {

// 6 Core Primordial Essences + Astral reserved for future Space updates
enum class Essence : uint8_t {
    NONE = 0,
    BLAZE,      // Crimson fire
    TIDE,       // Azure water
    TEMPEST,    // Violet sky-thunder
    FROST,      // Pale ice
    GALE,       // Jade wind
    TERRA,      // Amber stone
    ASTRAL      // Reserved for future Space / Astral Sea expansion
};

enum class ReactionType : uint8_t {
    NONE = 0,
    SCALD,          // Blaze + Tide (1.5x / 2.0x burst)
    THERMAL_SHOCK,  // Blaze + Frost (1.5x / 2.0x burst)
    ARC_SURGE,      // Tide + Tempest (chain lightning ticks)
    GLACIER_BIND,   // Tide + Frost (freeze + shatter)
    DETONATION,     // Blaze + Tempest (AoE knockback + armor break)
    RIME_BOLT,      // Frost + Tempest (physical defense shred)
    VORTEX_SPREAD,  // Gale + Blaze/Tide/Tempest/Frost (AoE aura spread)
    JADE_WARD       // Terra + Blaze/Tide/Tempest/Frost (drops elemental shield)
};

enum class CharRank : uint8_t {
    C_RANK = 0,
    B_RANK = 1,
    A_RANK = 2,
    S_RANK = 3      // Mythic / Special Anomaly
};

enum class WeaponType : uint8_t {
    SPIRIT_SWORD,
    TALISMAN_FAN,
    HEAVY_GLAIVE,
    WATER_FLUTE,
    TWIN_KUNAI,
    CELESTIAL_ZITHER
};

enum class UnlockMethod : uint8_t {
    STARTER,
    MAIN_QUEST,
    SIDE_QUEST,
    GUILD_SHOP,
    MYTHIC_TRIAL
};

enum class WorldLayer : uint8_t {
    MAINLAND = 0,       // Chinese/Japanese fantasy valleys, shrines, seas
    FLOATING_ISLES = 1, // Sky islands tethered by Spirit Chains
    UNDERWATER = 2,     // Submerged Coral Palace & Sunken Torii
    ASTRAL_SPACE = 3    // Reserved for future Space update
};

} // namespace aetherion

#endif
