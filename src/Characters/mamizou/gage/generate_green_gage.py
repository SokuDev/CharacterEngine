from PIL import Image

leaf = Image.open("leaf.png")
gage = Image.open("gage.png")


base = leaf.copy()
ipix = base.load()
opix = leaf.load()

ecolor = (0, 255, 0)
for y in range(0, leaf.size[1]):
	for x in range(0, leaf.size[0]):
		icolor = ipix[x, y]
		color = (icolor[0] * ecolor[0] // 255, icolor[1] * ecolor[1] // 255, icolor[2] * ecolor[2] // 255, icolor[3])
		opix[x, y] = color
result = gage.copy()
result.paste(leaf, (15, 0), leaf)
result.save(f"result/gageAa000.png")
