#include <stdio.h>

enum {
    MAXNUM = 1000,
};

int main(void)
{
    for (int i = 1; i <= MAXNUM; i++) {
        printf("%d", i);
        if (!(i % 3) | ((i % 10) == 3) | ((i % 100) / 10 == 3) | ((i % 1000) / 100 == 3))
            printf("!!!");
        puts("");
    }
    return 0;
}
