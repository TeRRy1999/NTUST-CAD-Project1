#!/bin/bash
dir=./comb/

for file in $(ls $dir); do   
    ./a.out ./comb/$file
    echo /comb/$file sucess!
done