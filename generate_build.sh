#!/bin/sh

if [ -z $WINDIR ]; then
	if ! type shady-cli; then
		export PATH="$(dirname $0)/shady/:$PATH"
	fi
	type shady-cli || exit
	OPTIONS=-j
else
	if ! type shady-cli.exe; then
		export PATH="$(realpath $(dirname $0))/shady/:$PATH"
	fi
	type shady-cli.exe || exit
	alias "shady-cli=shady-cli.exe"
	OPTIONS=
fi

if [ $# -lt 1 ]; then
	echo "Usage: $0 <cmake_build_folder>"
	exit 1
fi

if ! type jq; then
	function jq() {
		grep "$(echo $1 | cut -d . -f 2-)" | cut -f 2 -d : | sed -r 's/^\s*([^,\s]*)\s*,?\s*$/\1/g'
	}
fi

OUTPUT="$(realpath $1)"
cd "$(dirname $0)"

cmake --build $OUTPUT --target CharacterEngine -- $OPTIONS || exit
cmake --build $OUTPUT --target Soku2Loader -- $OPTIONS || exit

rm -f "$OUTPUT/Soku2Addon.zip"
rm -f "$OUTPUT/CharacterEngine.zip"
rm -rf "$OUTPUT/Soku2_package" "$OUTPUT/standalone"
mkdir "$OUTPUT/Soku2_package" "$OUTPUT/standalone"

cp "$OUTPUT/CharacterEngine.dll" "$OUTPUT/Soku2_package/$character"
cp "$OUTPUT/Soku2Loader.dll" "$OUTPUT/Soku2_package/Soku2.dll"
cp "src/Soku2Loader/Soku2Loader.ini" "$OUTPUT/Soku2_package/"
mkdir -p "$OUTPUT/Soku2_package/config/info"
cp "lib/Soku2Engine.dll" "$OUTPUT/Soku2_package/"
cp "lib/MemoryPatch.dll" "$OUTPUT/Soku2_package/"
cp "lib/MemoryPatch.ini" "$OUTPUT/Soku2_package/"
cp "lib/SOKU2_base.lua" "$OUTPUT/Soku2_package/config/"
cp "lib/characters_base.csv" "$OUTPUT/Soku2_package/config/info/"

cp "$OUTPUT/CharacterEngine.dll" "$OUTPUT/standalone/Soku2.dll"
mkdir -p "$OUTPUT/standalone/config/info"
echo 'set_version("3.0")' > "$OUTPUT/standalone/config/SOKU2_base.lua"
echo > "$OUTPUT/standalone/config/info/characters_base.csv"

for character in `ls src/Characters/`; do
	echo "Checking $character"
	OUT="$OUTPUT/$character"
	IN="src/Characters/$character"
	if [ $(cat "$IN/character.json" | jq .enabled) '==' false ]; then
		echo "Disabled"
	else
		FILES=""
		mkdir -p "$OUT/data"
		if ! [ -z "$2" ]; then
			echo 'Not checking assets'
		elif [ -f "$OUT/$character.dat" ]; then
			echo "Checking diff"
			DATE1=$(stat -c '%Y' "$OUT/$character.dat")
			IFS="
"
			for file in `find "$IN/data" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv" -o -name "*.pal" -o -name "${character}_labels.json"`; do
				DATE2=$(stat -c '%Y' "$file")
				RESULT="$OUT/data/$(echo $file | tail -c +$(echo "$IN/data/" | wc -c) | sed 's/.json$/.txt/g')"
				if [ $DATE1 -lt $DATE2 ] || ! [ -f $(echo $RESULT | sed 's/\.xml$/\.pat/g' | sed 's/\.csv$/\.cv1/g' | sed 's/\.png$/\.cv2/g' | sed 's/\.wav$/\.cv3/g') ]; then
					mkdir -p $(dirname "$RESULT")
					cp -f "$file" "$RESULT"
					FILES="yes"
				fi
			done
		else
			echo "Generating structure"
			IFS="
"
			for file in `find "$IN/data" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv" -o -name "*.pal" -o -name "${character}_labels.json"`; do
				RESULT="$OUT/data/$(echo $file | tail -c +$(echo "$IN/data/" | wc -c) | sed 's/.json$/.txt/g')"
				mkdir -p $(dirname "$RESULT")
				cp -f "$file" $RESULT
				FILES="yes"
			done
		fi
		if [ -z $FILES ]; then
			echo "Assets are clean"
		else
			for file in `find "$OUT/data" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv"`; do
				cd "$(dirname $file)"
				shady-cli convert $file | grep --color=never Converting
				rm $file
				cd - >/dev/null
			done
			rm -f "$OUTPUT/$character/$character.dat"
			shady-cli pack -o "$OUT/$character.dat" -m data "$OUT"
		fi
		DLL="$(cat "$IN/character.json" | jq .character_dll | sed -r 's/^"(.*)"$/\1/g')"
		echo "Building $DLL"
		if cmake --build $OUTPUT --target $(echo $DLL | sed -r 's/^(.*)\..*$/\1/g') -- $OPTIONS; then
			mkdir -p "$OUTPUT/Soku2_package/characters/$character" "$OUTPUT/standalone/characters/$character"
			cp "$OUTPUT/src/Characters/$character/$DLL" "$OUT/$character.dat" "$IN/character.json" "$OUTPUT/standalone/characters/$character"
			cp "$OUTPUT/src/Characters/$character/$DLL" "$OUT/$character.dat" "$IN/character.json" "$OUTPUT/Soku2_package/characters/$character"
			echo ";#" > "$OUTPUT/Soku2_package/characters/$character/dummy.asm"
			echo "return function() end" > "$OUTPUT/Soku2_package/characters/$character/dummy.lua"
			cat "$IN/data/csv/$character/deck.csv" "$IN/data/csv/$character/deck.csv" "$IN/data/csv/$character/deck.csv" "$IN/data/csv/$character/deck.csv" > "$OUTPUT/Soku2_package/characters/$character/deck.cfg"
		fi
	fi
done
cd "$OUTPUT/Soku2_package"
echo "Generating Soku2Addon.zip"
zip ../Soku2Addon.zip -r .
cd "$OUTPUT/standalone"
echo "Generating CharacterEngine.zip"
zip ../CharacterEngine.zip -r .
