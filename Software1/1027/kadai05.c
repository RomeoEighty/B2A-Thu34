/* 
 * ------------------------
 * 16/10/27
 * kadai05
 * 540479c
 * Ryutar Yamaguchi
 * "conway's game of life
 * ------------------------
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

//#define DEBUG

int get_filesize(FILE *fp)
{
    int count = 0;
    while (fgetc(fp) != EOF) {
        count++;
    }
    return count;
}

enum {
    ROWS = 50,
    COLUMNS = 48,
};

typedef enum {
    GETNUM,
    IGNORESPACE,
    GETCHAR,
} State_t;

int run_length_decoding(FILE *src)
{
    State_t state = GETNUM;
    char ch;
    char num_buf[16];
    char char_buf;
    int i = 0;
    char *array = (char *)malloc(sizeof(char) * get_filesize(src));
    int j = 0;
    while ((ch = fgetc(src)) != EOF) {
        switch (ch) {
        case GETNUM:
            if (isdigit(ch)) {
                num_buf[i++] = ch;
            } else if (ch == ' ') {
                state = GETCHAR;
                i = 0;
                j = atoi(num_buf);
                memset(num_buf, '\0', 16);
            } else {
                return EOF;
            }
            break;
        case GETCHAR:
            char_buf = ch;
            state = IGNORESPACE;
            break;
        case IGNORESPACE:
            if (ch == ' ') {
                for (int k = j; k > 0; k--) {
                    array[j++] = char_buf;
                }
                char_buf = '\0';
                state = GETNUM;
            } else {
                fputs("Data file is corrupted\n", stderr);
                return EOF;
            }
            break;
        default:
            fputs("Undefined state\n", stderr);
            return EOF;
        }
    };
    return 0;
}

void arrcpy(bool (*dest)[COLUMNS], const bool (*src)[COLUMNS], const int rows)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void parr(bool (*arr)[COLUMNS], int rows)
{
    for (int i = 0; i < COLUMNS; i++) {
        putchar('_');
    }
    puts("");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (arr[i][j]) {
                putchar('#');
            } else {
                putchar(' ');
            }
        }
        puts("|");
    }
}

bool isalive(bool (*arr)[COLUMNS], int rows, int row_current, int column_current, bool flag_current)
{
    int count = 0;
    if (row_current != 0 && column_current != 0 && row_current != rows - 1 && column_current != COLUMNS - 1) { // not in end
        if (arr[row_current - 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current - 1][column_current]) {
            count++;
        }
        if (arr[row_current - 1][column_current + 1]) {
            count++;
        }
        if (arr[row_current][column_current - 1]) {
            count++;
        }
        if (arr[row_current][column_current + 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current]) {
            count++;
        }
        if (arr[row_current + 1][column_current + 1]) {
            count++;
        }
    } else if (row_current == 0 && column_current == 0) { // top left
        if (arr[row_current][column_current + 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current]) {
            count++;
        }
        if (arr[row_current + 1][column_current + 1]) {
            count++;
        }
    } else if (row_current == 0 && column_current == COLUMNS - 1) { // top right
        if (arr[row_current][column_current - 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current]) {
            count++;
        }
    } else if (row_current == ROWS - 1 && column_current == 0) { // bottom left
        if (arr[row_current - 1][column_current]) {
            count++;
        }
        if (arr[row_current - 1][column_current + 1]) {
            count++;
        }
        if (arr[row_current][column_current + 1]) {
            count++;
        }
    } else if (row_current == ROWS - 1 && column_current == COLUMNS - 1) { // bottom right
        if (arr[row_current - 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current - 1][column_current]) {
            count++;
        }
        if (arr[row_current][column_current - 1]) {
            count++;
        }
    } else if (row_current == 0) { // top
        if (arr[row_current][column_current - 1]) {
            count++;
        }
        if (arr[row_current][column_current + 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current]) {
            count++;
        }
        if (arr[row_current + 1][column_current + 1]) {
            count++;
        }
    } else if (row_current == ROWS - 1) { // bottom
        if (arr[row_current - 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current - 1][column_current]) {
            count++;
        }
        if (arr[row_current - 1][column_current + 1]) {
            count++;
        }
        if (arr[row_current][column_current - 1]) {
            count++;
        }
        if (arr[row_current][column_current + 1]) {
            count++;
        }
    } else if (column_current == 0) { // left end
        if (arr[row_current - 1][column_current]) {
            count++;
        }
        if (arr[row_current - 1][column_current + 1]) {
            count++;
        }
        if (arr[row_current][column_current + 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current]) {
            count++;
        }
        if (arr[row_current + 1][column_current + 1]) {
            count++;
        }
    } else if (column_current == COLUMNS - 1) { // right end
        if (arr[row_current - 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current - 1][column_current]) {
            count++;
        }
        if (arr[row_current][column_current - 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current - 1]) {
            count++;
        }
        if (arr[row_current + 1][column_current]) {
            count++;
        }
    }

#ifdef DEBUG
    printf("%d", count);
    if (column_current == COLUMNS - 1)
        puts("");
#endif

    if (flag_current) {
        if (count == 3 || count == 2) {
            return true;
        } else {
            return false;
        }
    } else {
        if (count == 3) {
            return true;
        } else {
            return false;
        }
    }

}



int main(void)
{
#ifdef DEBUG
    bool array[ROWS][COLUMNS] = {{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,0,1,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,1,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,1,0,0,0,0, 0,0,0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,0,0,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,1,0,0,0,0,0,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,1,0,0,0,0, 0,0,0,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,0,0,0,0, 0,0,0,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0}};
#else
    bool array[ROWS][COLUMNS] = {{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,1,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,1,0,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,1,1,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,1,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,1,1,0,0,0,0,0,0,0, 0,1,0,0,0,1,0,1,1,0, 0,0,0,1,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,1,0,0, 0,0,0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,1,1,1,1,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,1,1,1,1,1,1,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,0,0,0,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,0,0,0,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,0,0,0,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,1,1,1,1, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,1,1,0,1,1,1,1, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0,0,1, 1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                                                                                                     
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0}};
#endif

    bool array_buf[ROWS][COLUMNS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            array_buf[i][j] = false;
        }
    }
//  arrcpy(array_buf, array, ROWS);
    parr(array, ROWS);

    while (1) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                array_buf[i][j] = isalive(array, ROWS, i, j, array[i][j]);
            }
        }
        arrcpy(array, array_buf, ROWS);
        parr(array, ROWS);
        usleep(100000);
        system("clear");
    }

    return 0;
}