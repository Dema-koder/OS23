#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void convert(char num[300], int x, int y) {
    long long num10 = 0;
    long long c = 1;
    for (int i = strlen(num) - 1; i>=0; i--) {
        if (num[i] - '0' >= x) {
            printf("cannot convert!");
            return;
        } 
        num10 += c * (num[i] - '0');
        c *= x;
    }
    char ans[300];
    int ind = 0;
    while (num10 > 0) {
        ans[ind] = num10 % y + '0';
        num10 /= y;
        ind++;
    }
    for (int i = strlen(ans) - 1; i>=0; i--)
        printf("%c", ans[i]);
}

int main() {
    char num[300];
    int x, y;
    scanf("%s %d %d", num, &x, &y); 
    convert(num, x, y);
}
