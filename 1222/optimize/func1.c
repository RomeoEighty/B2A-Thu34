#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    char *loc;   // location name
    double alt;  // altitude (km)
    double temp; // temperature (centigrade)
} Sample;

const Sample data[] = {
    { "Nozawa-onsen",   0.576,  22.3 },
    { "Iiyama",         0.313,  23.3 },
    { "Nagano",         0.4182, 23.8 },
    { "Oomachi",        0.784,  21.1 },
    { "Sugadaira",      1.253,  18.5 },
    { "Karuizawa",      0.9991, 19.5 },
    { "Matsumoto",      0.610,  23.6 },
    { "Nagawa",         1.068,  19.7 },
    { "Suwa",           0.760,  22.7 },
    { "Nobeyama",       1.350,  18.4 },
    { "Ina",            0.633,  22.7 },
    { "Nagiso",         0.560,  22.3 },
    { "Iida",           0.5164, 23.9 },
    { "Minami-shinano", 0.407,  23.5 }
};

const int numOfSamples = sizeof(data) / sizeof(data[0]);

double totalsum(int index)
{
    double sum = 0.;
    if (index == 0) {
        for (int i = 0; i < numOfSamples; i++) {
            sum += data[i].alt;
        }
    } else {
        for (int i = 0; i < numOfSamples; i++) {
            sum += data[i].temp;
        }
    }
    return sum;
}
static double sum[2];

double average(int index)
{
    return sum[index] / numOfSamples;
}
static double avg[2];

double sigma(int index)
{
    double sigma = 0.;
    double avg = average(index);
    if (index == 0) {
        for (int i = 0; i < numOfSamples; i++) {
            sigma += pow(data[i].alt - avg, 2);
        }
    } else {
        for (int i = 0; i < numOfSamples; i++) {
            sigma += pow(data[i].temp - avg, 2);
        }
    }
    return sqrt(sigma / numOfSamples);
}

double cov(void)
{
    double cov = 0.;
    for (int i = 0; i < numOfSamples; i++) {
        cov += (avg[0] - data[i].alt) * (avg[1] - data[i].temp);
    }
    return cov / numOfSamples;
}

double regression(double x[], int dim)
{
    if (dim != 2) {
        fprintf(stderr, "[%s %d] dimention is not supported.\n", __FILE__, __LINE__);
        exit(1);
    }
    double a[2] = {0., 0.};
    for (int i = 0; i < numOfSamples; i++) {
        a[0] += data[i].alt * data[i].temp;
        a[1] += pow(data[i].alt, 2);
    }
    x[0] = (numOfSamples * a[0] - sum[0] * sum[1]) / (numOfSamples * a[1] - pow(sum[0], 2));
    x[1] = avg[1] - x[0] * avg[0];
    return cov() / sigma(0) / sigma(1);
}

void init_data(void)
{
    sum[0] = totalsum(0);
    sum[1] = totalsum(1);
    avg[0] = average(0);
    avg[1] = average(1);
}

int f_dimension(void)
{
    return 2;
}

double f_value(const double x[])
{
    double value = 0.;
    for (int i = 0; i < numOfSamples; i++) {
        value += pow(data[i].temp - x[0] * data[i].alt - x[1], 2);
    }
    return value;
}

void f_gradient(const double x[], double g[])
{
    g[0] = 0.;
    g[1] = 0.;

    for (int i = 0; i < numOfSamples; i++) {
        g[0] += - 2.0 * data[i].alt * (data[i].temp - x[0] * data[i].alt - x[1]);
        g[1] += - 2.0 * (data[i].temp - x[0] * data[i].alt - x[1]);
    }
}

