#!/bin/sh
export SHADY="$(pwd)/shady/shady-cli"
rm -rf output
cp -r data output
for file in `find "$(pwd)/output" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv"`; do
	cd $(dirname $file)
	$SHADY convert $file
	rm $file
	cd - >/dev/null
done
