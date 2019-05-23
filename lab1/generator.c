#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    float LAMBDA_INVERSE = 1.0/75;

    for(int i = 0; i < 1000; i++) {
        float random = (float)rand() / (float)RAND_MAX;
        float exponential_random = -1 * LAMBDA_INVERSE * log(1 - random);
        printf("%f\n", exponential_random);
    }
    return 0;
}