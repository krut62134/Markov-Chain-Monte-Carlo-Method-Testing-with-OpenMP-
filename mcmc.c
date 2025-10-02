#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int is_inside_circle(double x, double y) {
    return x * x + y * y <= 1.0;
}

double estimate_pi_mcmc(long long int N) {
    long long int inside_circle_count = 0;

    // Use a different seed for each thread
    unsigned int seed = (unsigned int)time(NULL);

    for (long long int i = 0; i < N; ++i) {
        // Generate random points in the unit square
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        // Check if the point is inside the unit circle
        if (is_inside_circle(x, y)) {
            inside_circle_count++;
        }
    }

    // Estimated value of pi based on the ratio of points inside the circle
    return (double)inside_circle_count / N * 4.0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    long long int N = atoi(argv[1]);
    if (N <= 0) {
        printf("Please enter a valid positive integer for the value of N.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    double pi_estimate = estimate_pi_mcmc(N);
    printf("Estimated value of pi using 2D MCMC: %.10f\n", pi_estimate);

    // Known value of pi up to 10 decimal places
    const double known_pi = 3.1415926535;

    // Calculate and print the error estimate
    double error_estimate = fabs(known_pi - pi_estimate);
    printf("Error estimate: %.10f\n", error_estimate);

    return 0;
}

