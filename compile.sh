#!/bin/bash

OPT=$1

case $OPT in
  -d) file_to_compile="readCsv.cpp";;
  -f) file_to_compile=$2;;
esac

echo $file_to_compile

g++ "/home/knn/src/$file_to_compile" -fopenmp 

./a.out