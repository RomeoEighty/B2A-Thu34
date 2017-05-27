#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *error = "error\n";
char *fault = "invalid input\n";

int main(void)
{
    char janken[3];

    printf("Rock(g), Paper(p), Scissors(c)!!\n");
    while (1) {
        if (!(fgets(janken, sizeof(janken)/sizeof(char), stdin)))
            perror(error);

        switch (tolower(janken[0])) {
        case 'g':
            puts("You threw Rock, but I threw Scissors: You're winner!");
            break;
        case 'c':
            puts("You threw Scissors, but I threw Paper: You're winner!");
            break;
        case 'p':
            puts("You threw Paper, but I threw Rock: You're winner!");
            break;
        case 'q':
            puts("QUIT");
            exit(0);
            break;
        default:
            perror(fault);
        }
        memset(janken, '\0', sizeof(janken)/sizeof(char));
    }

    return 0;
}
