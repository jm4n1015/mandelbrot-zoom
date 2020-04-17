#include <stdio.h>
#include "image.h"
#include "mandelbrot.h"

#define WIDTH       1366
#define HEIGHT      768
#define ITERATIONS  100
#define FRAMES      200

/*
 * NOTES:
 * It has been verified that the zooming functionality works within
 * mandelbrot.c, so any problems are occurring due to code in image.c
 *
 * TODO: Make precision update in preparation for zooming
 * TODO: Write the zoom code
 */

int main(void) {
    mpc_t center;

    // TODO: Fix the precision later
    mpc_init2(center, 128);
    mpc_set_d_d(center, -0.75, 0, MPC_RNDNN);

    output_stream(stdout, &center, FRAMES, WIDTH, HEIGHT, ITERATIONS);

    mpc_clear(center);

    return 0;
}
