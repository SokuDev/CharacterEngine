#!/bin/sh
export SHADY="$(pwd)/shady/shady-cli.exe"
file_ext="png,cv2 xml,pat csv,cv1 wav,cv3"
cmd='echo $base'
for f in $file_ext; do
	cmd="$cmd | sed 's/$(echo $f | cut -d ',' -f 1)/$(echo $f | cut -d ',' -f 2)/g'"
done
echo $cmd
rm -rf output
cp -r data prepare
for file in `find "$PWD/prepare" -name "*.png" -o -name "*.xml" -o -name "*wav" -o -name "*.csv"`; do
	#base="$(echo $file | sed 's/\/data\//\/output\//g')"
	#out="$(eval $cmd)"
	
	cd $(dirname $file)
	$SHADY convert $file
	rm $file
	cd - >/dev/null
done
mv prepare output