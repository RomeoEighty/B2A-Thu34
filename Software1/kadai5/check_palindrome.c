#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH 100000

typedef enum {
    START,
    WAIT_END,
    END,
} flag_t;

bool check_palindrome(const char *str,  const int length)
{
    for (int i = 0; i < length / 2; i++) {
        if (!(str[i] == str[length - 1 - i])) {
            return false;
        }
    }
    return true;
}

int main(void)
{
    char buf[MAX_LENGTH];
    char word[MAX_LENGTH];
    int j;
    flag_t flag = END;

    fgets(buf, MAX_LENGTH, stdin);

    for (int i = 0; i < strlen(buf); i++) {
        if (flag == END) {
            if (isalnum(buf[i])) {
                flag = WAIT_END;
                word[j++] = buf[i];
            }
        } else if (flag == WAIT_END) {
            if (isalnum(buf[i])) {
                word[j++] = buf[i];
            } else {
                flag = END;
                if (check_palindrome(word, j)) {
                    printf("%s is palindrome\n", word);
                }
                memset(word, '\0', j);
                j = 0;
            }
        }
    }

        return 0;
    }
