#include <limits.h>
#include "get_max.h"

int get_max(const int *numbers, const int length)
{
    int max = INT_MIN;
    for (int i = 0; i < length; i++)
        if (max < numbers[i]) max = numbers[i];
    return max;
}
