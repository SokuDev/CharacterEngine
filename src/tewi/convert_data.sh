#!/bin/sh
export SHADY="wine $(pwd)/shady/shady-cli.exe"
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
LINK=$($SHADY pack -o tewi.dat -m data "$(pwd)/pack" 2>&1 | while read x; do if echo $x | grep "cannot rename" >/dev/null; then echo $x | cut -d '[' -f 2,3; fi; done)
if ! [ -z "$LINK" ]; then
	mv `echo $LINK | cut -f 1 -d ']'` `echo $LINK | cut -f 2 -d '[' | cut -f 1 -d ']'`
fi
rm -rf pack
mv prepare output
