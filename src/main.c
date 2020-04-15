#include <stdio.h>
#include "image.h"
#include "mandelbrot.h"

#define WIDTH       750
#define HEIGHT      500
#define ITERATIONS  50

/*
 * TODO: Make precision update in preparation for zooming
 * TODO: Write the zoom code
 */

int main(void) {
    output_image(WIDTH, HEIGHT, ITERATIONS);

    return 0;
}
