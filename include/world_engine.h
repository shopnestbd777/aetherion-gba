#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H

#include "game_types.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_string_view.h"
#include "bn_vector.h"

namespace aetherion {

constexpr int MAP_COLS = 32;
constexpr int MAP_ROWS = 32;
constexpr int MAP_CELL_COUNT = MAP_COLS * MAP_ROWS;

enum class RegionID : uint8_t {
    SKY_ISLET = 0,       // Prologue Floating Island
    BAMBOO_VALLEY = 1,   // Mainland Eastern Village & Shrine
    SUNKEN_TORII = 2     // Underwater Coral Palace
};

class WorldEngine {
public:
    WorldEngine();

    void load_region(RegionID region, bn::camera_ptr& camera);
    RegionID current_region() const { return _region; }
    bn::string_view current_region_name() const;

    bool is_walkable(bn::fixed world_x, bn::fixed world_y) const;
    uint8_t tile_at_world(bn::fixed world_x, bn::fixed world_y) const;

    // Checks if player stepped onto a Sky Chain Portal or Tide Pearl Dive Swirl
    bool check_region_portal(bn::fixed world_x, bn::fixed world_y, RegionID& out_target, bn::fixed_point& out_spawn) const;

private:
    RegionID _region = RegionID::SKY_ISLET;
    alignas(4) bn::regular_bg_map_cell _cells[MAP_CELL_COUNT];
    bn::optional<bn::regular_bg_map_item> _map_item;
    bn::optional<bn::regular_bg_ptr> _bg;
};

} // namespace aetherion

#endif
