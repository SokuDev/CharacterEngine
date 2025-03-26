#!/bin/python3
from PIL import Image
import math

head = Image.open("hammer_head.png")
stick = Image.open("hammer_stick.png")
pixels = stick.load()

offsets = [1, 0, 0, -2, -2, -2, -2, -2, -2, 0, 0, -1, -1, -2, -2, -1, -2, -2]
i = 0
for angle in range(-45, 45, 5):
    img = Image.new("RGBA", (111, 80))
    pos = [img.size[0] // 3 - 1, 79]
    px = img.load()
    rotated = head.rotate(-angle, expand=True)
    for y in range(stick.size[1]):
        pos[0] += math.sin(angle * y / stick.size[1] * math.pi / 180) * 4 / 3
        x = round(pos[0])
        for j in range(3):
            px[x + j, pos[1] - y] = pixels[j, stick.size[1] - 1 - y]
    img.paste(rotated, (round(pos[0] - 4 + offsets[i - 1]), pos[1] - stick.size[1] - 14), rotated)
    img.save(f"hammerBullet{i:03d}.png")
    i += 1
