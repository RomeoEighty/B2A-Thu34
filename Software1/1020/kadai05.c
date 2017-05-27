#include <stdio.h>
#include <stdlib.h>
#define NUMALPHA 26
int main()
{
    char* str;
    int i;
    /* mallocによる領域の確保が必要なケース */
    str = (char*)malloc(sizeof(char) * NUMALPHA + 1);
    /* AからZまでをFor文で入れる */
    for (i = 0; i < NUMALPHA; i++) {
        *(str + i) = i + 'A';
    }
    *(str + i) = '\0';
    printf("Alphabet: %s\n", str);
    return 0;
}
