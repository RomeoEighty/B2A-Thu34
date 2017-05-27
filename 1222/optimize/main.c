#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "optimize.h"
#include "func1.h"

int main(const int argc, const char *argv[])
{
    clock_t st, et;
    const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;

    int i;
    const int dim = f_dimension();

    double *x = malloc(dim * sizeof(double));
    for (i = 0; i < dim; i++) {
        x[i] = 0;
    }

    puts("------- gradient descent -------");
    printf("alpha = %f\n", alpha);

    st = clock();
    const int iter = optimize(alpha, dim, x, f_gradient);
    et = clock();

    printf("number of iterations = %d\n", iter);
    printf("x[0] %lf x[1] %lf\n", x[0], x[1]);
    printf("process time : %Lf\n", (long double)(et - st) / CLOCKS_PER_SEC);

    const double hightOfFuji = 3.77612;
    printf("estimated average temperature at the summit of Mt. Fuji(%lfm)\n"
           "\tCelsius:%lf degree\n", hightOfFuji * 1000, x[0] * hightOfFuji + x[1]);

    double coefficient;
    st = clock();
    init_data();
    coefficient = regression(x, dim);
    et = clock();
    puts("------- single regression result -------");
    printf("x[0] %lf x[1] %lf\n", x[0], x[1]);
    printf("coefficient : %lf\n", coefficient);
    printf("process time : %Lf\n", (long double)(et - st) / CLOCKS_PER_SEC);
    free(x);

    return 0;
}

