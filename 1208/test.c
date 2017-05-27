#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HOGE 80
#define FUGA 70
#define FOOBAR (HOGE * FUGA)
int main(void)
{
    char str[] = "hoge fuga  foo";
    char *s;
    //s = strtok(str, " ");
    printf("%d ", strcmp(strtok(str, " "), "hoge"));
    printf("%lf ", atan(-1));
    for (int i = 0; i < 2; i++) {
        //s = strtok(NULL, " ");
        printf("%d ", strcmp(strtok(NULL, " "), "foo"));
    }
    printf("%d %d %d\n", HOGE, FUGA, FOOBAR);
    return 0;
}
