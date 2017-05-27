#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <limits.h>


enum {
    NUMBEROFALPHABET = 26,
};

char buffer[2] = {};

void push(char);
void printBhattacharyyaMin(const long int(*)[NUMBEROFALPHABET], const long int*);
void printBhattacharyyaMax(const long int(*)[NUMBEROFALPHABET], const long int*);

int main(void)
{
    long int count[NUMBEROFALPHABET][NUMBEROFALPHABET] = {};
    char c;
    long int sum_sub[NUMBEROFALPHABET] = {};

    while ((c = getchar()) != EOF) {
        push(c);
        if (isalpha(buffer[0]) && isalpha(buffer[1])) {
            count[buffer[1] - 'a'][buffer[0] - 'a']++;
            sum_sub[buffer[1] - 'a']++;
            assert(sum_sub[buffer[1] - 'a'] < LONG_MAX); // オーバーフロー処理
        }
    }

    printBhattacharyyaMin(count, sum_sub);
    printBhattacharyyaMax(count, sum_sub);

    return 0;
}

void push(char ch)
{
    buffer[1] = buffer[0];
    buffer[0] = ch;
}

void printBhattacharyyaMin(const long int (*count)[NUMBEROFALPHABET], const long int *sum_sub)
{
    double min = DBL_MAX;
    double tmp = 0.;
    int index1, index2;

    for (int i = 0; i < NUMBEROFALPHABET; i++)
        for (int j = i + 1; j < NUMBEROFALPHABET; j++) {
            tmp = 0.;
            for (int k = 0; k < NUMBEROFALPHABET; k++)
                tmp += sqrt((double)count[i][k] * (double)count[j][k] / sum_sub[i] / sum_sub[j]);
            if (min > -log(tmp)) {
                min = -log(tmp);
                index1 = i;
                index2 = j;
            }
        }

    printf("[%c][%c]: min of bd = %e\n", index1 + 'a', index2 + 'a', min);
}

void printBhattacharyyaMax(const long int (*count)[NUMBEROFALPHABET], const long int *sum_sub)
{
    double max = DBL_MIN;
    double tmp = 0.;
    int index1, index2;

    for (int i = 0; i < NUMBEROFALPHABET; i++)
        for (int j = i + 1; j < NUMBEROFALPHABET; j++) {
            tmp = 0.;
            for (int k = 0; k < NUMBEROFALPHABET; k++)
                tmp += sqrt((double)count[i][k] * (double)count[j][k] / sum_sub[i] / sum_sub[j]);
            if (max < -log(tmp)) {
                max = -log(tmp);
                index1 = i;
                index2 = j;
            }
        }

    printf("[%c][%c]: max of bd = %e\n", index1 + 'a', index2 + 'a', max);
}
