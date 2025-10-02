#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

int k = 7, n = 10;
double alpha = 2.0, beta_param = 2.0;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

double log_posterior(double theta) {
    double log_likelihood = k * log(theta) + (n - k) * log(1 - theta);
    double log_prior = (alpha - 1) * log(theta) + (beta_param - 1) * log(1 - theta);
    return log_likelihood + log_prior;
}

void mcmc_sample(double *samples, int n_samples, double proposal_sd) {
    double theta = 0.5;
    int accepted = 0;
    
    for (int i = 0; i < n_samples; i++) {
        double theta_prop = theta + proposal_sd * ((double)rand()/RAND_MAX - 0.5) * 2;
        
        if (theta_prop < 0 || theta_prop > 1) {
            samples[i] = theta;
            continue;
        }
        
        double log_ratio = log_posterior(theta_prop) - log_posterior(theta);
        if (log((double)rand()/RAND_MAX) < log_ratio) {
            theta = theta_prop;
            accepted++;
        }
        
        samples[i] = theta;
    }
    
    printf("Acceptance rate: %.3f\n", (double)accepted / n_samples);
}

int main() {
    srand(time(NULL));
    
    int n_samples = 10000000;
    double *samples = malloc(n_samples * sizeof(double));
    
    double start = get_time();
    mcmc_sample(samples, n_samples, 0.9);
    double end = get_time();
    printf("Time: %.3f seconds\n", end - start);

    // Save to file
    FILE *f = fopen("samples.dat", "w");
    for (int i = 0; i < n_samples; i++) {
        fprintf(f, "%.6f\n", samples[i]);
    }
    fclose(f);

    free(samples);
    return 0;
}
