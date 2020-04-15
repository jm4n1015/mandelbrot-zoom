#include "image.h"
#include "mandelbrot.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCALE   10

// Concatenate r, g, and b variables
static unsigned long rgb_join(float r, float g, float b);
// Convert from hsv to rgb
static unsigned long convert_color(float h, float s, float v);

void output_image(unsigned int width, unsigned int height, unsigned int iterations) {
    struct mandelbrot m = mandelbrot_init(width, height, iterations);

    // Times three because of each color channe          
    // TODO: reformat this possibly
    unsigned char *buf = malloc(m.width * m.height * 3 * sizeof(*buf));

    mpc_t min, max;

    mpc_init2(min, m.prec);
    mpc_init2(max, m.prec);

    mpc_set_si_si(min, -2, -1, m.rnd);
    mpc_set_si_si(max, 1, 1, m.rnd);

    mandelbrot_image(&m, &min, &max);

    for (unsigned int y = 0; y < m.height; ++y) {
        for (unsigned int x = 0; x < m.width; ++x) {
            unsigned int unformatted = m.image[y * m.width + x] * SCALE;

            float saturation = 1;

            // Set to black
            if (unformatted == iterations * SCALE) {
                saturation = 0;
            }

            unsigned long color = convert_color(unformatted % 360, saturation, 1.0f);

            // Once again, 3 for each channel
            buf[y * m.width * 3 + x * 3 + 0] = color >> 16;
            buf[y * m.width * 3 + x * 3 + 1] = color >> 8;
            buf[y * m.width * 3 + x * 3 + 1] = color >> 0;
        }
    }

    // Only for testing purposes
    FILE *fp = fopen("test.ppm", "wb");

    fprintf(fp, "P6\n%d %d\n255\n", m.width, m.height);
    fwrite(buf, m.width * 3, m.height, fp);
    fflush(fp);

    // Clear resources that are no longer needed
    fclose(fp);

    mandelbrot_clear(&m);

    mpc_clear(min);
    mpc_clear(max);

    free(buf);
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
