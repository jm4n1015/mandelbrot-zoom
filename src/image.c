#include "image.h"
#include "mandelbrot.h"

#include <math.h>
#include <stdlib.h>

// The color scale
// TODO: this will probably need to change
#define SCALE       10

// The number of color channels
#define CHANNELS    3

// TODO: Replace this with something better
#define ZOOM_RATE   50.0

// Concatenate r, g, and b variables
static unsigned long rgb_join(float r, float g, float b);
// Convert from hsv to rgb
static unsigned long convert_color(float h, float s, float v);

void output_image(FILE *stream, struct mandelbrot *m, const mpc_t *min, const mpc_t *max) {
    // Times three because of each color channe          
    // TODO: reformat this possibly
    unsigned char *buf = malloc(m->width * m->height * CHANNELS * sizeof(*buf));

    mandelbrot_image(m, min, max);

    for (unsigned int y = 0; y < m->height; ++y) {
        for (unsigned int x = 0; x < m->width; ++x) {
            unsigned int unformatted = m->image[y * m->width + x] * SCALE;

            float saturation = 1;

            // Set to black
            if (unformatted == m->iterations * SCALE) {
                saturation = 0;
            }

            unsigned long color = convert_color(unformatted % 360, saturation, 1.0f);

            // Once again, 3 for each channel
            buf[y * m->width * CHANNELS + x * CHANNELS + 0] = color >> 16;
            buf[y * m->width * CHANNELS + x * CHANNELS + 1] = color >> 8;
            buf[y * m->width * CHANNELS + x * CHANNELS + 1] = color >> 0;
        }
    }

    fprintf(stream, "P6\n%d %d\n255\n", m->width, m->height);
    fwrite(buf, m->width * CHANNELS, m->height, stream);
    fflush(stream);

    // Clear resources that are no longer needed
    free(buf);
}

void output_stream(FILE *stream, const mpc_t *center, unsigned int max_zoom,
                   unsigned int width, unsigned int height, unsigned int iterations) {
    // Mandelbrot struct
    struct mandelbrot m = mandelbrot_init(width, height, iterations);

    // Zoom amount; this is the percent to increase the scale
    double zoom = 1.0 - (1.0 / ZOOM_RATE);
    // This is the complement of the zoom, for scaling calculations
    double zoom_complement = 1.0 - zoom;

    mpfr_t tmp1, tmp2, tmp3;

    mpfr_init2(tmp1, m.prec);
    mpfr_init2(tmp2, m.prec);
    mpfr_init2(tmp3, m.prec);

    // These are the minimum and maximum values
    mpc_t min, max;

    mpc_init2(min, m.prec);
    mpc_init2(max, m.prec);

    mpc_set_si_si(min, -2, -1, m.rnd);
    mpc_set_si_si(max, 1, 1, m.rnd);

    // Zoom in towards the center
    for (unsigned int i = 0; i < max_zoom; ++i) {
        // Min
        // Real
        mpc_real(tmp1, min, m.rnd);
        mpc_real(tmp2, *center, m.rnd);
        mpfr_mul_d(tmp1, tmp1, zoom, m.rnd);
        mpfr_mul_d(tmp2, tmp2, zoom_complement, m.rnd);
        mpfr_add(tmp3, tmp1, tmp2, m.rnd);

        // Imaginary
        mpc_imag(tmp1, min, m.rnd);
        mpc_imag(tmp2, *center, m.rnd);
        mpfr_mul_d(tmp1, tmp1, zoom, m.rnd);
        mpfr_mul_d(tmp2, tmp2, zoom_complement, m.rnd);
        mpfr_add(tmp1, tmp1, tmp2, m.rnd);

        mpc_set_fr_fr(min, tmp3, tmp1, m.rnd);

        // Max
        // Real
        mpc_real(tmp1, max, m.rnd);
        mpc_real(tmp2, *center, m.rnd);
        mpfr_mul_d(tmp1, tmp1, zoom, m.rnd);
        mpfr_mul_d(tmp2, tmp2, zoom_complement, m.rnd);
        mpfr_add(tmp3, tmp1, tmp2, m.rnd);

        // Imaginary
        mpc_imag(tmp1, max, m.rnd);
        mpc_imag(tmp2, *center, m.rnd);
        mpfr_mul_d(tmp1, tmp1, zoom, m.rnd);
        mpfr_mul_d(tmp2, tmp2, zoom_complement, m.rnd);
        mpfr_add(tmp1, tmp1, tmp2, m.rnd);

        mpc_set_fr_fr(max, tmp3, tmp1, m.rnd);

        // Run the mandelbrot calculation + image transformation
        output_image(stream, &m, &min, &max);
    }

    mpfr_clear(tmp1);
    mpfr_clear(tmp2);
    mpfr_clear(tmp3);

    mpc_clear(min);
    mpc_clear(max);
}

static unsigned long rgb_join(float r, float g, float b) {
    unsigned long ir = roundf(r * 255.0f);
    unsigned long ig = roundf(g * 255.0f);
    unsigned long ib = roundf(b * 255.0f);
    
    return (ir << 16) | (ig << 8) | ib;
}

static unsigned long convert_color(float h, float s, float v) {
    int i;
    float f, p, q, t;
    float r, g, b;

    if(s == 0) {
        r = g = b = v;
        return 0;
    }

    h /= 60;
    i = h;
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch(i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
            break;
    }

    return rgb_join(r, g, b);
}
