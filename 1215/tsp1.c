#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 70
#define HEIGHT 40
#define MAX_CITIES 1000

//#define DEBUG
//#define STATUS

#ifdef DEBUG

#define IOLog1(...) { printf(__VA_ARGS__); fflush(stdout); }
//#define IOLog2(args...) { printf(args...); fflush(stdout); }

#endif /* DEBUG */

#ifndef IOLog1
#define IOLog1(args...)
#endif
#ifndef IOLog2
#define IOLog2(args...) 
#endif


typedef struct
{
    int x;
    int y;
} City;

City city[MAX_CITIES];
char map[WIDTH][HEIGHT];

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}

void swap(int *route1, int *route2)
{
    int tmp = *route2;
    *route2 = *route1;
    *route1 = tmp;
}

void draw_line(const int x0, const int y0, const int x1, const int y1)
{
    int i;
    const int n = max(abs(x1 - x0), abs(y1 - y0)) + 1;
    for (i = 0; i <= n; i++) {
        const int x = x0 + i * (x1 - x0) / n;
        const int y = y0 + i * (y1 - y0) / n;
        if (map[x][y] == ' ')
            map[x][y] = '*';
    }
}

void draw_route(const int n, const int* route)
{
    if (route == NULL)
        return;

    int i;
    for (i = 0; i < n; i++) {
        const int c0 = route[i];
        const int c1 = (i < n - 1) ? route[i + 1] : route[0];
        draw_line(city[c0].x, city[c0].y, city[c1].x, city[c1].y);
    }
}

void plot_cities(FILE* fp, const int n, const int* route)
{
    int i, x, y;

    fprintf(fp, "----------\n");

    memset(map, ' ', sizeof(map));

    for (i = 0; i < n; i++) {
        char buf[100];
        sprintf(buf, "C_%d", i);
        for (size_t j = 0; j < strlen(buf); j++) {
            const int x = city[i].x + j;
            const int y = city[i].y;
            map[x][y] = buf[j];
        }
    }

    draw_route(n, route);

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            const char c = map[x][y];
            fputc(c, fp);
        }
        fputc('\n', fp);
    }

    fflush(fp);
}

double distance(const int i, const int j)
{
    const double dx = city[i].x - city[j].x;
    const double dy = city[i].y - city[j].y;
    return sqrt(dx * dx + dy * dy);
}

double total_distance(const int n, const int route[])
{
    double sum_d = 0;
    int i;
    for (i = 0; i < n; i++) {
        const int c0 = route[i];
        const int c1 = (i < n - 1) ? route[i + 1] : route[0];
        sum_d += distance(c0, c1);
    }
    return sum_d;
}

bool visited[MAX_CITIES];
double best_total_distance;
int best_route[MAX_CITIES];

void search_hc(const int i, const int n, int route[])
{
    if (i == n) {
        /* do nothing */
        return;
    }

    double td_i = DBL_MAX;
    int best_j;
    for (int j = 1; j < n; j++) {
        if (j == i)
            continue;
        swap(&route[i], &route[j]);
        double tdtmp = total_distance(n, route);
        if (td_i >= tdtmp) {
            td_i = tdtmp;
            best_j = j;
        }
        swap(&route[i], &route[j]);
    }
    if (i != best_j)
        swap(&route[i], &route[best_j]);
    search_hc(i + 1, n, route);
}

double solve_hc(const int n, int route[])
{
    route[0] = 0;  // Start from city[0]

    best_total_distance = DBL_MAX;

    for (int i = 1; i < n; i++) {
        route[i] = i;
    }
    for (int i = 0; i < n; i++) {
        best_route[i] = route[i];
    }

#ifndef DEBUG
    unsigned max_trials = 100000;
#else
    unsigned max_trials = 10;
#endif
#ifdef STATUS
    int pct = 0;
#endif
    for (unsigned j = 0; j < max_trials; j++) {
#ifdef STATUS
        if ((double)pct / 100 <= (double)j / max_trials) {
            printf("\r%d%%",++pct);
            if (pct == 100)
                puts("");
            fflush(stdout);
        }
#endif

        /* shuffle */
        for (int i = 1; i < n; i++) {
            int k = rand() % (n - 1) + 1;
            if (k != i)
                swap(&route[i], &route[k]);
        }

#ifdef DEBUG
        for (int i = 0; i < n; i++) {
            printf("%d ", route[i]);
        }
        printf(" <- route [%d]\n", __LINE__);
        for (int i = 0; i < n; i++) {
            printf("%d ", best_route[i]);
        }
        puts(" <- best_route");
        puts("");
#endif
        search_hc(1, n, route);

        // Compute the total distance
        double sum_d = total_distance(n, route);

        if (best_total_distance > sum_d) {
            for (int i = 1; i < n; i++) {
                best_route[i] = route[i];
            }
            best_total_distance = sum_d;
        }
    }

    return best_total_distance;
}

int main(const int argc, const char** argv)
{
    FILE* fp;
    const char* map_file = "map.txt";
    if ((fp = fopen(map_file, "a")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", map_file);
        return 1;
    }

    if (argc != 2) {
        fprintf(stderr, "error: please specify the number of cities.\n");
        exit(1);
    }

    const int n = atoi(argv[1]);
    assert(n > 1 && n <= MAX_CITIES);

    for (int i = 0; i < n; i++) {
        city[i].x = rand() % (WIDTH - 5);
        city[i].y = rand() % HEIGHT;
    }

    plot_cities(fp, n, NULL);
//  sleep(1);

    int route[MAX_CITIES];
    const double d = solve_hc(n, route);


    printf("total distance = %f\n", d);
    plot_cities(fp, n, best_route);

//  for (int i = 0; i < n; i++)
//      printf("[%d]%d ", i, best_route[i]);
//  puts("");
//  best_route[7] = 1;
//  best_route[8] = 19;
//  best_route[9] = 16;
//  plot_cities(fp, n, best_route);
//  printf("total distance = %f\n", total_distance(n, best_route));
    return 0;
}
