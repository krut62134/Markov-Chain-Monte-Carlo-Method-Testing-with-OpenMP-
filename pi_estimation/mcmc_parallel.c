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
    unsigned int seed = (unsigned int)time(NULL) + omp_get_thread_num();

    #pragma omp parallel for reduction(+:inside_circle_count) private(seed)
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
    if (argc != 3) {
        printf("Usage: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    double start_total_time = omp_get_wtime(); // Start timing for the entire program

    long long int N = strtoull(argv[1], NULL, 10);
    if (N <= 0) {
        printf("Please enter a valid positive integer for the value of N.\n");
        return 1;
    }

    int num_threads = atoi(argv[2]);
    if (num_threads <= 0) {
        printf("Please enter a valid positive integer for the number of threads.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Print the number of threads
//    #pragma omp parallel
//    {
//        #pragma omp single
//       printf("Number of threads: %d\n", omp_get_num_threads());
//    }

    double pi_estimate = estimate_pi_mcmc(N);

//    printf("Estimated value of pi using 2D MCMC: %.20f\n", pi_estimate);

    // Known value of pi up to 20 decimal places
    const double known_pi = 3.14159265358979323846;

    // Calculate and print the error estimate
    double error_estimate = fabs(known_pi - pi_estimate);
//    printf("Error estimate: %.20f\n", error_estimate);

    double end_total_time = omp_get_wtime(); // End timing for the entire program

    // Print the total time
//    printf("Total time taken: %f seconds\n", end_total_time - start_total_time);
    
    printf("%d %lli %.20f %.20f %f\n", num_threads, N, pi_estimate,  error_estimate, end_total_time - start_total_time);

    return 0;
}

