#!/usr/bin/env python3
"""Print a Hatari screenshot as text (one char per 5x8 pixel cell), so a
CI log shows what was on screen when artifacts can't be fetched."""
import sys
from PIL import Image

RAMP = " .:-=+*#%@"
for path in sys.argv[1:]:
    im = Image.open(path).convert("L")
    w, h = im.size
    cw, ch = 5, 8
    im = im.resize((w // cw, h // ch), Image.BOX)
    print(f"--- {path} ({w}x{h})")
    px = im.load()
    for y in range(im.size[1]):
        print("|" + "".join(RAMP[(255 - px[x, y]) * (len(RAMP) - 1) // 255] for x in range(im.size[0])).rstrip())
