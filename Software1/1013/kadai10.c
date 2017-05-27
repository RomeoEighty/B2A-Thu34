#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum {
    NUMBEROFALPHABET = 26,
};

int count[NUMBEROFALPHABET][NUMBEROFALPHABET];
char buffer[2];

double probability[NUMBEROFALPHABET];

void push(char);


int main(void)
{
    char c;
    int sum = 0;
    double entropy = 0.;

    while ((c = getchar()) != EOF) {
        push(c);
        if (isalpha(buffer[0]) && isalpha(buffer[1]))
            count[buffer[1] - 'a'][buffer[0] - 'a'] += 1;
    }

    for (int ch1 = 0; ch1 < NUMBEROFALPHABET; ch1++) {
        sum = 0;
        entropy = 0.;

        for (int ch2 = 0; ch2 < NUMBEROFALPHABET; ch2++)
            sum += count[ch1][ch2];
        for (int ch2 = 0; ch2 < NUMBEROFALPHABET; ch2++)
            if (count[ch1][ch2] > 0)
                entropy += 1. / sum * (double)count[ch1][ch2] * ( (log(sum) - log((double)count[ch1][ch2])) / log(2));

        printf("[%c]: ent = %e\n", ch1 + 'a',  entropy);
    }

    return 0;
}

void push(char ch)
{
    buffer[1] = buffer[0];
    buffer[0] = ch;
}
