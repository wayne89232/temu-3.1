#!/bin/bash
# $1 = argv -> filename

entry=$(ifind -n "$1" ../../../win7_test.img -o 206848)
#fixed image: win7_test.img 
info=$(istat -o 206848 ../../../win7_test.img $entry)

cluster=$(sed -n '/init_size/{n;p}' <<<"$info")

clusters=($cluster)
#take the first cluster

#resident file -> give it a -1000 for later judgement
#non resident file -> echo its sector number (the offset for the file system in the image fixed)
if [ "${clusters[0]}" == "" ]
then
    echo "-1000"
else
    let sector="${clusters[0]}*8+206848"
    echo $sector
fi

