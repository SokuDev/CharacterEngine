import bs4

soup = bs4.BeautifulSoup(open("tewi.xml", "rb"), features="xml")
f = [i for i in soup.findAll("move") if i.attrs['id'] == '858']
f1 = f[0]
last = f1
for i in f[1:]:
	i.decompose()

increaseUntech = lambda u, i: u + i * 15
increaseSpeedX = lambda x, i: x + i * 100
increaseSpeedY = lambda y, i: y + i * 250
increaseScale = lambda i: 200 + i * 100
increaseBox = lambda i: increaseScale(i) / 200

for i in range(1, 4):
	f2 = bs4.BeautifulSoup(str(f1), features="xml")
	f2.find("move").attrs["index"] = i
	for box in f2.findAll("box"):
		box.attrs["down"]  = str(increaseBox(i) * int(box.attrs["down"]))
		box.attrs["up"]    = str(increaseBox(i) * int(box.attrs["up"]))
		box.attrs["left"]  = str(increaseBox(i)* int(box.attrs["left"]))
		box.attrs["right"] = str(increaseBox(i) * int(box.attrs["right"]))
	for blend in f2.findAll("blend"):
		blend.attrs["xscale"] = str(increaseScale(i))
		blend.attrs["yscale"] = str(increaseScale(i))
	for blend in f2.findAll("traits"):
		blend.attrs["xspeed"] = str(increaseSpeedX(int(blend.attrs["xspeed"]), i))
		blend.attrs["yspeed"] = str(increaseSpeedY(int(blend.attrs["yspeed"]), i))
		blend.attrs["untech"] = str(increaseUntech(int(blend.attrs["untech"]), i))
	last.insert_after(f2)
	last=[i for i in soup.findAll("move") if i.attrs['id'] == '858'][-1]

open("tewi.xml", "w").write(str(soup))