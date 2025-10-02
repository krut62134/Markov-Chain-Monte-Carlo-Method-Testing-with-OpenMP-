#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int k = 7, n = 10;
double alpha = 2.0, beta_param = 2.0;

double log_posterior(double theta) {
    double log_likelihood = k * log(theta) + (n - k) * log(1 - theta);
    double log_prior = (alpha - 1) * log(theta) + (beta_param - 1) * log(1 - theta);
    return log_likelihood + log_prior;
}

void mcmc_sample(double *samples, int n_samples, double proposal_sd, unsigned int seed) {
    double theta = 0.3 + 0.4 * ((double)rand_r(&seed) / RAND_MAX); // random start
    
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
    if (argc != 3) {
        printf("Usage: %s <n_chains> <n_samples_per_chain>\n", argv[0]);
        return 1;
    }
    
    int n_chains = atoi(argv[1]);
    int n_samples = atoi(argv[2]);
    
    double **all_samples = malloc(n_chains * sizeof(double*));
    for (int c = 0; c < n_chains; c++) {
        all_samples[c] = malloc(n_samples * sizeof(double));
    }
    
    double start = omp_get_wtime();
    
    #pragma omp parallel for
    for (int chain = 0; chain < n_chains; chain++) {
        unsigned int seed = time(NULL) + chain * 1000 + omp_get_thread_num();
        mcmc_sample(all_samples[chain], n_samples, 0.9, seed);
    }
    
    double end = omp_get_wtime();
    printf("Time: %.3f seconds\n", end - start);
    
    // Save all chains
    FILE *f = fopen("chains.dat", "w");
    for (int i = 0; i < n_samples; i++) {
        for (int c = 0; c < n_chains; c++) {
            fprintf(f, "%.6f ", all_samples[c][i]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    
    for (int c = 0; c < n_chains; c++) free(all_samples[c]);
    free(all_samples);
    
    return 0;
}
