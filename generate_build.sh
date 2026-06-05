#!/bin/bash

if [ -z $WINDIR ]; then
	if ! type shady-cli; then
		export PATH="$(dirname $0)/shady/:$PATH"
	fi
	type shady-cli || exit
	OPTIONS="-j $(nproc)"
else
	if ! type shady-cli.exe; then
		export PATH="$(realpath $(dirname $0))/shady/:$PATH"
	fi
	type shady-cli.exe || exit
	alias "shady-cli=shady-cli.exe"
	OPTIONS=
fi

if [ $# -lt 1 ]; then
	echo "Usage: $0 <cmake_build_folder> [<character>]"
	exit 1
fi

if ! type jq; then
	function jq() {
		grep "$(echo $1 | cut -d . -f 2-)" | cut -f 2 -d : | sed -r 's/^\s*([^,\s]*)\s*,?\s*$/\1/g'
	}
fi

OUTPUT="$(realpath $1)"
cd "$(dirname $0)"

generate_character() {
	echo "Checking $1"
	OUT="$OUTPUT/$1"
	IN="src/Characters/$1"
	if [ $(cat "$IN/character.json" | jq .enabled) '==' false ]; then
		echo "Disabled"
	else
		DLL="$(cat "$IN/character.json" | jq -r .character_dll)"
		DAT="$(cat "$IN/character.json" | jq -r .data)"
		if [ -z $NO_BUILD ]; then
			echo "Building $DLL"
			cmake --build "$OUTPUT" --target $(echo $DLL | sed -r 's/^(.*)\..*$/\1/g') -- $OPTIONS || exit
		fi
		mkdir -p "$OUTPUT/Soku2_full/characters/$1" "$OUTPUT/standalone/characters/$1"
		PDB="$(echo $DLL | rev | cut -d '.' -f 2- | rev).pdb"
		cp "$OUTPUT/src/Characters/$1/$DLL" "$OUTPUT/src/Characters/$1/$PDB" "$OUTPUT/src/Characters/$1/$DAT" "$IN/character.json" "$OUTPUT/standalone/characters/$1"
		cp "$OUTPUT/src/Characters/$1/$DLL" "$OUTPUT/src/Characters/$1/$PDB" "$OUTPUT/src/Characters/$1/$DAT" "$IN/character.json" "$OUTPUT/Soku2_full/characters/$1"
		echo ";#" > "$OUTPUT/Soku2_full/characters/$1/dummy.asm"
		echo "return function() end" > "$OUTPUT/Soku2_full/characters/$1/dummy.lua"
		cat "$IN/data/csv/$1/deck.csv" "$IN/data/csv/$1/deck.csv" "$IN/data/csv/$1/deck.csv" "$IN/data/csv/$1/deck.csv" > "$OUTPUT/Soku2_full/characters/$1/deck.cfg"
	fi
}

if [ $# -eq 1 ]; then
	cmake --build "$OUTPUT" --target All -- $OPTIONS || exit

	rm -f "$OUTPUT/Soku2Full.zip" "$OUTPUT/CharacterEngine.zip"
	rm -rf "$OUTPUT/Soku2_full" "$OUTPUT/standalone"
	mkdir "$OUTPUT/standalone"

	cp -r "lib/Soku2" "$OUTPUT/Soku2_full"
	cp "$OUTPUT/CharacterEngine.dll" "$OUTPUT/Soku2_full/CharacterEngine.dat"
	cp "$OUTPUT/Soku2Loader.dll" "$OUTPUT/Soku2_full/Soku2.dll"
	cp "src/Soku2Loader/Soku2Loader.ini" "$OUTPUT/Soku2_full/"
	mkdir -p "$OUTPUT/Soku2_full/characters/common"
	cp "$OUTPUT/common.dat" "$OUTPUT/Soku2_full/characters/common"

	cp "$OUTPUT/CharacterEngine.dll" "$OUTPUT/standalone/Soku2.dll"
	mkdir -p "$OUTPUT/standalone/config/info"
	echo 'set_version("3.0")' > "$OUTPUT/standalone/config/SOKU2_base.lua"
	echo > "$OUTPUT/standalone/config/info/characters_base.csv"

	export NO_BUILD=1
	for character in `ls src/Characters/`; do
		generate_character "$character"
	done
	if [ -z $NO_ZIP ]; then
		cd "$OUTPUT/Soku2_full"
		echo "Generating Soku2Full.zip"
		zip ../Soku2Full.zip -r .
		cd "$OUTPUT/standalone"
		echo "Generating CharacterEngine.zip"
		zip ../CharacterEngine.zip -r .
	fi
else
	while [ $# -ge 2 ]; do
		generate_character "$2"
		shift
	done
fi
