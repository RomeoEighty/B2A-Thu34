#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int count[26];

int main(void)
{
    char c;

    while ((c = getchar()) != EOF) {
        if (isalpha(c))
            count[c - 'a']++;
    }

    for (char ch = 'a'; ch <= 'z'; ch++)
        printf("%c : %d\n", ch, count[ch - 'a']);

    return 0;
}
