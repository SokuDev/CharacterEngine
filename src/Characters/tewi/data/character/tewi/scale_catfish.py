import bs4

soup = bs4.BeautifulSoup(open("tewi.xml", "rb"), features="xml")
f = [i for i in soup.findAll("move") if i.attrs['id'] == '830']
last = f[0]

increaseScale = lambda: 200
increaseBox = lambda: 2

for f1 in f:
	f2 = bs4.BeautifulSoup(str(f1), features="xml")
	for box in f2.findAll("box"):
		box.attrs["down"]  = str(int(2 * int(box.attrs["down"])))
		box.attrs["up"]    = str(int(2 * int(box.attrs["up"])))
		box.attrs["left"]  = str(int(2 * int(box.attrs["left"])))
		box.attrs["right"] = str(int(2 * int(box.attrs["right"])))
	for blend in f2.findAll("blend"):
		blend.attrs["xscale"] = str(int(blend.attrs["xscale"]) * 2)
		blend.attrs["yscale"] = str(int(blend.attrs["yscale"]) * 2)
	last.insert_after(f2)
	last=[i for i in soup.findAll("move") if i.attrs['id'] == '830'][-1]
for i in f:
	i.decompose()

open("tewi.xml", "w").write(str(soup))
