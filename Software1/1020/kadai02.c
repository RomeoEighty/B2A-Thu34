#include <stdio.h>
#include <stdlib.h>

int main()
{
    int* p;
    p = (int*)malloc(sizeof(int));
    *p = 100;
    printf("*p = %d\n", *p); // ポインタpの指すint型の実体をprint
    printf("p = %p\n", p); // ポインタのアドレスを16進数で。
    printf("&p = %p\n", &p); // ポインタのポインタのアドレス値を16進数で。
    return 0;
}
