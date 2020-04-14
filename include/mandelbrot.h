#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <mpc.h>

struct mandelbrot {
    // Array of values for how long it takes to escape
    unsigned int *image;

    // mpc_t variables are declared here so they only have to be initialized once
    mpc_t C, Z;

    mpfr_t mpfr_tmp1, mpfr_tmp2;

    // Precision of mpc_t variables, this is doubled whenever necessary
    mpfr_prec_t prec;

    // Rounding mode
    mpc_rnd_t rnd;

    // Dimensions of the image
    unsigned int width, height;

    // The maximum number of iterations
    unsigned int iterations;
};

// Initialize a mandelbrot struct
struct mandelbrot mandelbrot_init(unsigned int width, unsigned int height, unsigned int iterations);

// Clear resources allocated by mandelbrot struct
void mandelbrot_clear(struct mandelbrot *m);

// Generate an image
unsigned int *mandelbrot_image(struct mandelbrot *m, const mpc_t *min, const mpc_t *max);

#endif
