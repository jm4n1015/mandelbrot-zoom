#ifndef IMAGE_H
#define IMAGE_H

#include "mandelbrot.h"

#include <stdio.h>

// Generate one image of the mandelbrot set
void output_image(FILE *stream, struct mandelbrot *m, const mpc_t *min, const mpc_t *max);

// Output a stream of images that are zooming in.
// For now, the zoom rate is set at (1 / width)
void output_stream(FILE *stream, const mpc_t *center, unsigned int max_zoom,
                   unsigned int width, unsigned int height, unsigned int iterations);

#endif
