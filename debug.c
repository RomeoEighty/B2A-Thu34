
void printintMatrix(const int(*)[NUMBEROFALPHABET]);
void printdoubleMatrix(const double(*)[NUMBEROFALPHABET]);

void printintMatrix(const int (*array)[NUMBEROFALPHABET])
{
    int counter = 0;
    puts("--------------------DEBUG----------------------");
    for (int i = 0; i < NUMBEROFALPHABET; i++)
        for (int j = 0; j < NUMBEROFALPHABET; j++) {
            if ((i == 'a' - 'a') || (i == 'u' - 'a'))
                printf("[%c][%c]:%7d ", i + 'a', j + 'a', array[i][j]);
            if (counter++ % 13 == 12)
                puts("");
        }
    puts("-----------------------------------------------");
}

void printdoubleMatrix(const double (*array)[NUMBEROFALPHABET])
{
    int counter = 0;
    puts("--------------------DEBUG----------------------");
    for (int i = 0; i < NUMBEROFALPHABET; i++)
        for (int j = 0; j < NUMBEROFALPHABET; j++) {
            printf("[%c][%c]:%0.4f ", i + 'a', j + 'a', array[i][j]);
            if (counter++ % 13 == 12)
                puts("");
        }
    puts("-----------------------------------------------");
}
