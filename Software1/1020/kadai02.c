#include <stdio.h>
#include <stdlib.h>

int main()
{
    int* p;
    p = (int*)malloc(sizeof(int));
    *p = 100;
    printf("*p = %d\n", *p); // $B%]%$%s%?(Bp$B$N;X$9(Bint$B7?$N<BBN$r(Bprint
    printf("p = %p\n", p); // $B%]%$%s%?$N%"%I%l%9$r(B16$B?J?t$G!#(B
    printf("&p = %p\n", &p); // $B%]%$%s%?$N%]%$%s%?$N%"%I%l%9CM$r(B16$B?J?t$G!#(B
    return 0;
}
