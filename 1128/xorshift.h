#ifndef XORSHIFT_H_INCLUDED
#define XORSHIFT_H_INCLUDED

unsigned int xorshift(void);
int random_integer(int m);
double random_double(void);
double random_normal(void);
double random_normal_d(double mu, double sigma);
#endif
