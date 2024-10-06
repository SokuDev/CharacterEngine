#!/bin/sh
export SHADY="$(pwd)/shady/shady-cli"
rm -rf output prepare
cp -r data prepare
for file in `find "$(pwd)/prepare" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv"`; do
	cd $(dirname $file)
	$SHADY convert $file
	rm $file
	cd - >/dev/null
done
find "$(pwd)/prepare" -name "*_labels.json" | while read x; do mv $x $(echo $x | sed 's/.json$/.txt/g'); done
find "$(pwd)/prepare" -name "*.py" -delete

mkdir pack
cp -r prepare pack/data
$SHADY pack -o tewi.dat -m data "$(pwd)/pack"
rm -rf pack
mv prepare output
