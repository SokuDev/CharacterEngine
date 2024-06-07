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
mv prepare output
