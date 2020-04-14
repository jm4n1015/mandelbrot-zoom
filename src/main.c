#include <stdio.h>
#include <mandelbrot.h>

#define WIDTH       75
#define HEIGHT      50
#define ITERATIONS  50

int main(void) {
    struct mandelbrot m = mandelbrot_init(WIDTH, HEIGHT, ITERATIONS);

    mpc_t min, max;

    mpc_init2(min, m.prec);
    mpc_init2(max, m.prec);

    mpc_set_si_si(min, -2, -1, m.rnd);
    mpc_set_si_si(max, 1, 1, m.rnd);

    mandelbrot_image(&m, &min, &max);

    mandelbrot_clear(&m);

    mpc_clear(min);
    mpc_clear(max);

    return 0;
}
