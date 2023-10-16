gcc scheduler.c -o scheduler
gcc worker.c -o worker
echo "idx at bt
0   5   2
1            7          6
2  20               3
3          3              8
4 2                    4
5    3                               1
6      10                   6" > data.txt
./scheduler data.txt
