"""Small helpers for reading and writing pokeemerald tilesets and layouts.

Used by the vertical-slice map generators in this folder. Emerald layout only
(512-tile / 512-metatile / 6-palette primary).
"""
import os
import struct

from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
NUM_PRIMARY_TILES = 512
NUM_PRIMARY_METATILES = 512
NUM_PRIMARY_PALS = 6


def path(*p):
    return os.path.join(ROOT, *p)


def read_pal(p):
    lines = open(p).read().split('\n')[3:19]
    return [tuple(int(x) for x in l.split()) for l in lines if l.strip()]


def write_pal(p, colors):
    with open(p, 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n')
        for c in colors:
            f.write('%d %d %d\n' % c)


def read_u16s(p):
    raw = open(p, 'rb').read()
    return list(struct.unpack('<%dH' % (len(raw) // 2), raw))


def write_u16s(p, vals):
    with open(p, 'wb') as f:
        f.write(struct.pack('<%dH' % len(vals), *vals))


class Tileset:
    """One tileset directory: tiles (list of 64-int tuples), metatiles, attributes, palettes."""

    def __init__(self, d):
        self.dir = d
        im = Image.open(os.path.join(d, 'tiles.png'))
        self.png_palette = im.getpalette()
        w, h = im.size
        px = im.load()
        self.tiles = []
        for ty in range(h // 8):
            for tx in range(w // 8):
                self.tiles.append(tuple(px[tx * 8 + x, ty * 8 + y] & 15 for y in range(8) for x in range(8)))
        mt = read_u16s(os.path.join(d, 'metatiles.bin'))
        self.metatiles = [list(mt[i:i + 8]) for i in range(0, len(mt), 8)]
        self.attrs = read_u16s(os.path.join(d, 'metatile_attributes.bin'))
        self.pals = [read_pal(os.path.join(d, 'palettes', '%02d.pal' % i)) for i in range(16)]

    def save(self, d, num_tiles_min=0):
        os.makedirs(os.path.join(d, 'palettes'), exist_ok=True)
        tiles = list(self.tiles)
        while len(tiles) % 16:
            tiles.append((0,) * 64)
        im = Image.new('P', (128, len(tiles) // 16 * 8))
        if self.png_palette:
            im.putpalette(self.png_palette)
        px = im.load()
        for i, t in enumerate(tiles):
            tx, ty = i % 16, i // 16
            for j, c in enumerate(t):
                px[tx * 8 + j % 8, ty * 8 + j // 8] = c
        im.save(os.path.join(d, 'tiles.png'))
        write_u16s(os.path.join(d, 'metatiles.bin'), [v for m in self.metatiles for v in m])
        write_u16s(os.path.join(d, 'metatile_attributes.bin'), self.attrs)
        for i, p in enumerate(self.pals):
            write_pal(os.path.join(d, 'palettes', '%02d.pal' % i), p)


def flip_tile(t, hf, vf):
    rows = [list(t[y * 8:(y + 1) * 8]) for y in range(8)]
    if hf:
        rows = [r[::-1] for r in rows]
    if vf:
        rows = rows[::-1]
    return tuple(c for r in rows for c in r)


def tileset_dir_for(symbol):
    """gTileset_Foo -> data/tilesets/.../foo using src/data/tilesets/metatiles.h."""
    import re
    src = open(path('src/data/tilesets/metatiles.h')).read()
    short = symbol.replace('gTileset_', '')
    m = re.search(r'gMetatiles_%s\[\] = INCBIN_U16\("([^"]+)/metatiles.bin"\)' % short, src)
    return path(m.group(1))


class Layout:
    def __init__(self, w, h, fill=0):
        self.w, self.h = w, h
        self.b = [fill] * (w * h)

    @classmethod
    def load(cls, p, w):
        vals = read_u16s(p)
        l = cls(w, len(vals) // w)
        l.b = vals
        return l

    def get(self, x, y):
        return self.b[y * self.w + x]

    def set(self, x, y, v):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.b[y * self.w + x] = v

    def copy(self):
        l = Layout(self.w, self.h)
        l.b = list(self.b)
        return l

    def save(self, p):
        os.makedirs(os.path.dirname(p), exist_ok=True)
        write_u16s(p, self.b)


def block(mt, collision=0, elevation=3):
    return (mt & 0x3FF) | ((collision & 3) << 10) | ((elevation & 15) << 12)
