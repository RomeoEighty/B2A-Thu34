#include <stdio.h>
#include <math.h>

double newton(double x);
#define f(x) ((x - 1)*(x - 3)*(x - 5)*sin(x)) // f(x)の定義
#define df(x) ((x - 3) * (x - 5) * sin(x) + (x - 1) * (x - 5) * sin(x) + (x - 1) * (x - 3) * sin(x) + (x - 1) * (x - 3) * (x - 5) * cos(x))   // f'(x)の定義

int main() 
{
    // 初期値x0の設定
    double initial_value = 0.;
    double result;

    for (int i = 0; initial_value < 10; i++, initial_value = initial_value + 0.2) {
        result = newton(initial_value);
        printf("x%02d = %9.4lf...f(%9.4lf) = %9.4lf\n", i, result, result, f(result));
    }

//  printf("x1 = %lf\n",newton(initial_value));
//  // 別の初期値x0の設定
//  initial_value = 3.0;
//  printf("x2 = %lf\n",newton(initial_value));
    
    return 0;
}

double newton(double val) 
{
    double x1 = val - f(val) / df(val);
    
    if(fabs(x1 - val) < 0.0001) {
        return x1;
    } else {
        return newton(x1); //再帰呼び出し
    }
}
