#!/usr/bin/env python3
"""compare.py <a.png> <b.png> <x,y,w,h> [slider|frame]

Counts differing pixels between two screenshots inside a window rect.
Prints "DIFF <count> <bbox>" and exits 0. Pixel coordinates are Atari
screen coordinates; Hatari screenshots at --zoom 1 are 1:1 for VGA."""
import sys
from PIL import Image, ImageChops

a = Image.open(sys.argv[1]).convert("RGB")
b = Image.open(sys.argv[2]).convert("RGB")
x, y, w, h = (int(v) for v in sys.argv[3].split(","))
box = (x, y, x + w, y + h)
d = ImageChops.difference(a.crop(box), b.crop(box)).convert("L").point(lambda v: 255 if v else 0)
n = sum(1 for v in d.getdata() if v)
bb = d.getbbox()
if bb:
    bb = (bb[0] + x, bb[1] + y, bb[2] + x, bb[3] + y)
print(f"DIFF {n} {bb} size={a.size}")
