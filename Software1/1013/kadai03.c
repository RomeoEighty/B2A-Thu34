#include <stdio.h>
#include <math.h>



int main(void)
{
    int x = 0;
    for (int i = 0; i < 100; i++) {
        x = 10 + 10 * sin(2 * M_PI * (double)i / 30);
        for (int j = 0; j < x; j++) {
            printf(" ");
        }
        printf("*\n");
    }
    return 0;
}
