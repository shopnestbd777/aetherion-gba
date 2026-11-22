#include "world_engine.h"
#include "bn_regular_bg_items_world_tiles.h"
#include "bn_regular_bg_map_cell_info.h"

namespace aetherion {

WorldEngine::WorldEngine() {
    for (int i = 0; i < MAP_CELL_COUNT; ++i) {
        _cells[i] = 0;
    }
}

bn::string_view WorldEngine::current_region_name() const {
    switch (_region) {
        case RegionID::SKY_ISLET:     return "SKY ISLET";
        case RegionID::BAMBOO_VALLEY: return "BAMBOO VALE";
        case RegionID::SUNKEN_TORII:  return "SUNKEN TORII";
    }
    return "AETHERION";
}

void WorldEngine::load_region(RegionID region, bn::camera_ptr& camera) {
    _region = region;
    _bg.reset();

    for (int y = 0; y < MAP_ROWS; ++y) {
        for (int x = 0; x < MAP_COLS; ++x) {
            uint16_t tile_id = 0;

            if (region == RegionID::SKY_ISLET) {
                // Floating Island surrounded by Sky Clouds (tile 0)
                if (x >= 5 && x <= 26 && y >= 5 && y <= 26) {
                    if (x == 5 || x == 26 || y == 5 || y == 26) {
                        tile_id = 2; // Gold border
                    } else if ((x == 15 || x == 16) && y >= 10 && y <= 22) {
                        tile_id = 4; // Central stone walkway
                    } else if (x >= 14 && x <= 17 && y == 24) {
                        tile_id = 5; // Sky Tether Portal to Mainland!
                    } else {
                        tile_id = 1; // Floating Jade Stone
                    }
                } else {
                    tile_id = 0; // Sky void
                }
            } else if (region == RegionID::BAMBOO_VALLEY) {
                // Mainland Eastern Valley with River & Torii Shrine
                if (x <= 1 || x >= 30 || y <= 1 || y >= 30) {
                    tile_id = 1; // Mountain cliff border
                } else if (y >= 21 && y <= 23) {
                    if (x >= 14 && x <= 17) {
                        tile_id = 4; // Wooden/Stone bridge over river
                    } else {
                        tile_id = 6; // Spirit River (deep water -> Underwater portal at x=24)
                    }
                } else if (x >= 13 && x <= 18 && y >= 5 && y <= 9) {
                    tile_id = 5; // Vermilion Torii Shrine plaza
                } else if (x >= 15 && x <= 16 && y >= 4 && y <= 27) {
                    tile_id = 4; // Village path
                } else {
                    tile_id = 3; // Bamboo grass
                }
            } else {
                // Underwater Coral Palace (Sunken Torii)
                if (x <= 2 || x >= 29 || y <= 2 || y >= 29) {
                    tile_id = 6; // Deep ocean wall
                } else if (x >= 14 && x <= 17 && y == 6) {
                    tile_id = 5; // Ascent whirlpool back to Mainland
                } else {
                    tile_id = 7; // Coral floor
                }
            }

            bn::regular_bg_map_cell cell = 0;
            bn::regular_bg_map_cell_info info(cell);
            info.set_tile_id(tile_id);
            _cells[y * MAP_COLS + x] = info.cell();
        }
    }

    _map_item.emplace(_cells[0], bn::size(MAP_COLS, MAP_ROWS));
    bn::regular_bg_item bg_item(
        bn::regular_bg_items::world_tiles.tiles_item(),
        bn::regular_bg_items::world_tiles.palette_item(),
        *_map_item
    );

    _bg = bg_item.create_bg(0, 0);
    _bg->set_camera(camera);
}

uint8_t WorldEngine::tile_at_world(bn::fixed world_x, bn::fixed world_y) const {
    // Map is 256x256 centered at (0,0) -> top-left is (-128, -128)
    int tx = (world_x.integer() + 128) / 8;
    int ty = (world_y.integer() + 128) / 8;
    if (tx < 0 || tx >= MAP_COLS || ty < 0 || ty >= MAP_ROWS) {
        return 0;
    }
    bn::regular_bg_map_cell_info info(_cells[ty * MAP_COLS + tx]);
    return static_cast<uint8_t>(info.tile_id());
}

bool WorldEngine::is_walkable(bn::fixed world_x, bn::fixed world_y) const {
    uint8_t t = tile_at_world(world_x, world_y);
    if (_region == RegionID::SKY_ISLET) {
        return t != 0; // Cannot walk off floating island clouds
    }
    if (_region == RegionID::BAMBOO_VALLEY) {
        // Water (tile 6) is swimmable/diveable at x>=22, solid cliff (tile 1) blocks
        return t != 1;
    }
    return t != 6;
}

bool WorldEngine::check_region_portal(bn::fixed world_x, bn::fixed world_y, RegionID& out_target, bn::fixed_point& out_spawn) const {
    uint8_t t = tile_at_world(world_x, world_y);
    if (_region == RegionID::SKY_ISLET && t == 5) {
        out_target = RegionID::BAMBOO_VALLEY;
        out_spawn = bn::fixed_point(0, -40);
        return true;
    }
    if (_region == RegionID::BAMBOO_VALLEY && t == 6 && world_x > 48) {
        // Dive into deep river swirl -> Underwater Sunken Torii!
        out_target = RegionID::SUNKEN_TORII;
        out_spawn = bn::fixed_point(0, 40);
        return true;
    }
    if (_region == RegionID::SUNKEN_TORII && t == 5) {
        out_target = RegionID::BAMBOO_VALLEY;
        out_spawn = bn::fixed_point(0, 20);
        return true;
    }
    return false;
}

} // namespace aetherion
