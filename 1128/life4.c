/*
 * 540479c
 * Ryutaro Yamaguchi
 * life4.c
 * "Conway's game of life original"
 * Rules:
 *      1. Any live cell under seven with fewer than five young neighbours lives on
 *         the next generation.
 *      2. Any live cell with more than 4 young neighbours dies, as if by over-population.
 *      3. 10% of dead cell with two live young neighbours and 100% of dead cell with
 *         three live young neighbours becomes a live cell, as if by reproduction.
 *      4. Any live cell dies at the age of seven, as if by aging.
 *      5. Death by underpopulation does not happen.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 73
#define WIDTH  237

#define MAX_XORSHIFT 4294967295

int pass_time = 0;

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

    for (int k = 0; k < 50; k++) random_double();
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
            if (cell[i][j] >= 1) count++;
        }
    }
    return count;
}

void print_cells(FILE* fp)
{
    if (pass_time > 0) {
        pass_time--;
    } else {
        int i, j;

        fprintf(fp, "%5d", count_all_live_cells());
        for (i = 5; i < WIDTH; i++) {
            fputc('_', fp);
        }
        fputc('\n', fp);

        for (i = 0; i < HEIGHT; i++) {
            for (j = 0; j < WIDTH; j++) {
//              const char c = (cell[i][j] = 1) ? '#' : ' ';
                char c;
                switch (cell[i][j]) {
                    case 0:
                        c = ' ';
                        break;
                    case 1:
                        c = '.';
                        break;
                    case 2:
                        c = '*';
                        break;
                    case 3:
                        c = 'o';
                        break;
                    case 4:
                        c = 'O';
                        break;
                    case 5:
                        c = '@';
                        break;
                    case 6:
                        c = '#';
                        break;
                    default:
                        exit(1);
                }
                fputc(c, fp);
            }
            fputc('|', fp);
            fputc('\n', fp);
        }
        fflush(fp);

        usleep(50000);
    }
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

int count_adjacent_cells_loop(int i, int j)
{
    int n = 0;
    for (int k = i - 1; k <= i + 1; k++) {
        for (int l = j - 1; l <= j + 1; l++) {
            if (k == i && l == j) continue;
            n += cell[(k + HEIGHT) % HEIGHT][(l + WIDTH) % WIDTH];
        }
    }
    return n;
}

int count_adjacent_cells_loop_youngling(int i, int j)
{
    int n = 0;
    int num;
    for (int k = i - 1; k <= i + 1; k++) {
        for (int l = j - 1; l <= j + 1; l++) {
            if (k == i && l == j) continue;
            if ((num = cell[(k + HEIGHT) % HEIGHT][(l + WIDTH) % WIDTH]) <= 3 &&
                 num >= 1) {
                n++;
            }
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
            const int n = count_adjacent_cells_loop_youngling(i, j);

            /*  ???  */
            if (cell[i][j] >= 1) {
                cell_next[i][j] = cell[i][j] + 1;
                if (n >= 5 || cell_next[i][j] >= 7) {
                    cell_next[i][j] = 0;
                }
            } else { // born
                if (n == 3 || (n == 2 && random_double() >= 0.9)) {
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


int main(int argc, char *argv[])
{
    int decades;
    FILE* fp;

    if (argc >= 2) pass_time = atoi(argv[1]);

    if ((fp = fopen("cells.txt", "a")) == NULL) {
        fprintf(stderr, "error: cannot open a file.\n");
        return 1;
    }
//  if ((lfp = fopen("cells_load.csv", "r")) == NULL) {
//      printf("load file doesn't exist.\n");
//  } else {
//      printf("Successflly load file.\n");
//      load_file = true;
//  }

    init_cells();
    print_cells(fp);

    for (decades = 1;; decades++) {
        if (pass_time <= 0)
            printf("year:%5d\tnumber of living cells:%d\n", decades * 10, count_all_live_cells());
        update_cells();
        print_cells(fp);
    }

    fclose(fp);
    if (load_file) {
        fclose(lfp);
    }

    return 0;
}
