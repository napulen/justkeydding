#!/bin/bash

# arg1 is the dataset folder

i=0

for f in $(ls $1/features)
do
	id=$(printf %04d $i)
	filename=$(basename -- "$f")
	ext="${filename##*.}"
	noext="${filename%.*}"
	
	oldfile="$1/features/$noext.$ext"
	newfile="$1/features/$id.$ext"
	cp $oldfile $newfile
	oldannotation="$1/annotations/$noext.key"
	newannotation="$1/annotations/$id.key"
	cp $oldannotation $newannotation
	rm $oldfile
	rm $oldannotation
	((i++))
done
	
