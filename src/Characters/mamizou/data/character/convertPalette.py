import sys
import struct

defaultMamizouPal    = [i & 0x7FFF for i in struct.unpack("<c256H", open(sys.argv[1], "rb").read())[1:]]
try:
	defaultChrPal= [i & 0x7FFF for i in struct.unpack("<c256H", open(sys.argv[2], "rb").read())[1:]]
except FileNotFoundError:
	defaultChrPal = [-1] * 256
defaultMamizouChrPal = [i & 0x7FFF for i in struct.unpack("<c256H", open(sys.argv[3], "rb").read())[1:]]
targetMamizouPal     = [i & 0x7FFF for i in struct.unpack("<c256H", open(sys.argv[4], "rb").read())[1:]]

indiceMap = {}
result = list(defaultMamizouChrPal)
for j, color in enumerate(defaultMamizouChrPal):
	try:
		indice = defaultMamizouPal.index(color)
		if color != defaultChrPal[j]:
			result[j] = targetMamizouPal[indice]
	except ValueError:
		pass


r = struct.pack("<c256H", b'\x10', *((k & 0x7FFF) if i == 0 else (k | 0x8000) for i, k in enumerate(result)))
open(sys.argv[5], "wb").write(r)
