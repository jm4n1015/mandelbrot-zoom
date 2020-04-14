#include "mandelbrot.h"

#include <stdlib.h>
#include <stdio.h>

// The initial precision value
#define PREC_INIT   128

// The round mode to use
#define RND_MODE    MPC_RNDNN

// Constant at which the mandelbrot set escapes
#define BOUND       2.0

// Compute the value of the mandelbrot set at one point
static unsigned int compute_value(struct mandelbrot *m);

struct mandelbrot mandelbrot_init(unsigned int width, unsigned int height, unsigned int iterations) {
    struct mandelbrot m;

    m.width = width;
    m.height = height;
    m.iterations = iterations;

    m.image = malloc(m.width * m.height * sizeof(*m.image));

    m.prec = PREC_INIT;
    m.rnd = RND_MODE;

    // Initialize mpc_t variables, precision is doubled whenever necessary to remain accurate
    mpc_init2(m.C, m.prec);
    mpc_init2(m.Z, m.prec);

    mpfr_init2(m.mpfr_tmp1, m.prec);
    mpfr_init2(m.mpfr_tmp2, m.prec);

    return m;
}

void mandelbrot_clear(struct mandelbrot *m) {
    free(m->image);

    mpc_clear(m->C);
    mpc_clear(m->Z);

    mpfr_clear(m->mpfr_tmp1);
    mpfr_clear(m->mpfr_tmp2);
}

unsigned int *mandelbrot_image(struct mandelbrot *m, const mpc_t *min, const mpc_t *max) {
    for (unsigned int i = 0; i < m->height; ++i) {
        printf("%u\n", i);
        for (unsigned int x = 0; x < m->width; ++x) {
            // Calculate the value of C
            // TODO: this can be optimized

            // Calculate the imaginary offset; Do this first so the mpfr variable
            // can be easily converted back to being imaginary
            mpc_imag(m->mpfr_tmp1, *min, m->rnd);
            mpc_imag(m->mpfr_tmp2, *max, m->rnd);

            mpfr_sub(m->mpfr_tmp1, m->mpfr_tmp2, m->mpfr_tmp1, m->rnd);
            mpfr_div_ui(m->mpfr_tmp1, m->mpfr_tmp1, m->height, m->rnd);
            mpfr_mul_ui(m->mpfr_tmp1, m->mpfr_tmp1, i, m->rnd);

            mpc_set_fr(m->C, m->mpfr_tmp1, m->rnd);
            mpc_mul_i(m->C, m->C, 1, m->rnd);

            // Calculate the real offset
            mpc_real(m->mpfr_tmp1, *min, m->rnd);
            mpc_real(m->mpfr_tmp2, *max, m->rnd);

            mpfr_sub(m->mpfr_tmp1, m->mpfr_tmp2, m->mpfr_tmp1, m->rnd);
            mpfr_div_ui(m->mpfr_tmp1, m->mpfr_tmp1, m->width, m->rnd);
            mpfr_mul_ui(m->mpfr_tmp1, m->mpfr_tmp1, x, m->rnd);
            
            mpc_add_fr(m->C, m->C, m->mpfr_tmp1, m->rnd);

            // Add minimum value to C
            mpc_add(m->C, m->C, *min, m->rnd);

            // Set the pixel in the image to the computed value
            m->image[i * m->width + x] = compute_value(m);
        }
    }

    return m->image;
}

static unsigned int compute_value(struct mandelbrot *m) {
    mpc_set(m->Z, m->C, m->rnd);

    unsigned int i;

    for (i = 0; i < m->iterations; ++i) {
        mpc_abs(m->mpfr_tmp1, m->Z, m->rnd);

        if (mpfr_cmp_d(m->mpfr_tmp1, BOUND) > 0) {
            break;
        }

        mpc_mul(m->Z, m->Z, m->Z, m->rnd);
        mpc_add(m->Z, m->Z, m->C, m->rnd);
    }

    return i;
}
