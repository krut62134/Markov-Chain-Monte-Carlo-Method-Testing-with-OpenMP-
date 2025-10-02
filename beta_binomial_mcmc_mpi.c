#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int k = 7, n = 10;
double alpha = 2.0, beta_param = 2.0;

double log_posterior(double theta) {
    return k*log(theta) + (n-k)*log(1-theta) + (alpha-1)*log(theta) + (beta_param-1)*log(1-theta);
}

void mcmc_sample(double *samples, int n_samples, double proposal_sd, unsigned int seed) {
    double theta = 0.3 + 0.4 * ((double)rand_r(&seed) / RAND_MAX);
    
    for (int i = 0; i < n_samples; i++) {
        double theta_prop = theta + proposal_sd * (((double)rand_r(&seed)/RAND_MAX) - 0.5) * 2;
        
        if (theta_prop < 0 || theta_prop > 1) {
            samples[i] = theta;
            continue;
        }
        
        double log_ratio = log_posterior(theta_prop) - log_posterior(theta);
        if (log((double)rand_r(&seed)/RAND_MAX) < log_ratio) {
            theta = theta_prop;
        }
        samples[i] = theta;
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc != 2) {
        if (rank == 0) printf("Usage: ibrun -n <procs> %s <samples>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    
    int n_samples = atoi(argv[1]);
    
    // Each MPI process runs 1 chain
    double *samples = malloc(n_samples * sizeof(double));
    
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    
    unsigned int seed = rank * 12345 + time(NULL);
    mcmc_sample(samples, n_samples, 0.9, seed);
    
    MPI_Barrier(MPI_COMM_WORLD);
    double local_time = MPI_Wtime() - start;
    
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("%d %d %.3f\n", size, size, max_time);
    }
    
    free(samples);
    MPI_Finalize();
    return 0;
}
