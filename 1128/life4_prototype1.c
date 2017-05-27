#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 60
#define WIDTH 60

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

//  for (int k = 0; k < 10; k++)
//      random_double();
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

void print_cells(FILE* fp)
{
    if (pass_time > 0) {
        pass_time--;
    } else {
        int i, j;

    //  fprintf(fp, "----------\n");
        for (i = 0; i < WIDTH; i++) {
            fputc('_', fp);
        }
        fputc('\n', fp);

        for (i = 0; i < HEIGHT; i++) {
            for (j = 0; j < WIDTH; j++) {
                const char c = (cell[i][j] == 1) ? '#' : ' ';
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

void update_cells()
{
    int i, j;
    int cell_next[HEIGHT][WIDTH];

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cell_next[i][j] = 0;
            const int n = count_adjacent_cells_loop(i, j);

            /*  ???  */
            if (cell[i][j] == 1) {
                if (n == 2 || n == 3) {
                    cell_next[i][j] = 1;
                } else if (random_double() <= 0.3) {
                    cell_next[i][j] = 1;
                }
            } else {
                if (n == 3) {
                    if (random_double() <= 0.7)
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
    int gen;
    FILE* fp;

    if (argc >= 2) pass_time = atoi(argv[1]);

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
        if (pass_time <= 0)
            printf("generation = %d\n", gen);
        update_cells();
        print_cells(fp);
    }

    fclose(fp);
    if (load_file) {
        fclose(lfp);
    }

    return 0;
}
