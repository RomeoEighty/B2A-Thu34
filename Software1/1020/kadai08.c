#include <stdio.h>

int strcmp1(char *s, char *t);

int main(void)
{
    char s[128] = "ABC";
    char s1[128] = "ABC";
    char s2[128] = "ABD";
    char s3[128] = "B";
    char s4[128] = "AAAA";


    printf("strcmp(%s, %s) = %d\n", s, s1, strcmp1(s, s1));
    printf("strcmp(%s, %s) = %d\n", s, s2, strcmp1(s, s2));
    printf("strcmp(%s, %s) = %d\n", s, s3, strcmp1(s, s3));
    printf("strcmp(%s, %s) = %d\n", s, s4, strcmp1(s, s4));

    return 0;
}

int strcmp1(char *s, char *t)
{
    for (; (*s != '\0') && (*t != '\0'); s++, t++)
        if (*s != *t)
            return ((*s > *t) - (*s < *t));

    return 0;
}
