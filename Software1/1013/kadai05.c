#include <stdio.h>
//#define APPLEBERRY

#ifndef APPLEBERRY
#define NUMFRT 2
#define MAXCHAR 50

int main()
{
  char instrument[MAXCHAR] = "Pen";
  char fruit[NUMFRT][MAXCHAR] = {"Apple", "Pineapple"};

#else
#define NUMFRT 3
#define MAXCHAR 50
 
int main()
{
  char instrument[MAXCHAR] = "Pen";
  char fruit[NUMFRT][MAXCHAR] = {"Apple", "Pineapple", "Appleberry"};

#endif
  
/* NUMFRTに応じて以下のように表示
NUMFRT = 2のとき
I have a Pen.
I have a Apple.
...Uh, Apple Pen 

I have a Pen.
I have a Pineapple.
...Uh, Pineapple Pen 

Apple Pen, Pineapple Pen, 
...Uh, Pen Pineapple Apple Pen!

NUMFRT = 3でAppleberryを追加のとき
$ ./a.out 
I have a Pen.
I have a Apple.
...Uh, Apple Pen 

I have a Pen.
I have a Pineapple.
...Uh, Pineapple Pen 

I have a Pen.
I have a Appleberry.
...Uh, Appleberry Pen 

Apple Pen, Pineapple Pen, Appleberry Pen, 
...Uh, Pen Appleberry Pineapple Apple Pen!
 */
    for (int i = 0; i < NUMFRT; i++) {
        printf("I have a %s.\n", instrument);
        printf("I have a %s.\n", fruit[i]);
        printf("...Uh, %s %s \n", fruit[i], instrument);
        puts("");
    }
    for (int i = 0; i < NUMFRT; i++) {
        printf("%s %s, ", fruit[i], instrument);
    }
    puts("");
    printf("...Uh, %s ", instrument);
    for (int i = NUMFRT - 1; i >= 0; i--) {
        printf("%s ", fruit[i]);
    }
    printf("Pen!\n");
  
    return 0;
}
