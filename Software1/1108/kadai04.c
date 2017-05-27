#include <stdio.h>
#include <math.h>

/* kadai04-01 */
double pie(double x0, int N)
{
    int x = 10 * x0;
    for (int i = 0; i < N; i++) {
        x = ((x < 5) ? 2 * x : 20 - 2 * x);
    }
    return (double) x / 10;
}

int main(void)
{
    double x0 = 0.1;
    printf("x0:%4f\n", x0);
    for (int i = 0; i < 10; i++) {
        printf("N:%4d -> %f\n", i, pie(x0, i));
    }
    int i = 100;
    printf("N:%4d -> %f\n", i, pie(x0, i));
    i = 1000;
    printf("N:%4d -> %f\n", i, pie(x0, i));

    return 0;
}
