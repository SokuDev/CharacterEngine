#!/bin/bash

if [ $# -lt 3 ]; then
	echo "Usage: $0 <cmake build folder> <Soku2 out> <shady-loader path>"
	exit 1
fi

CMAKE_OUT="$(realpath "$1")"
SOKU2_OUT="$(realpath "$2")"
SHADY_OUT="$(realpath "$3")"
cd "$(dirname $0)"

setup_dev_character() {
	echo "Generating $1"
	OUT="$OUTPUT/$1"
	IN="src/Characters/$1"
	if [ $(cat "$IN/character.json" | jq .enabled) '==' false ]; then
		echo "Disabled"
	else
		IN="src/Characters/$1"
		DLL="$(cat "$IN/character.json" | jq -r .character_dll)"
		DAT="$(cat "$IN/character.json" | jq -r .data)"
		PDB="$(echo "$DLL" | sed 's/\.dll/.pdb/g')"
		rm "$SOKU2_OUT/characters/$1/$DLL" || exit
		ln -s "$(realpath $CMAKE_OUT/$IN/$DLL)" "$SOKU2_OUT/characters/$1/$DLL" || exit
		if [ -f "$SOKU2_OUT/characters/$1/$PDB" ]; then
			rm "$SOKU2_OUT/characters/$1/$PDB" || exit
			ln -s "$(realpath $CMAKE_OUT/$IN/$PDB)" "$SOKU2_OUT/characters/$1/$PDB" || exit
		fi
		rm -f "$SHADY_OUT/$1" || exit
		ln -s "$(realpath $CMAKE_OUT/$IN/${1}_assets)" "$SHADY_OUT/$1" || exit
		CONFIG="$CONFIG
$1=1"
	fi
}

export NO_ZIP=1
./generate_build.sh "$CMAKE_OUT" || exit
rm -rf "$SOKU2_OUT" || exit
cp -r "$CMAKE_OUT/Soku2_full" "$SOKU2_OUT" || exit
CONFIG="[Options]
autoUpdate=0
useLoadLock=0
enableLua=1
[Packages]"
for character in `ls src/Characters/`; do
	setup_dev_character "$character"
done
rm "$SOKU2_OUT"/CharacterEngine.dat
ln -s "$CMAKE_OUT"/CharacterEngine.dll "$SOKU2_OUT"/CharacterEngine.dat
printf "%s\n" "$CONFIG" > "$SHADY_OUT/shady-loader.ini"