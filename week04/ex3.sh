#!/bin/bash 
gcc ex3.c -o ex3
./ex3 3 &
last_program=$!
pstree -p $last_program
sleep 5
pstree -p $last_program
sleep 5
pstree -p $last_program
./ex3 5 &
last_program=$!
pstree -p $last_program
sleep 5
pstree -p $last_program
sleep 5
pstree -p $last_program
sleep 5
pstree -p $last_program
sleep 5
pstree -p $last_program
