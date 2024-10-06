#!/bin/sh

if ! type shady-cli; then
	export PATH="$(PWD)/shady/:$PATH"
fi

if [ $# -lt 1 ]; do
	echo "Usage: $0 <cmake_build_folder>"
	exit 1
fi

cd "$(dirname $0)"
make -C $1 CharacterEngine
for character in `ls src/Characters/`; do
	OUT="$1/$character"
	IN="src/Characters/$character"
	mkdir -p "$OUT/data"
	if [ -f "$OUT/$character.dat" ]; then
		for file in `find "$OUT/data" -newer "$OUT/$character.dat" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv" -o -name "${character}_labels.json"`; do
			mkdir -p $(dirname "$file")
			cp -f $file "$OUT/data/$(echo $file | sed 's/.json$/.txt/g')"
		done
	else
		for file in `find "$OUT/data" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv" -o -name "${character}_labels.json"`; do
			mkdir -p $(dirname "$file")
			cp -f $file "$OUT/data/$(echo $file | sed 's/.json$/.txt/g')"
		done
	fi
	for file in `find "$OUT/data" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv"`; do
		cd "$(dirname $file)"
		shady-cli convert $file
		rm $file
		cd - >/dev/null
	done
	rm -f "$1/$character/$character.dat"
	shady-cli pack -o "$1/$character/$character.dat" -m data "$1/$character"
fi
