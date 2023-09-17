#include <limits.h>
#include <float.h>
#include <stdio.h>

int main() {
    int a = INT_MAX;
    unsigned short int b = USHRT_MAX;
    signed long int c = LONG_MAX;
    float d = FLT_MAX;
    double e = DBL_MAX;
    
    printf("%d %d\n", sizeof(a), a); 
    printf("%d %d\n", sizeof(b), b); 
    printf("%d %ld\n", sizeof(c), c); 
    printf("%d %f\n", sizeof(d), d); 
    printf("%d %lf\n", sizeof(e), e); 
}
