#!/bin/bash
#set -x
count=`cat /proc/stat |grep 'cpu[0-9]'|wc -l`
array=(`cat /proc/stat |grep 'cpu[0-9]'|awk '{print $1,$2,$3,$4,$5}'`)
for((i=0;i<count;++i))
do
	echo ${array[5*i]}  0`echo "scale=4; (${array[5*i+1]} + ${array[5*i+2]} + ${array[5*i+3]})  / (${array[5*i+1]} + ${array[5*i+2]} + ${array[5*i+3]} + ${array[5*i+4]}) "  | bc `
done
