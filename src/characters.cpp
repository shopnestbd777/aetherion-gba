#include "characters.h"
#include "bn_array.h"

namespace aetherion {

namespace {
constexpr bn::array<CharacterTemplate, static_cast<int>(CharacterID::COUNT)> ROSTER_DB = {{
    {
        CharacterID::REN, "Ren", "The Skybound",
        CharRank::S_RANK, Essence::GALE, WeaponType::SPIRIT_SWORD,
        UnlockMethod::STARTER, 0,
        1250, 118, 85, 300, true // Special S-Rank Starter Anomaly
    },
    {
        CharacterID::MEI_HUA, "Mei-Hua", "Ember Talisman",
        CharRank::C_RANK, Essence::BLAZE, WeaponType::TALISMAN_FAN,
        UnlockMethod::MAIN_QUEST, 0,
        920, 88, 62, 360, false
    },
    {
        CharacterID::KOJI, "Koji", "Jade Bulwark",
        CharRank::C_RANK, Essence::TERRA, WeaponType::HEAVY_GLAIVE,
        UnlockMethod::SIDE_QUEST, 0,
        1150, 76, 105, 420, false
    },
    {
        CharacterID::SORA, "Sora", "Misty Melodist",
        CharRank::B_RANK, Essence::TIDE, WeaponType::WATER_FLUTE,
        UnlockMethod::MAIN_QUEST, 0,
        1040, 84, 78, 480, false
    },
    {
        CharacterID::SHUN_LEI, "Shun-Lei", "Swift Thunder",
        CharRank::A_RANK, Essence::TEMPEST, WeaponType::TWIN_KUNAI,
        UnlockMethod::GUILD_SHOP, 2500,
        1080, 112, 74, 270, false
    },
    {
        CharacterID::YUEYING, "Yueying", "Frost Moon Sovereign",
        CharRank::S_RANK, Essence::FROST, WeaponType::CELESTIAL_ZITHER,
        UnlockMethod::MYTHIC_TRIAL, 0,
        1220, 126, 88, 390, true // Special S-Rank Mythic
    }
}};
} // namespace

const CharacterTemplate& get_character_template(CharacterID id) {
    return ROSTER_DB[static_cast<int>(id)];
}

} // namespace aetherion
