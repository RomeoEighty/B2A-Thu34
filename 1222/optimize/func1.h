#ifndef OPTIMIZE_FUNCONE_H
#define OPTIMIZE_FUNCONE_H
extern int numOfSamples;

double totalsum(int);
double avg(int);
double sigma(int);
double cov(void);
double regression(double x[], int);
void init_data(void);
int f_dimension(void);
double f_value(const double x[]);
void f_gradient(const double x[], double g[]);

#endif
