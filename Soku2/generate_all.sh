#!/bin/sh
if [ $# -lt 2 ]; then
	exit 1
fi
major=$1
minor=$2
shift 2
exe=~/projects/Shared/hisoutensoku/th123.exe
csv=~/Soku2_stuff/characters.csv
for pair in clownpiece:udonge flandre:remilia kaguya:patchouli mima:iku mokou:aya momiji:reimu murasa:suika orin:chirno ran:sakuya satori:yuyuko sekibanki:youmu shou:tenshi yuuka:utsuho; do
	chr="$(echo $pair | cut -d : -f 1)"
	base="$(echo $pair | cut -d : -f 2)"
	echo $pair
	python3 soku2assembler.py --soku_exe $exe --csv $csv --major $major --minor $minor --base $base --class_name $chr $@ ~/Soku2_stuff/$chr/$chr.asm ../src/Characters/$chr
done
