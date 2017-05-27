#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 50
#define WIDTH 70

#define MAX_XORSHIFT 4294967295

unsigned int xorshift(void)
{
    static unsigned int x = 123456789;
    static unsigned int y = 362436069;
    static unsigned int z = 521288629;
    static unsigned int w = 88675123;
    unsigned int t;
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

int random_integer(int m)
{
    return xorshift() % m;
}

double random_double(void)
{
    return xorshift() * 1.0 / MAX_XORSHIFT;
}

int cell[HEIGHT][WIDTH];
bool load_file = false;

FILE* lfp;
void init_cells()
{
    int i, j;

    if (!(load_file)) {
        for (i = 0; i < HEIGHT; i++) {
            for (j = 0; j < WIDTH; j++) {
                if (random_double() <= 0.1) {
                    cell[i][j] = 1;
                } else {
                    cell[i][j] = 0;
                }
            }
        }
    } else {
        for (i = 0; i < HEIGHT; i++) {
            for (j = 0; j < WIDTH; j++) {
                cell[i][j] = 0;
            }
        }
        while (fscanf(lfp, "%d,%d\n", &i, &j) != EOF) {
            cell[i][j] = 1;
        }
    }

}

int count_all_live_cells(void)
{
    int count = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            count += cell[i][j];
        }
    }
    return count;
}

void print_cells(FILE* fp)
{
    int i, j;

    fprintf(fp, "%d----------\n", count_all_live_cells());

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            const char c = (cell[i][j] == 1) ? '#' : ' ';
            fputc(c, fp);
        }
        fputc('\n', fp);
    }
    fflush(fp);

    usleep(100000);
}

int count_adjacent_cells(int i, int j)
{
    int n = 0;
    int k, l;
    for (k = i - 1; k <= i + 1; k++) {
        if (k < 0 || k >= HEIGHT)
            continue;
        for (l = j - 1; l <= j + 1; l++) {
            if (k == i && l == j)
                continue;
            if (l < 0 || l >= WIDTH)
                continue;
            n += cell[k][l];
        }
    }
    return n;
}

void update_cells()
{
    int i, j;
    int cell_next[HEIGHT][WIDTH];

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cell_next[i][j] = 0;
            const int n = count_adjacent_cells(i, j);

            /*  ???  */
            if (cell[i][j] == 1) {
                if (n == 2 || n == 3) {
                    cell_next[i][j] = 1;
                }
            } else {
                if (n == 3) {
                    cell_next[i][j] = 1;
                }
            }
        }
    }

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cell[i][j] = cell_next[i][j];
        }
    }
}


int main(void)
{
    int gen;
    FILE* fp;

    if ((fp = fopen("cells.txt", "a")) == NULL) {
        fprintf(stderr, "error: cannot open a file.\n");
        return 1;
    }
    if ((lfp = fopen("cells_load.csv", "r")) == NULL) {
        printf("load file doesn't exist.\n");
    } else {
        printf("Successflly load file.\n");
        load_file = true;
    }

    init_cells();
    print_cells(fp);

    for (gen = 1;; gen++) {
        printf("generation = %3d  number of life cell = %d\n", gen, count_all_live_cells());
        update_cells();
        print_cells(fp);
    }

    fclose(fp);
    if (load_file) {
        fclose(lfp);
    }

    return 0;
}
