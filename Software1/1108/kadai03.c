#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCT 30000

double random_float(double, double);
bool is_random_inside_circle(double);

int main()
{
    int seed;
//    int k, ct = 0;
    int ct = 0;
//    float x, y;
    double x, y;

    printf("Please input random seed:");
    scanf("%d", &seed); /* シードの入力 */
    printf("seed = %d\n", seed);
    srand(seed); /* 乱数の初期化 */

    // 正方形内部に点(x,y)をMAXCT個作り、
    // 半径１の扇の中に(x,y)が入ってる場合の数をctとして数える操作をここに書く．
    for (int i = 0; i < MAXCT; i++) {
        if (is_random_inside_circle(1.0))
            ct++;
    }

    printf("ct %d\n", ct);
    x = ct;
    y = MAXCT;
    printf("Approx. PI = %f\n", 4.0 * x / y);
}

/* minとmaxの間の実数乱数を発生する関数 */
double random_float(double min, double max)
{
    return min + ((max - min) * rand()) / RAND_MAX;

}

bool is_random_inside_circle(double radius)
{
    double x = random_float(-1.0, 1.0);
    double y = random_float(-1.0, 1.0);

    return radius >= sqrt(x * x + y * y);
}
