import os
import json
import struct

os.makedirs("graphics", exist_ok=True)

def write_4bpp_bmp(filename, width, height, palette_rgb, pixels):
    """Writes a standard 4-bit indexed BMP compatible with Butano / grit."""
    assert width % 8 == 0 and height % 8 == 0
    assert len(palette_rgb) <= 16
    pal = list(palette_rgb) + [(0, 0, 0)] * (16 - len(palette_rgb))

    row_bytes = width // 2
    pixel_data_size = row_bytes * height
    palette_size = 16 * 4
    header_size = 14 + 40 + palette_size
    file_size = header_size + pixel_data_size

    with open(filename, "wb") as f:
        # BITMAPFILEHEADER
        f.write(struct.pack("<2sIHHI", b"BM", file_size, 0, 0, header_size))
        # BITMAPINFOHEADER
        f.write(struct.pack("<IIIHHIIIIII",
            40, width, height, 1, 4, 0, pixel_data_size, 2835, 2835, 16, 16))
        # RGBQUAD Palette (B, G, R, 0)
        for r, g, b in pal:
            f.write(struct.pack("<BBBB", b, g, r, 0))
        # Bottom-up 4bpp pixel rows
        for y in range(height - 1, -1, -1):
            row = bytearray(row_bytes)
            for x in range(0, width, 2):
                p0 = pixels[y * width + x] & 0x0F
                p1 = pixels[y * width + x + 1] & 0x0F
                row[x // 2] = (p0 << 4) | p1
            f.write(row)

def write_json(filename, data):
    with open(filename, "w") as f:
        json.dump(data, f, indent=2)

# 1. Generate 16x16 Hero Sprite (Ren - Skybound Swordsman)
hero_pal = [
    (255, 0, 255),   # 0: Transparent magenta
    (24, 28, 40),    # 1: Dark ink outline
    (245, 224, 198), # 2: Skin
    (46, 196, 148),  # 3: Gale Jade scarf/trim
    (240, 244, 248), # 4: White Eastern tunic
    (36, 58, 94),    # 5: Navy hakama/pants
    (235, 190, 65),  # 6: Gold S-rank crest
    (110, 235, 210), # 7: Spirit blade glow
]
hero_art = [
    "0000011111100000",
    "0000155555510000",
    "0001555665551000",
    "0001222222221000",
    "0001212222121000",
    "0000122222210000",
    "0001333333331000",
    "0014443663444100",
    "0124444334444217",
    "0121444444441217",
    "0001555555551070",
    "0001555115551060",
    "0001551001551000",
    "0001551001551000",
    "0001110000111000",
    "0000000000000000",
]
hero_px = [int(ch) for row in hero_art for ch in row]
write_4bpp_bmp("graphics/hero_sprite.bmp", 16, 16, hero_pal, hero_px)
write_json("graphics/hero_sprite.json", {"type": "sprite", "width": 16, "height": 16})

# 2. Generate 16x16 Spirit Slime Enemy Sprite
enemy_pal = [
    (255, 0, 255),   # 0: Transparent
    (35, 18, 48),    # 1: Outline
    (220, 68, 74),   # 2: Blaze Crimson body
    (250, 145, 90),  # 3: Highlight
    (255, 245, 210), # 4: Glowing eyes
]
enemy_art = [
    "0000001111000000",
    "0000113333110000",
    "0001332222221000",
    "0013222222222100",
    "0122222222222210",
    "0122442222442210",
    "0122412222412210",
    "0122222222222210",
    "1222222112222221",
    "1222222222222221",
    "0122222222222210",
    "0011111111111100",
    "0000000000000000",
    "0000000000000000",
    "0000000000000000",
    "0000000000000000",
]
enemy_px = [int(ch) for row in enemy_art for ch in row]
write_4bpp_bmp("graphics/enemy_sprite.bmp", 16, 16, enemy_pal, enemy_px)
write_json("graphics/enemy_sprite.json", {"type": "sprite", "width": 16, "height": 16})

# 3. Generate 16x16 Shrine Elder / Companion NPC Sprite
npc_pal = [
    (255, 0, 255),   # 0: Transparent
    (32, 24, 24),    # 1: Outline
    (245, 224, 198), # 2: Skin
    (214, 58, 68),   # 3: Vermilion shrine robe
    (245, 200, 75),  # 4: Gold trim
    (245, 245, 250), # 5: Hair / collar
]
npc_art = [
    "0000011111100000",
    "0000155555510000",
    "0001555445551000",
    "0001222222221000",
    "0001212222121000",
    "0000122222210000",
    "0001555555551000",
    "0013334444333100",
    "0123333443333210",
    "0121333333331210",
    "0001333333331000",
    "0001444444441000",
    "0001333333331000",
    "0001333333331000",
    "0000111111110000",
    "0000000000000000",
]
npc_px = [int(ch) for row in npc_art for ch in row]
write_4bpp_bmp("graphics/npc_sprite.bmp", 16, 16, npc_pal, npc_px)
write_json("graphics/npc_sprite.json", {"type": "sprite", "width": 16, "height": 16})

# 4. Generate 8x8 World Tileset (8 tiles stacked vertically: 8x64)
# Tile 0: Sky Void (Floating Island clouds)
# Tile 1: Floating Island Jade Stone
# Tile 2: Floating Island Gold/Jade Trim Edge
# Tile 3: Mainland Bamboo Grass
# Tile 4: Mainland Stone Path
# Tile 5: Torii / Shrine Red Pillar Tile
# Tile 6: Spirit River / Sea Water
# Tile 7: Underwater Coral Floor
tile_pal = [
    (72, 132, 198),  # 0: Sky azure
    (225, 240, 252), # 1: Cloud white
    (82, 148, 128),  # 2: Jade stone base
    (118, 188, 162), # 3: Jade stone highlight
    (224, 184, 72),  # 4: Celestial gold trim
    (68, 148, 76),   # 5: Bamboo grass
    (96, 176, 98),   # 6: Grass blade highlight
    (168, 162, 148), # 7: Stone path
    (196, 56, 56),   # 8: Torii vermilion
    (42, 108, 184),  # 9: Deep river/sea water
    (92, 172, 232),  # 10: Water ripple
    (48, 136, 148),  # 11: Underwater teal sand
    (216, 112, 148), # 12: Underwater pink coral
]
tiles_px = []
for t in range(8):
    for y in range(8):
        for x in range(8):
            if t == 0:   # Sky Cloud Void
                c = 1 if ((x + y) % 7 == 0 or (x in (2,3) and y == 4)) else 0
            elif t == 1: # Floating Jade Stone
                c = 3 if (x == 0 or y == 0 or (x + y) % 5 == 0) else 2
            elif t == 2: # Celestial Gold Border
                c = 4 if (y in (0, 1, 6, 7) or x in (0, 7)) else 2
            elif t == 3: # Mainland Bamboo Grass
                c = 6 if ((x * 3 + y * 5) % 7 == 0) else 5
            elif t == 4: # Mainland Stone Path
                c = 3 if ((x + y) % 4 == 0) else 7
            elif t == 5: # Torii Shrine Tile
                c = 4 if (y == 1 or y == 6) else 8
            elif t == 6: # Spirit River / Sea
                c = 10 if ((x + y * 2) % 6 == 0) else 9
            else:        # Underwater Coral Palace Floor
                c = 12 if ((x in (2, 3) and y in (3, 4)) or (x == 6 and y == 1)) else 11
            tiles_px.append(c)

write_4bpp_bmp("graphics/world_tiles.bmp", 8, 64, tile_pal, tiles_px)
write_json("graphics/world_tiles.json", {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"})

# 5. Generate Crisp 8x8 ASCII Font Sprite Sheet (95 printable chars: 8 x 760)
FONT_5X7 = {
    ' ': [0,0,0,0,0,0,0], '!': [4,4,4,4,4,0,4], '"': [10,10,0,0,0,0,0],
    '#': [10,31,10,10,31,10,0], '$': [4,15,20,14,5,30,4], '%': [24,25,2,4,8,19,3],
    '&': [8,20,20,8,21,18,13], "'": [4,4,0,0,0,0,0], '(': [2,4,8,8,8,4,2],
    ')': [8,4,2,2,2,4,8], '*': [0,4,21,14,21,4,0], '+': [0,4,4,31,4,4,0],
    ',': [0,0,0,0,0,4,8], '-': [0,0,0,31,0,0,0], '.': [0,0,0,0,0,4,4],
    '/': [1,2,2,4,8,8,16], '0': [14,17,19,21,25,17,14], '1': [4,12,4,4,4,4,14],
    '2': [14,17,1,2,4,8,31], '3': [31,2,4,2,1,17,14], '4': [2,6,10,18,31,2,2],
    '5': [31,16,30,1,1,17,14], '6': [6,8,16,30,17,17,14], '7': [31,1,2,4,8,8,8],
    '8': [14,17,17,14,17,17,14], '9': [14,17,17,15,1,2,12], ':': [0,4,4,0,4,4,0],
    ';': [0,4,4,0,4,4,8], '<': [2,4,8,16,8,4,2], '=': [0,0,31,0,31,0,0],
    '>': [8,4,2,1,2,4,8], '?': [14,17,1,2,4,0,4], '@': [14,17,23,21,23,16,14],
    'A': [14,17,17,31,17,17,17], 'B': [30,17,17,30,17,17,30], 'C': [14,17,16,16,16,17,14],
    'D': [28,18,17,17,17,18,28], 'E': [31,16,16,30,16,16,31], 'F': [31,16,16,30,16,16,16],
    'G': [14,17,16,23,17,17,15], 'H': [17,17,17,31,17,17,17], 'I': [14,4,4,4,4,4,14],
    'J': [7,2,2,2,2,18,12], 'K': [17,18,20,24,20,18,17], 'L': [16,16,16,16,16,16,31],
    'M': [17,27,21,21,17,17,17], 'N': [17,17,25,21,19,17,17], 'O': [14,17,17,17,17,17,14],
    'P': [30,17,17,30,16,16,16], 'Q': [14,17,17,17,21,18,13], 'R': [30,17,17,30,20,18,17],
    'S': [15,16,16,14,1,1,30], 'T': [31,4,4,4,4,4,4], 'U': [17,17,17,17,17,17,14],
    'V': [17,17,17,17,17,10,4], 'W': [17,17,17,21,21,27,17], 'X': [17,17,10,4,10,17,17],
    'Y': [17,17,10,4,4,4,4], 'Z': [31,1,2,4,8,16,31], '[': [14,8,8,8,8,8,14],
    '\\': [16,8,8,4,2,2,1], ']': [14,2,2,2,2,2,14], '^': [4,10,17,0,0,0,0],
    '_': [0,0,0,0,0,0,31], '`': [8,4,0,0,0,0,0],
}
font_pal = [(255, 0, 255), (255, 255, 255), (20, 24, 36)]
font_px = []
for code in range(32, 127):
    ch = chr(code).upper()
    glyph = FONT_5X7.get(ch, [0,0,0,0,0,0,0])
    for y in range(8):
        for x in range(8):
            if y < 7 and x < 5 and (glyph[y] & (1 << (4 - x))):
                font_px.append(1)
            elif y > 0 and x > 0 and (y - 1) < 7 and (x - 1) < 5 and (glyph[y - 1] & (1 << (4 - (x - 1)))):
                font_px.append(2) # Drop shadow for readability over bright sky/grass
            else:
                font_px.append(0)

write_4bpp_bmp("graphics/font_8x8.bmp", 8, 8 * 95, font_pal, font_px)
write_json("graphics/font_8x8.json", {"type": "sprite", "width": 8, "height": 8})
print("GBA graphics generated successfully!")
          
