#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int count[26][26];
char data[2];


void push(char);


int main(void)
{
    char c;

    while ((c = getchar()) != EOF) {
        push(c);
        if (isalpha(data[0]) && isalpha(data[1]))
            count[data[1] - 'a'][data[0] - 'a'] += 1;
    }


    for (char ch1 = 'a'; ch1 <= 'z'; ch1++)
        for (char ch2 = 'a'; ch2 <= 'z'; ch2++)
            printf("[%c][%c] = %d\n", ch1, ch2, count[ch1 - 'a'][ch2 - 'a']);

    return 0;
}

void push(char ch)
{
    data[1] = data[0];
    data[0] = ch;
}
