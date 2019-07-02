#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    float LAMBDA_INVERSE = 1.0/75;

    float mean = 0;
    float items[1000];
    // Compute mean and add values into the array
    for(int i = 0; i < 1000; i++) {
        float random = (float)rand() / (float)RAND_MAX;
        float exponential_random = -1 * LAMBDA_INVERSE * log(1 - random);
        items[i] = exponential_random;
        mean = mean + exponential_random;
    }

    mean = mean / 1000;

    // Compute variance
    float variance = 0;
    for(int i = 0; i < 1000; i++) {
    	variance = variance + ( (items[i] - mean) * (items[i] - mean) );
    }
    variance = variance / 1000;

    // Print out results
    printf("lamdba: %f\n", 1/LAMBDA_INVERSE);
    printf("mean: %f\n", mean);
    printf("variance: %f\n", variance);

    return 0;
}