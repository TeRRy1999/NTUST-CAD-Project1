#!/bin/bash
dir=./comb/

for file in $(ls $dir); do
    echo /comb/$file sucess!
    ./a.out ./comb/$file
done