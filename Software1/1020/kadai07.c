#include <stdio.h>
#include <stdlib.h>

void strcpy1(char *s, char *t);

int main(void)
{
    char t_str[] = "This is a source string.";
    char *s_str = (char *) malloc(sizeof(char) * 128);

    printf("t_str:%s\ns_str:%s\n", t_str, s_str);
    strcpy1(s_str, t_str);
    printf("t_str:%s\ns_str:%s\n", t_str, s_str);

    return 0;
}

void strcpy1(char *s, char *t)
{
    char *tmp = t;
    for (; *tmp != '\0'; tmp++)
        *s++ = *tmp;
    *s = '\0';
}
