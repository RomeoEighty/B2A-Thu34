#include <stdio.h>

int strlen1(char *s);

int main(void)
{
    char str[] = "hogefoobar";

    printf("length of str:%d\n", strlen1(str));

    return 0;
}

int strlen1(char *s)
{
    int len = 0;

    for (; s[len] != '\0'; len++);

    return len;
}
