#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("error\n");
        exit(1);
    }
    srand(atoi(argv[1]));
    for (int i = 0; i < 10; i++) {
        printf("%d\n", rand() % 1000 + 1);
    }
    return 0;
}
