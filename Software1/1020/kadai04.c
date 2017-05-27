#include <stdio.h>
int main()
{
    char* namelist[] = {"Kawahara", "Nakata", "Sakamoto", "Shinoda"};
    int i;
    /* ポインタ配列で表現された文字列の表示 */
    for (i = 0; i < 4; i++) {
        printf("Name: %s\n", namelist[i]);
    }
    return 0;
}
