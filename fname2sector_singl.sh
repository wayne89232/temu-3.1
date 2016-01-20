#!/bin/bash
# $1 = argv -> filename
 
entry=$(ifind -n "$1" ../../win7_test.img -o 206848)
 
info=$(istat -o 206848 ../../win7_test.img $entry)
 
cluster=$(sed -n '/init_size/{n;p}' <<<"$info")
 
clusters=($cluster)
 
if [ "${clusters[0]}" == "" ]
then
    echo "-1000"
else
    let sector="${clusters[0]}*8+206848"
    echo $sector
fi