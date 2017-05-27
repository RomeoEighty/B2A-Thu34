/*
 * 540479c
 * Ryutaro Yamaguchi
 * paint1.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#define WIDTH  70
#define HEIGHT 70

#define MAX_HISTORY 100
#define BUFSIZE 1000

char canvas[WIDTH][HEIGHT];

const char *default_history_file = "history.txt";

typedef struct node 
{
    char *str;
    struct node *next;
} Node;

Node *begin = NULL;

Node *push_front(Node *begin, const char *str)
{
    // Create a new element
    Node *p = malloc(sizeof(Node));
    char *s = malloc(strlen(str) + 1);
    strcpy(s, str);
    p->str = s;
    p->next = begin; 

    return p;  // Now the new element is the first element in the list
}

Node *pop_front(Node *begin)
{
    assert(begin != NULL); // Don't call pop_front() when the list is empty
    Node *p = begin->next;

    free(begin->str);
    free(begin);

    return p;
}

Node *pop_back(Node *begin)
{
    assert(begin != NULL);

    if (begin->next == NULL) return pop_front(begin);

    Node *newend = begin;

    while (newend->next->next) {
        newend = newend->next;
    }

    // pop off the last node
    free(newend->next->str);
    free(newend->next);

    newend->next = NULL;
    return begin;
}

Node *push_back(Node *begin, const char *str)
{
    if (begin == NULL) {   // If the list is empty
        return push_front(begin, str);
    }

    // Find the last element
    Node *p = begin;
    while (p->next != NULL) {
        p = p->next;
    }

    // Create a new element
    Node *q = malloc(sizeof(Node));
    char *s = malloc(strlen(str) + 1);
    strcpy(s, str);
    q->str = s;
    q->next = NULL;

    // The new element should be linked from the previous last element
    p->next = q;

    return begin;
}

Node *remove_all(Node *begin)
{
    while ((begin = pop_front(begin)))
        ; // Repeat pop_front() until the list becomes empty
    return begin;  // Now, begin is NULL
}


void print_canvas(FILE *fp)
{
    int x, y;

    fprintf(fp, "----------\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            const char c = canvas[x][y];
            fputc(c, fp);
        }
        fputc('\n', fp);
    }
    fflush(fp);
}

void init_canvas()
{
    memset(canvas, ' ', sizeof(canvas));
}

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}

void draw_line(const int x0, const int y0, const int x1, const int y1)
{
    int i;
    const int n = max(abs(x1 - x0), abs(y1 - y0));
    for (i = 0; i <= n; i++) {
        const int x = x0 + i * (x1 - x0) / n;
        const int y = y0 + i * (y1 - y0) / n;
        canvas[x][y] = '#';
    }
}

void draw_rect(const int x0, const int y0, const int width, const int height)
{
    draw_line(x0, y0, x0 + width, y0);
    draw_line(x0, y0, x0, y0 + height);
    draw_line(x0 + width, y0, x0 + width, y0 + height);
    draw_line(x0, y0 + height, x0 + width, y0 + height);
}

void draw_circle(const int x0, const int y0, const int radius)
{
    for (int i = x0 - radius; i <= x0 + radius; i++) {
        for (int j = y0 - radius; j <= y0 + radius; j++) {
            if ((int)sqrt(pow(i - x0, 2) + pow(j - y0, 2)) == radius) {
                if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT)
                    canvas[i][j] = '#';
            }
        }
    }
}

void save_history(const char *filename)
{
    if (filename == NULL)
        filename = default_history_file;

    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return;
    }

    const Node *p;
    for (p = begin; p != NULL; p = p->next) {
        fprintf(fp, "%s", p->str);
    }

    printf("saved as \"%s\"\n", filename);

    fclose(fp);
}

// Interpret and execute a command
//   return value:
//     0, normal commands such as "line"
//     1, unknown or special commands (not recorded in history[])
//     2, quit
int interpret_command(const char *command)
{
    int i;
    char buf[BUFSIZE];
    strcpy(buf, command);
    buf[strlen(buf) - 1] = 0; // remove the newline character at the end

    const char *s = strtok(buf, " ");

    if (strcmp(s, "line") == 0) {
        int x0, y0, x1, y1;
        x0 = atoi(strtok(NULL, " "));
        y0 = atoi(strtok(NULL, " "));
        x1 = atoi(strtok(NULL, " "));
        y1 = atoi(strtok(NULL, " "));
        draw_line(x0, y0, x1, y1);
        return 0;
    }

    if (strcmp(s, "rect") == 0) {
        int x0, y0, width, height;
        x0 = atoi(strtok(NULL, " "));
        y0 = atoi(strtok(NULL, " "));
        width = atoi(strtok(NULL, " "));
        height = atoi(strtok(NULL, " "));
        draw_rect(x0, y0, width, height);
        return 0;
    }

    if (strcmp(s, "circle") == 0) {
        int x0, y0, radius;
        x0 = atoi(strtok(NULL, " "));
        y0 = atoi(strtok(NULL, " "));
        radius = atoi(strtok(NULL, " "));
        draw_circle(x0, y0, radius);
        return 0;
    }

    if (strcmp(s, "save") == 0) {
        s = strtok(NULL, " ");
        save_history(s);
        return 1;
    }

    if (strcmp(s, "undo") == 0) {
        init_canvas();
        begin = pop_back(begin);
        Node *p;
        for (p = begin; p != NULL; p = p->next) {
            interpret_command(p->str);
        }
        return 1;
    }

    if (strcmp(s, "quit") == 0) {
        return 2;
    }

    printf("error: unknown command.\n");

    return 1;
}

int main()
{
    const char *filename = "canvas.txt";
    FILE *fp;
    char buf[BUFSIZE];

    if ((fp = fopen(filename, "a")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return 1;
    }

    Node *p;
    init_canvas();
    print_canvas(fp);

    while (1) {

        int hsize = 0;
        for (p = begin; p != NULL; p = p->next) hsize++;
        printf("%d > ", hsize);
        fgets(buf, BUFSIZE, stdin);

        const int r = interpret_command(buf);
        if (r == 2) break;
        if (r == 0) {
            begin = push_back(begin, buf);
        }

        print_canvas(fp);
    }

    fclose(fp);

    return 0;
}
