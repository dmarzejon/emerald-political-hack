#!/usr/bin/env python3
"""Make the slice trainer pics as palette swaps of vanilla front pics.

Each new pic keeps its base pic's pixels and only replaces palette entries, so
it stays a valid 64x64, 16-colour GBA sprite. Run from the repo root:

    python3 dev_scripts/trainer_pics/recolor.py [--preview out.png]
"""
import sys
from PIL import Image, ImageDraw

DIR = "graphics/trainers/front_pics/"

# new pic: (base pic, {palette index: (r, g, b)})
RECOLORS = {
    "prince_corwin": ("rich_boy", {
        5: (189, 74, 90), 6: (148, 49, 74), 7: (106, 32, 57), 8: (65, 24, 41)}),
    "princess_isolde": ("lady", {
        5: (32, 57, 32), 7: (213, 123, 82), 8: (164, 82, 57),
        11: (148, 205, 123), 12: (90, 156, 90), 13: (49, 106, 65)}),
    "tamsin": ("pokemon_ranger_f", {
        6: (115, 106, 106), 7: (74, 65, 65), 8: (41, 36, 36),
        10: (230, 180, 90), 11: (189, 106, 65), 12: (148, 74, 49), 13: (106, 49, 41)}),
    "guild_clerk": ("super_nerd_frlg", {
        5: (230, 189, 82), 6: (180, 139, 49), 7: (139, 106, 74), 8: (90, 65, 49),
        9: (222, 205, 164), 10: (189, 164, 123)}),
    "foreman": ("engineer_frlg", {
        10: (164, 98, 57), 13: (222, 164, 106), 14: (189, 123, 74), 15: (115, 74, 49)}),
    "poacher": ("burglar_frlg", {
        6: (213, 197, 172), 7: (189, 164, 139), 8: (156, 131, 106), 9: (123, 98, 82),
        11: (123, 156, 90), 12: (90, 123, 65), 13: (156, 189, 123), 14: (106, 139, 82),
        15: (65, 90, 49)}),
    "carter": ("hiker", {
        5: (189, 148, 98), 6: (148, 106, 65), 11: (106, 82, 49),
        12: (90, 123, 189), 13: (57, 82, 139)}),
    "acolyte": ("hex_maniac", {
        5: (238, 238, 230), 6: (205, 205, 197), 7: (164, 164, 164), 8: (106, 106, 115),
        11: (156, 172, 197), 12: (82, 90, 115), 13: (115, 131, 156)}),
    "bailiff": ("pokemon_ranger_m", {
        10: (205, 189, 106), 11: (90, 139, 90), 12: (57, 106, 65), 13: (41, 74, 49)}),
    "gardener": ("aroma_lady", {
        5: (213, 238, 189), 6: (172, 213, 148), 7: (123, 172, 98), 8: (82, 123, 65)}),
}


def gba(c):
    # Snap to the 5-bit-per-channel colours the GBA can show.
    return tuple((v >> 3) << 3 for v in c)


def make(name):
    base, changes = RECOLORS[name]
    im = Image.open(DIR + base + ".png")
    pal = im.getpalette()[:48]
    for i, c in changes.items():
        pal[i * 3:i * 3 + 3] = gba(c)
    out = im.copy()
    out.putpalette(pal)
    out.save(DIR + name + ".png")
    return Image.open(DIR + base + ".png"), out


def main():
    pairs = [(n, *make(n)) for n in RECOLORS]
    if "--preview" in sys.argv:
        sheet = Image.new("RGB", (len(pairs) * 66, 150), "white")
        d = ImageDraw.Draw(sheet)
        for i, (n, base, new) in enumerate(pairs):
            sheet.paste(base.convert("RGB"), (i * 66, 12))
            sheet.paste(new.convert("RGB"), (i * 66, 80))
            d.text((i * 66 + 1, 0), n[:11], fill="black")
        sheet = sheet.resize((sheet.width * 2, sheet.height * 2), Image.NEAREST)
        sheet.save(sys.argv[sys.argv.index("--preview") + 1])


if __name__ == "__main__":
    main()
