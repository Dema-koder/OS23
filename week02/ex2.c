#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>

int main() {
    char s[256];
    while (1) {
    	char x;
    	x = getchar();
    	if (x == '.' || x == '\n')
    	    break;
    	else
    	    strncat(s, &x, 1);
    }
    char t[260];
    t[0] = '"';
    for (int i = 0; i < strlen(s); i++)
        t[i + 1] = s[strlen(s) - i - 1];
    t[strlen(s)+1] = '"';
    printf("%s", t);
}
