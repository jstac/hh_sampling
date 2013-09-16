/*

Perfect sampling from the stationary distribution of the Hopenhayn entry-exit
model of industry dynamimcs The algorithm used is Algorithm 1 in the paper.
Note, however, that in the computer code, negative time indexes on random
variables are replaced by positive ones for convenience.  Replacement is by
multiplying by -1.  For example, U[i] in the code should be interpreted as
U[-i] in the mathematical description found in the paper.


In the example treated below, the dynamics of incumbent firms is given by Y' =
Y * U where U is the shock.  See the function double g(double phi, double u).


John Stachurski and Takashi Kamihigashi, October 2012

*/


#include <stdio.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define MAX_DEPTH 5000

double x = 0.35;        // Exit threshold
double alpha1 = 5; double beta1 = 1;  // Shape parameters in beta distribution for incumbents
double alpha2 = 5; double beta2 = 1;  // Shape parameters in beta distribution for new firms


double g(double phi, double u) {
    // Law of motion for incumbents
    return phi * u;
}

double h(double phi, double z, double u) {
    // Law of motion
    return (phi >= x) * g(phi, u) + (phi < x) * z;
}

double iterate_h(int j, int k, double phi, double * Z, double * U) {
    // Compute h_j ... h_k(phi) given shock sequences Z and U
    int i = k;
    double r = phi;
    while (i >= j) {
        r = h(r, Z[i], U[i]);
        i = i - 1;
    }
    return r;
}

int get_sigma(int T, double * U) {
    /* 
     * Test whether the set Sigma_T is nonempty.  If it's nonempty, return
     * the positive integer sigma_T. If it's empty, return -1.
    */
    double r = 1.0;
    int k = 1;
    while (k < T) {
        r = g(r, U[T - k]);
        if (r < x) {
            return k;
        }
        k = k + 1;
    }
    return -1;
}

double compute_singleton(int sigma, int T, double * Z, double * U) {
    /*
     * Compute the single element of Lambda_T when it is a singleton, and
     * return a fail value otherwise.  The loop is terminated with a fail (and
     * returns -1) whenever the first distinct value is observed (and hence
     * Lambda_T is not a singleton).  Otherwise we return the singleton.  
     * After multiplying time scripts by -1, the set Lambda_T is given by
     *
     *   h[0] ... h[T-k-1] Z[T-k] for k = 1,...,sigma+1
     *
    */
    int k = sigma + 1;
    double s = iterate_h(0, T - k - 1, Z[T - k], Z, U);
    for (k=1; k <= sigma; k++) {
        double temp = iterate_h(0, T - k - 1, Z[T - k], Z, U); 
        if (temp != s) {
            return -1.0;
        }
    }
    return s;
}

double perfect(long int seed) {
    /* 
     * The perfect sampling algorithm.
     */
    int t = 100;            // starting value of t
    int increment = 50;     // increment t by this number if coalesence fails
    double Z[MAX_DEPTH+1];  // Grab memory for shocks
    double U[MAX_DEPTH+1];  // Ditto

    // Some boilerplate to set up the random number generator
    const gsl_rng_type * T;
    gsl_rng * rd;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    rd = gsl_rng_alloc (T);
    gsl_rng_set(rd, seed);

    // Draw the first set of random shocks for i = 0,...,t
    int i;
    for (i = 0; i <= t; i++) {
        U[i] = gsl_ran_beta(rd, alpha1, beta1);
        Z[i] = gsl_ran_beta(rd, alpha2, beta2);
    }

    while (t < MAX_DEPTH - 1) {

        int sigma = get_sigma(t, U);
        if (sigma > 0) { 
            double r = compute_singleton(sigma, t, Z, U);
            if (r >= 0) {
                gsl_rng_free(rd);
                return r;
            }
        }

        // Coalescence failed, so lets go round again
        for (i = t+1; i <= t+increment; i++) {
            U[i] = gsl_ran_beta(rd, alpha1, beta1);
            Z[i] = gsl_ran_beta(rd, alpha2, beta2);
        }
        t = t + increment;
    }

    // If we made it this far then we've hit MAX_DEPTH without success, so
    // return a warning and a fail value
    gsl_rng_free(rd);
    printf("Warning: MAX_DEPTH reached, returning fail value!\n");
    printf("If this happens repeatedly, you need to increase MAX_DEPTH.\n");
    return -1.0;
}


int main(void) {
    int num_draws = 200000;
    srand(time(0));
    int k = rand() % 1000000;  // Seed
    int i = 0;
    while (i < num_draws) {
        double p = perfect(k + i);
        printf("%g\n", p);
        i++;
    }
    return 0;
}
