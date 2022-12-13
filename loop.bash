#!/bin/bash

if [ $2 == compile ] 
then
    gcc $1.c -o $1.jack
fi

for (( c=0; c<$2;c++ ))
do
    echo "---------------------[Loop `expr $c + 1`]---------------------"
    ./$1.jack
done
