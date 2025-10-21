from PIL import Image

leaf = Image.open("leaf.png")
gage = Image.open("gage.png")

step = 32
all_colors = \
	[(255, i, 0) for i in range(0, 256, step)] +\
	[(i, 255, 0) for i in range(255, -1, -step)] +\
	[(0, 255, i) for i in range(0, 256, step)] +\
	[(0, i, 255) for i in range(255, -1, -step)] +\
	[(i, 0, 255) for i in range(0, 256, step)] +\
	[(255, 0, i) for i in range(255, -1, -step)]

index = leaf.size[1] % len(all_colors)
colors = []
while len(colors) + len(all_colors) < leaf.size[1]:
	colors += all_colors
colors += all_colors[:index]
base = leaf.copy()
ipix = base.load()
opix = leaf.load()

for i in range(0, len(all_colors)):
	colors.pop(0)
	colors.append(all_colors[index])
	index += 1
	index %= len(all_colors)
	for y in range(0, leaf.size[1]):
		for x in range(0, leaf.size[0]):
			ecolor = colors[len(colors) - y - 1]
			icolor = ipix[x, y]
			color = (icolor[0] * ecolor[0] // 255, icolor[1] * ecolor[1] // 255, icolor[2] * ecolor[2] // 255, icolor[3])
			opix[x, y] = color
	result = gage.copy()
	result.paste(leaf, (15, 0), leaf)
	result.save(f"result/gageAb{i:03d}.png")
