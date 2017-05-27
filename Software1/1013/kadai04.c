#include <stdio.h>

enum {
    WIDTH = 30,
    HEIGHT = 15,
};

int main(void)
{
    char array[WIDTH][HEIGHT];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if ((i - HEIGHT / 2) * (i - HEIGHT / 2) + (j - WIDTH / 2) * (j - WIDTH / 2) < (HEIGHT / 2 * HEIGHT / 2))
                array[j][i] = '*';
            else
                array[j][i] = '.';
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(array[j][i]);
            putchar(' ');
        }
        puts("");
    }

    return 0;
}
