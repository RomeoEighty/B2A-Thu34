#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 120
#define HEIGHT 70

const double G = 1.0;  // gravity constant

struct star {
    int index;   // index
    double M;   // mass
    double x;   // position_x
    double y;   // position_y
    double Vx;  // velocity_x
    double Vy;  // velocity_y
    bool exist; // exitance (if collision happens, it turns to false
};

struct star stars[] = {
    {1, 1.0, -10.0, 0.0, 0.0, 0.0, true},
    {2, 0.5, 10.0, 4.3, 0.0, 0.01, true},
    {3, 0.5, 10.0, -4.3, 0.0, -0.01, true},
};

const int nstars = sizeof(stars) / sizeof(struct star);

void plot_stars(FILE* fp, const double t)
{
    int i;
    char space[WIDTH][HEIGHT];

    memset(space, ' ', sizeof(space));
    for (i = 0; i < nstars; i++) {
        if (stars[i].exist) {
            const int x = WIDTH / 2 + stars[i].x;
            const int y = HEIGHT / 2 + stars[i].y;;
            if (x < 0 || x >= WIDTH) continue;
            if (y < 0 || y >= HEIGHT) continue;
            char c = '.';
            if (stars[i].M >= 1.0)
                c = 'o';
            else if (stars[i].M >= 5.0)
                c = 'O';
            space[x][y] = c;
            space[x + 1][y] = stars[i].index + '0';
        }
    }

    int x, y;
    fprintf(fp, "----------\n");
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++)
            fputc(space[x][y], fp);
        fputc('\n', fp);
    }
    fflush(fp);

    printf("t = %5.1f", t);
    for (i = 0; i < nstars; i++)
        if (stars[i].exist)
            printf(", stars[%d].x = %7.2f, stars[%d].y = %7.2f", i, stars[i].x, i, stars[i].y);
    printf("\n");
}

void update_velocities(const double dt)
{
    for (int i = 0; i < nstars; i++) {
        if (stars[i].exist) {
            double ax = 0.0;
            double ay = 0.0;
            double sigma_x = 0.0;
            double sigma_y = 0.0;
            double distance;

            for (int j = 0; j < nstars; j++) {
                if (stars[j].exist) {
                    if (i != j) {
                        distance = fabs(pow(sqrt(pow(stars[j].x - stars[i].x, 2.0) + pow(stars[j].y - stars[i].y, 2.0)), 3.));
                        if (distance <= 0.5) {
                            double mass_sum = stars[i].M + stars[j].M;
                            stars[j].exist = false;
                            stars[i].Vx = (stars[i].M * stars[i].Vx + stars[j].M * stars[j].Vx) / mass_sum;
                            stars[i].Vy = (stars[i].M * stars[i].Vy + stars[j].M * stars[j].Vy) / mass_sum;
                            stars[i].M = mass_sum;
                        } else {
                            sigma_x += stars[j].M * (stars[j].x - stars[i].x) / distance;
                            ax += sigma_x * G;
                            sigma_y += stars[j].M * (stars[j].y - stars[i].y) / distance;
                            ay += sigma_y * G;
                        }
                    }
                }
            }

            stars[i].Vx += ax * dt;
            stars[i].Vy += ay * dt;
        }
    }
}

void uvel_runge_kutta(const double dt)
{
    for (int i = 0; i < nstars; i++) {
        if (stars[i].exist) {
            double ax = 0.0;
            double ay = 0.0;
            double sigma_x = 0.0;
            double sigma_y = 0.0;
            double distance;

            for (int j = 0; j < nstars; j++) {
                if (stars[j].exist) {
                    if (i != j) {
                        distance = fabs(pow(sqrt(pow(stars[j].x - stars[i].x, 2.0) + pow(stars[j].y - stars[i].y, 2.0)), 3.));
                        if (distance <= 0.5) {
                            double mass_sum = stars[i].M + stars[j].M;
                            stars[j].exist = false;
                            stars[i].Vx = (stars[i].M * stars[i].Vx + stars[j].M * stars[j].Vx) / mass_sum;
                            stars[i].Vy = (stars[i].M * stars[i].Vy + stars[j].M * stars[j].Vy) / mass_sum;
                            stars[i].M = mass_sum;
                        } else {
                            sigma_x += stars[j].M * (stars[j].x - stars[i].x) / distance;
                            ax += sigma_x * G;
                            sigma_y += stars[j].M * (stars[j].y - stars[i].y) / distance;
                            ay += sigma_y * G;
                        }
                    }
                }
            }

            stars[i].Vx += ax * dt;
            stars[i].Vy += ay * dt;
        }
    }
}

void update_positions(const double dt)
{
    int i;
    for (i = 0; i < nstars; i++) {
        stars[i].x += stars[i].Vx * dt;
        stars[i].y += stars[i].Vy * dt;
    }
}

void upos_runge_kutta(const double dt)
{
    int i;
    for (i = 0; i < nstars; i++) {
        stars[i].x += stars[i].Vx * dt / 2;
        stars[i].y += stars[i].Vy * dt / 2;
    }
}

int main(int argc, char *argv[])
{
    const char* filename = "space.txt";
    FILE* fp;
    if ((fp = fopen(filename, "a")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return 1;
    }

    double dt = 1.0;
    if (argc >= 2) dt = atof(argv[1]);
    const double stop_time = 1000;

//  int i;
    double t;
//  for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
//      update_velocities(dt);
//      update_positions(dt);
//      if (i % 10 == 0) {
//          plot_stars(fp, t);
//          usleep(200 * 1000);
//      }
//  }
    int i;
    int j = 0;
    for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
        update_velocities(dt);
        update_positions(dt);
        if (i >= 10 * j) {
            plot_stars(fp, t);
            usleep(200 * 1000);
            j++;
        }
    }

    fclose(fp);

    return 0;
}
