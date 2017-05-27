#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(atoi(argv[1]));
    for (int i = 0; i < 10; i++) {
        printf("%d\n", rand() % 1000 + 1);
    }
    return 0;
}
