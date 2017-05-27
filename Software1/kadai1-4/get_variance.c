#include "get_average.h"
#include "get_variance.h"

double get_variance(const int *numbers, const int length)
{
    double ave = get_average(numbers, length);
    double variance = 0.0;

    for (int i = 0; i < length; i++)
        variance += (ave - (double)numbers[i]) * (ave - (double)numbers[i]);

    variance /= (double)length;

    return variance;
}
