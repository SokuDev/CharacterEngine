from PIL import Image
import sys

colors = {
    "5A3029": 85,
    "7C4129": 86,
    "B35622": 87,
    "EA864C": 88
}

load = Image.open(sys.argv[1]).convert("RGB")
loadpx = load.load()
palette = [0, 0, 0] * 256
for i, k in colors.items():
    palette[k * 3 + 0] = int(i[:2], 16)
    palette[k * 3 + 1] = int(i[2:4], 16)
    palette[k * 3 + 2] = int(i[4:6], 16)

p_img = Image.new('P', (16, 16))
p_img.putpalette(palette)
conv = load.quantize(palette=p_img, dither=0)
conv.save(sys.argv[1].split('/')[-1])
exit(0)
