#!/bin/bash

make freq_gen_clean
make freq_gen_install
if [ "$#" -eq 2 ]
then
	time ./freq_gen $1 $2
elif [ "$#" -eq 1 ]
then
	time ./freq_gen $1
else
	echo "Usage : sh frun.sh PATH_TO_CORPUS [TARGET_FOLDER]"
fi
