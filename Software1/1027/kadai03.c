/* 
 * ------------------------
 * 16/10/27
 * kadai03
 * 540479x
 * Ryutar Yamaguchi
 * ------------------------
 */

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

enum {
    WIDTH = 30,
    HEIGHT = 15,
};

char FILENAME[] = "data.txt";

bool parse_input(char *, int *, int *, char *);

int main(void)
{
    char array[WIDTH][HEIGHT];
    char str[128] = {};
    int height = 0;
    int width = 0;
    bool data_loaded = false;
    char ch;
    char *p;
    FILE *fp;
    bool data_changed = false;
    char load_data_buf[WIDTH * HEIGHT + 5] = {};

    /* 
     * opening file sequence
     */
    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("save data doesn't exist.\n");
        printf("creating a new file\n");
        if ((fp = fopen(FILENAME, "w+")) == NULL) {
            fprintf(stderr, "%d \"%s\" %s\n", errno, FILENAME, strerror(errno));
            return 1;
        }
    } else {
        fclose(fp);
        if ((fp = fopen(FILENAME, "a+")) == NULL) {
            fprintf(stderr, "%d \"%s\" %s\n", errno, FILENAME, strerror(errno));
            return 1;
        }
        fseek(fp, 0, SEEK_SET);
        while (fgets(load_data_buf, sizeof(load_data_buf), fp) != NULL);

        if (!(fgets(load_data_buf, sizeof(load_data_buf), fp))) {
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    array[j][i] = load_data_buf[i * WIDTH + j];
                }
            }
            puts("load save data success!");
            data_loaded = true;
        }

    }

    while (1) {
        if (!data_loaded) {
            printf("Initializing...\n");
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    if ((i - HEIGHT / 2) * (i - HEIGHT / 2) + (j - WIDTH / 2) * (j - WIDTH / 2) < (HEIGHT / 2 * HEIGHT / 2))
                        array[j][i] = '*';
                    else
                        array[j][i] = '.';
                }
            }
            data_loaded = true;
        } else if (data_changed) {
            array[width][height] = ch;
            data_changed = false;
        } else {
            // do nothing
        }

        for (int i = 0; i < HEIGHT; i++) {
            if (i == 0) {
                printf("   ");
                for (int j = 0; j < WIDTH; j++) {
                    if (!(j % 5)) {
                        for (int k = 0; k <= 4 - (int)log10(j); k++)
                            printf("  ");
                        printf("%d", j);
                    }
                }
//              for (int j = 0; j < WIDTH; j++) {
//                  if (!(j % 5))
//                      printf("%d", j);
//                  else
//                      putchar(' ');
//                  putchar(' ');
//              }
                puts("");
            }
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) {
                    if (!(i % 5)) {
                        printf("%3d", i);
                    } else {
                        printf("   ");
                    }
                }
                putchar(array[j][i]);
                fputc(array[j][i], fp);
                putchar(' ');
            }
            puts("");

        }
        fputc('\n', fp);
        fflush(fp);

        printf("\nusage: [height], [width], [character]\n");
        printf("%% ");
        fgets(str, sizeof(str), stdin);
        if ((p = strchr(str, '\n')) != NULL) {
            *p = '\0';
        }

        data_changed = parse_input(str, &height, &width, &ch);
        system("clear");
    }

    if (fclose(fp) == EOF) {
        fprintf(stderr, "%d \"%s\" %s\n", errno, FILENAME, strerror(errno));
    }

    return 0;
}

bool parse_input(char *str, int *height, int *width, char *ch)
{
    *height = 0;
    *width = 0;
    
    char delimiters[] = ",";

    char *tok;
    int valnum = 0;

    tok = strtok(str, delimiters);

    while(valnum <= 2) {
        //printf("arg%d:`%s'\n",valnum, tok);

        /* define height and width part */
        if (valnum <= 1) {
            size_t i, j = 0;
            bool digit_detected = false;
            bool digit_loaded = false;
            char buf_num[8] = {};

            for (i = 0; i < strlen(tok); i++) {
                if (!(digit_detected)) {
                    if (isdigit(tok[i]))
                        digit_detected = true;
                    else if (tok[i] != ' ') {
                        // argument should be either numbers or spaces
                        printf("argument:%d ", valnum + 1);
                        puts(strerror(22)); // 22:Invalid argument
                        puts("argument contains invalid value");
                        sleep(3);
                        return false;
                    } else {
                        // spaces are ignored
                    }
                }

                // when digit_detected, value should be loaded at once
                if (digit_detected && !(digit_loaded)) {
                    if (isdigit(tok[i])) {
                        buf_num[j++] = tok[i];
                    } else if (tok[i] == ' ') {
                        digit_loaded = true;
                    } else {
                        printf("argument:%d ", valnum + 1);
                        puts(strerror(22)); // 22:Invalid argument
                        puts("Try again");
                        sleep(3);
                        return false;
                    }
                }

                // if digits' already been loaded, spaces are only allowed.
                if (digit_loaded) {
                    if (tok[i] != ' ') {
                        printf("argument:%d ", valnum + 1);
                        puts(strerror(22)); // 22:Invalid argument
                        puts("Values should be delimited by `,', not ` '");
                        puts("try again");
                        sleep(3);
                        return false;
                    }
                }
            }

//          if (i != strlen(tok)) {
//              printf("argument:%d ", valnum + 1);
//              puts(strerror(22)); // 22:Invalid argument
//              puts("try again");
//              sleep(3);
//              return false;
//          }
            if (valnum == 0) {
                if (atoi(buf_num) >= HEIGHT) {
                    printf("Height value too large\n");
                    printf("Max height: %d\n", HEIGHT - 1);
                    sleep(3);
                    return false;
                }
                *height = atoi(buf_num);
            } else {
                if (atoi(buf_num) >= WIDTH) {
                    printf("Width value too large\n");
                    printf("Max width: %d\n", WIDTH - 1);
                    sleep(3);
                    return false;
                }
                *width = atoi(buf_num);
            }
        } else if (valnum == 2) {
            /* input char */
            size_t i;
            bool char_loaded = false;
            char buf_char;

            for (i = 0; i < strlen(tok); i++) {
                if (!(char_loaded)) {
                    if (tok[i] != ' ') {
                        buf_char = tok[i];
                        char_loaded = true;
                    } else {
                        // spaces are ignored
                    }
                } else {
                    if (tok[i] != ' ') {
                        printf("argument:%d ", valnum + 1);
                        puts(strerror(22)); // 22:Invalid argument
                        puts("try again");
                        sleep(3);
                        return false;
                    } else {
                        // spaces are ignored
                    }
                }
            }
            
            *ch = buf_char;
        } else {
            strerror(07);
            printf("usage: [height], [width], [character]\n");
            sleep(3);
            return false;
        }

        tok = strtok(NULL, delimiters);
        valnum++;
    }

//  if (!(tok[0])) {
//      strerror(07); // 07: Arg list too long
//      sleep(3);
//      return false;
//  } else {
        return true;
//    }
}
//  for (int i = 0; str[i] != ' '; i++);
//  if (!isdigit(str[i]))
//      printf(error_message);
//  else
        
