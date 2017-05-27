#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int count[26][26];
char buffer[2];

double probability[26];

void push(char);
void getMin(const double*, const int, double*, char*);
void getMax(const double*, const int, double*, char*);


int main(void)
{
    char c;
    int sum = 0;
    double min, max;
    char min_index, max_index;

    while ((c = getchar()) != EOF) {
        push(c);
        if (isalpha(buffer[0]) && isalpha(buffer[1]))
            count[buffer[1] - 'a'][buffer[0] - 'a'] += 1;
    }

    for (char ch1 = 'a'; ch1 <= 'z'; ch1++) {
        sum = 0;
        min = 1.1;
        max = -0.;
        max_index = 0;
        min_index = 0;
        for (char ch2 = 'a'; ch2 <= 'z'; ch2++)
            sum += count[ch1 - 'a'][ch2 - 'a'];
        for (char ch2 = 'a'; ch2 <= 'z'; ch2++) {
            probability[ch2 - 'a'] = (double)count[ch1 - 'a'][ch2 - 'a'] / (double)sum;
        }
        
        getMin(probability, 26, &min, &min_index);
        getMax(probability, 26, &max, &max_index);
        printf("[%c]: min = %c (%e), max = %c (%e)\n", ch1, min_index + 'a', min, max_index + 'a', max);
    }

    return 0;
}

void push(char ch)
{
    buffer[1] = buffer[0];
    buffer[0] = ch;
}

void getMin(const double* array, const int length, double *min, char *index)
{
    for (int i = 0; i < length; i++)
        if (array[i] < *min && array[i] > 0.) {
            *min = array[i];
            *index = (char)i;
        }
}

void getMax(const double* array, const int length, double *max, char *index)
{
    for (int i = 0; i < length; i++)
        if (array[i] >= *max) {
            *max = array[i];
            *index = (char)i;
        }
}
