#include <limits.h>
#include <float.h>
#include <stdio.h>

int main() {
    int a = INT_MAX;
    unsigned short int b = USHRT_MAX;
    signed long int c = LONG_MAX;
    float d = FLT_MAX;
    double e = DBL_MAX;
    
    printf("%ld %d\n", sizeof(a), a); 
    printf("%ld %d\n", sizeof(b), b); 
    printf("%ld %ld\n", sizeof(c), c); 
    printf("%ld %f\n", sizeof(d), d); 
    printf("%ld %lf\n", sizeof(e), e); 
}
