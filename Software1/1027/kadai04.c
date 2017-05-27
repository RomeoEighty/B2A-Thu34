/* 
 * ------------------------
 * 16/10/27
 * kadai04
 * 540479c
 * Ryutar Yamaguchi
 * "run length encoding"
 * ------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char SOURCEFILE[] = "data.txt";
char ENCODEDFILE[] = "data_encoded.txt";

int run_length_encoding(FILE *, FILE *);

int main(void)
{
    FILE *src_fp;
    FILE *dest_fp;

    if ((src_fp = fopen(SOURCEFILE, "r")) == NULL) {
        printf("save data doesn't exist.\n");
        return 1;
    }
    if ((dest_fp = fopen(ENCODEDFILE, "w")) == NULL) {
        printf("save destination file couldn't open.\n");
        return 1;
    }

    if (run_length_encoding(dest_fp, src_fp) == EOF) {
        printf("encoding failed\n");
        return 1;
    }

    // fclose()
    fclose(src_fp);
    fclose(dest_fp);
    return 0;
}

int run_length_encoding(FILE *dest, FILE *src)
{
    char ch;
    if ((ch = fgetc(src)) == EOF) {
        return EOF;
    }
    char buf = ch;
    int counter = 1;
    char str_buf[8];
    memset(str_buf, '\0', 8);
    while ((ch = fgetc(src)) != EOF) {
        if (ch == buf) {
            counter++;
        } else {
            sprintf(str_buf, "%d %c ", counter, buf);
            if (fputs(str_buf, dest) == EOF) {
                printf("write failed\n");
                return EOF;
            }
            buf = ch;
            counter = 1;
        }
    }
//  sprintf(str_buf, "%d%c", counter, buf);
//  if (puts(str_buf, dest) == EOF) {
//      printf("write failed\n");
//      return EOF;
//  }
    return 0;
}
