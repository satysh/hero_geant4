#!/bin/bash

if [[ -d archive_40 ]]; then
	echo "archive_40 is found!"
else
	mkdir archive_40
fi
while [[ true ]]; do
	if [[ $(ls tmp_output/ | wc -l) > 0 ]]; then
		mv -v tmp_output/* archive_40
	else
		echo "tmp_output is empty!" 	
	fi 
	sleep 1
done