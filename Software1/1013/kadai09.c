#include <stdio.h>
#include <ctype.h>
#include <math.h>

int count[26];

int main(void)
{
    char c = '\0';
    int sum = 0;
    double entropy = 0.;

    while ((c = getchar()) != EOF)
        if (isalpha(c))
            count[c - 'a']++;

    for (char ch = 'a'; ch <= 'z'; ch++)
        sum += count[ch - 'a'];

    for (char ch = 'a'; ch <= 'z'; ch++)
        entropy += 1. / sum * (double)count[ch - 'a'] * ( (log(sum) - log((double)count[ch - 'a'])) / log(2));

    printf("entropy = %e\n", entropy);
    return 0;
}
