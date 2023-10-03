#!/bin/bash 
gcc ex3.c -o ex3
./ex3 3 &
sleep 1
pstree -p > ex3_1.txt
./ex3 5 &
sleep 1
pstree -p > ex3_2.txt
