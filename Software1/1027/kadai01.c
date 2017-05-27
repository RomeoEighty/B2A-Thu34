/* 
 * ------------------------
 * 16/10/27
 * kadai01
 * 540479c
 * Ryutar Yamaguchi
 * ------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum {
    WIDTH = 30,
    HEIGHT = 15,
};

int main(void)
{
    char array[WIDTH][HEIGHT];
    int offset = 0;

    while (1) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if ((i - HEIGHT / 2) * (i - HEIGHT / 2) + (j - offset - WIDTH / 2) * (j - offset - WIDTH / 2) < (HEIGHT / 2 * HEIGHT / 2))
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

        offset++;
        sleep(1);
        system("clear");
    }

    return 0;
}
