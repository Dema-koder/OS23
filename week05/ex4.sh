#!/bin/bash
rm ex4_res.txt
gcc ex4.c -o ex4 -pthread
{ time ./ex4 10000000 1; } 2>> ex4_res.txt
{ time ./ex4 10000000 2; } 2>> ex4_res.txt
{ time ./ex4 10000000 4; } 2>> ex4_res.txt
{ time ./ex4 10000000 10; } 2>> ex4_res.txt
{ time ./ex4 10000000 100; } 2>> ex4_res.txt
