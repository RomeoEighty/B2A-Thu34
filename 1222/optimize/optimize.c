//#define O_VERBOSE

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "optimize.h"

double calc_norm(const int dim, double v[])
{
    double tmp = 0;
    for (int i = 0; i < dim; i++) {
        tmp += v[i] * v[i];
    }
    tmp = sqrt(tmp);
    return tmp;
}

int optimize(const double alpha, const int dim, double x[], 
             void (*calc_grad)(const double [], double []))
{
    int i;

    double *g = malloc(dim * sizeof(double));

    unsigned iter = 0;
    while (++iter < UINT_MAX) {

        (*calc_grad)(x, g);

        const double norm = calc_norm(dim, g);
#ifdef O_VERBOSE
        printf("%3d norm = %7.4f", iter, norm);
        for (i = 0; i < dim; i++) {
            printf(", x[%d] = %7.4f", i, x[i]);
        }
        putchar('\n');
#endif

        if (norm <= 0.00001) break;

        for (i = 0; i < dim; i++) {
            x[i] -= alpha * g[i];
        }
    }

    free(g);

    return iter;
}
