/*
 * 540479c
 * Ryutaro Yamaguchi
 * paint3.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>


#define DEBUG


#ifdef DEBUG

#define IOLog1(...) { printf(__VA_ARGS__); fflush(stdout); }
//#define IOLog2(args...) { printf(args...); fflush(stdout); }

#endif /* DEBUG */

#ifndef IOLog1
#define IOLog1(args...)
#endif
#ifndef IOLog2
#define IOLog2(args...) 
#endif


#define FULLWID 1024
#define FULLHGT 1024
#define WIDTH  70
#define HEIGHT 70

#define STR(var) #var

#define MAX_HISTORY 100
#define BUFSIZE 1000

char canvas[WIDTH][HEIGHT];

const char *default_history_file = "history.txt";
FILE *canvasfp;

typedef struct node 
{
    char *str;
    struct node *next;
} Node;

typedef enum color {
    clear,              // ' '
    white =   0,        // '.'
    silver =  3,        // ','
    gray =    2,        // 'o'
    dim =     4,        // 'a'
    black =   1,        // '#'
} color_t;

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

Node *remove_num(Node *begin, const int num)
{
    int posnum = 1;
    Node *pos = begin;
    Node *newnext;
    if (num == 0) {
        return pop_front(begin);
    }
    /*
     * The index number of "pos" is always posnum - 1.
     */
    while (pos->next != NULL && posnum != num) {
        pos = pos->next;
        posnum++;
    }
    if (pos->next == NULL) { // posnum is too large: nothing to do
    } else if (pos->next->next == NULL) {
        free(pos->next->str);
        free(pos->next);
        pos->next = NULL;
    } else {
        newnext = pos->next->next;
        free(pos->next->str);
        free(pos->next);
        pos->next = newnext;
    }
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

    for (int i = 0; i < 2*WIDTH; i++)
        fprintf(fp, "-");
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            const char c = canvas[x][y];
            fputc(' ', fp);
//          fputc(c, fp);
            fputc(c, fp);
        }
        fputc('|', fp);
        fputc('\n', fp);
    }
    fflush(fp);
}

void init_canvas()
{
    memset(canvas, ' ', sizeof(canvas));
}

void refresh(void);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
//int max(const int a, const int b)
//{
//    return (a > b) ? a : b;
//}

void drawLine(const int x0, const int y0, const int x1, const int y1, const color_t color)
{
    int i;

    const int n = MAX(abs(x1 - x0), abs(y1 - y0));
    for (i = 0; i <= n; i++) {
        const int x = x0 + i * (x1 - x0) / n;
        const int y = y0 + i * (y1 - y0) / n;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            if (color == white) {
                canvas[x][y] = '.';
            } else {
                canvas[x][y] = '#';
            }
    }
}

void draw_rect(const int x0, const int y0, const int width, const int height, const color_t color)
{
    if (color == clear) {
        drawLine(x0        , y0         , x0 + width, y0         , black);
        drawLine(x0        , y0         , x0        , y0 + height, black);
        drawLine(x0 + width, y0         , x0 + width, y0 + height, black);
        drawLine(x0        , y0 + height, x0 + width, y0 + height, black);
    } else if (color != clear) {
        for (int i = x0; i <= x0 + width; i++) {
            for (int j = y0; j <= y0 + height; j++) {
                if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT)
                    if (color == black) {
                        canvas[i][j] = '#';
                    } else if (color == white) {
                        canvas[i][j] = '.';
                    }
            }
        }
    }

    if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT)
        canvas[x0][y0] = '*';
}

void drawCircle(const int x0, const int y0, const int radius, const color_t color)
{
    if (color == clear) {
        for (int i = x0 - radius; i <= x0 + radius; i++) {
            for (int j = y0 - radius; j <= y0 + radius; j++) {
                if ((int)sqrt(pow(i - x0, 2) + pow(j - y0, 2)) == radius) {
                    if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT)
                        canvas[i][j] = '#';
                }
            }
        }
    } else {
        for (int i = x0 - radius; i <= x0 + radius; i++) {
            for (int j = y0 - radius; j <= y0 + radius; j++) {
                if ((int)sqrt(pow(i - x0, 2) + pow(j - y0, 2)) <= radius) {
                    if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT)
                        if (color == black) {
                            canvas[i][j] = '#';
                        } else {
                            canvas[i][j] = '.';
                        }
                }
            }
        }
    }
    if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT)
        canvas[x0][y0] = '*';
}

void drawCurl(const int x0, const int y0, const int radius, const double dig0, const double dig1, const color_t color)
{
    const double rdig0 = dig0 / 180.0 * M_PI;
    const double rdig1 = dig1 / 180.0 * M_PI;
    //const double rdig00 = (dig0 >= 180.0) ? 180.0 : dig0;
    //const double rdig01 = (dig0 >= 180.0) ? dig0 - 180.0 : 0;
    //const double rdig10 = (dig1 >= 180.0) ? 180.0 : dig1;
    //const double rdig11 = (dig1 >= 180.0) ? dig1 - 180.0 : 0;

    for (int i = MAX(0, x0 - radius); i <= MIN(x0 + radius, WIDTH - 1); i++) {
        for (int j = MAX(0, y0 - radius); j <= MIN(y0 + radius, HEIGHT - 1); j++) {
            //double cdig = fmod(atan((double)(j - y0) / (i - x0)) + 2 * M_PI, M_PI);
            double cdig = acos((double)(i - x0) / (sqrt(pow(i - x0, 2) + pow(j - y0, 2))));
            if (-(j - y0) <= 0) // many people feel default y-axis is upside down.
                cdig = 2 * M_PI - cdig;
            if ((int)sqrt(pow(i - x0, 2) + pow(j - y0, 2)) == radius) {
                if (cdig >= rdig0 && cdig <= rdig1) {
                    //IOLog1("[cdig%lf] ", cdig);
                    canvas[i][j] = (color == white) ? '.' : '#';
                }
            }
        }
    }
    if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT)
        canvas[x0][y0] = '*';
}

/* 
 * ************************
 * fill area function (for canvas)
 * ************************
 */
typedef struct paintBuf {
    int sx; /* the right end of the field */
    int sy; /* the y coordinate of the line field */
} paintBuf_t;
#define PAINTBUFSIZE (WIDTH * HEIGHT)
#define MINX 0
#define MAXX (WIDTH - 1)
#define MINY 0
#define MAXY (HEIGHT - 1)
#define PBMPBUFSIZE (FULLWID * FULLHGT)
#define PBMMINX 0
#define PBMMAXX (FULLWID - 1)
#define PBMMINY 0
#define PBMMAXY (FULLHGT - 1)
paintBuf_t *paintBuffer; /* memory is to be allocated in fill function */
paintBuf_t *startPBuf;
paintBuf_t *endPBuf;

void colorSet(const int x, const int y, const color_t col)
{
    switch (col) {
        case black:
            canvas[x][y] = '#';
            break;
        case white:
            canvas[x][y] = '.';
            break;
        case silver:
            canvas[x][y] = ',';
            break;
        case gray:
            canvas[x][y] = 'o';
            break;
        case dim:
            canvas[x][y] = 'a';
            break;
        default:
            printf("colorSet:canvas[%d][%d] = %c:UNKNOWN_COLOR\n", x, y, canvas[x][y]);
            exit(1);
    }
}
void pbmColorSet(char *pixel, const int x, const int y, const color_t col)
{
    int resolution = 1024;
    switch (col) {
        case black:
            pixel[x + y * resolution] = 1;
            break;
        case white:
            pixel[x + y * resolution] = 0;
            break;
        case gray:
            pixel[x + y * resolution] = 2;
            break;
        case silver:
            pixel[x + y * resolution] = 3;
            break;
        case dim:
            pixel[x + y * resolution] = 4;
            break;
        default:
            printf("pbmColorSet:UNKNOWN_COLOR\n");
            exit(1);
    }
}

color_t point(const int x, const int y)
{
    switch (canvas[x][y]) {
        case '#':
            return black;
        case '.':
            return white;
        case ' ':
            return white;
        case '*':
            return white;
        case ',':
            return silver;
        case 'o':
            return gray;
        case 'a':
            return dim;
        default:
            printf("point:canvas[%d][%d] = %c:UNKNOWN_COLOR\n", x, y, canvas[x][y]);
            exit(1);
    }
}
color_t pbmPoint(const char *pixel, const int x, const int y)
{
    int resolution = 1024;
    switch (pixel[x + y * resolution]){
        case 0:
            return white;
        case 1:
            return black;
        case 2:
            return gray;
        case 3:
            return silver;
        case 4:
            return dim;
        default:
            printf("UNKNOWN_COLOR\n");
            exit(1);
    }
}

void scanLine(int lx, int rx, int y, color_t col)
{
    while (lx <= rx) {

        /* ignore non-field color */
        for ( ; lx <= rx ; lx++)
            if (point(lx, y) == col) break;
        //IOLog1("[lx %d rx %d y %d]", lx, rx, y);
        if (point(lx, y) != col) break;

        /* ignore field color */
        for (; lx <= rx ; lx++)
            if (point(lx, y) != col) break;

        endPBuf->sx = lx - 1;
        endPBuf->sy = y;

        if (++endPBuf == &paintBuffer[PAINTBUFSIZE]) /* ring buffer */
            endPBuf = paintBuffer;
    }
}
void pbmScanLine(const char *pixel, int lx, int rx, int y, color_t col)
{
    while (lx <= rx) {

        /* ignore non-field color */
        for ( ; lx <= rx ; lx++)
            if (pbmPoint(pixel, lx, y) == col) break;
        if (pbmPoint(pixel, lx, y) != col) break;

        /* ignore field color */
        for (; lx <= rx ; lx++)
            if (pbmPoint(pixel, lx, y) != col) break;

        endPBuf->sx = lx - 1;
        endPBuf->sy = y;

        if (++endPBuf == &paintBuffer[PBMPBUFSIZE]) /* ring buffer */
            endPBuf = paintBuffer;
    }
}
//void fill( int x, int y, unsigned int paintCol )
void fill(int x0, int y0, const color_t paintCol )
{
    int lx, rx; /* the right and left x coordinates of the line field */
    int ly;     /* the y coordinate of the line field */
    int i;
    if (x0 > WIDTH - 1 || y0 > HEIGHT - 1)
        return;
    color_t col = point(x0, y0); /* target field color */
    if ( col == paintCol )
        return;    /* no need to paint the same color */

    paintBuffer = malloc(sizeof(paintBuf_t) * PAINTBUFSIZE);;
    if (paintBuffer == NULL) {
        fprintf(stderr, "malloc error\n");
        return;
    }
    startPBuf = paintBuffer;
    startPBuf->sx = x0;
    startPBuf->sy = y0;
    endPBuf = startPBuf + 1;

    do {
        lx = startPBuf->sx;
        rx = startPBuf->sx;
        ly = startPBuf->sy;
        //IOLog1("lx %d rx %d ly %d", lx, rx, ly);
        if (++startPBuf == &paintBuffer[PAINTBUFSIZE]) /* ring buffer */
            startPBuf = paintBuffer;

        /* ignore the cell that is already been processed */
        if (point(lx, ly) != col)
            continue;

        /* search right border */
        while (rx < MAXX - 1) {
            if (point(rx + 1, ly) != col) break;
            rx++;
        }
        /* search left border */
        while (lx > MINX) {
            if (point( lx - 1, ly) != col) break;
            lx--;
        }
        /* lx-rx */
        for ( i = lx ; i <= rx ; i++ )
            colorSet(i, ly, paintCol);

        if ( ly - 1 >= MINY )
            scanLine(lx, rx, ly - 1, col);

        if ( ly + 1 <= MAXY )
            scanLine(lx, rx, ly + 1, col);

    } while ( startPBuf != endPBuf);
    free(paintBuffer);
}
void pbmFill(char *pixel, int x0, int y0, const color_t paintCol )
{
    int lx, rx; /* the right and left x coordinates of the line field */
    int ly;     /* the y coordinate of the line field */
    int i;
    color_t col = pbmPoint(pixel, x0, y0); /* target field color */
    if ( col == paintCol )
        return;    /* no need to paint the same color */

    paintBuffer = malloc(sizeof(paintBuf_t) * PBMPBUFSIZE);;
    if (paintBuffer == NULL) {
        fprintf(stderr, "malloc error\n");
        return;
    }
    startPBuf = paintBuffer;
    startPBuf->sx = x0;
    startPBuf->sy = y0;
    endPBuf = startPBuf + 1;

    do {
        lx = startPBuf->sx;
        rx = startPBuf->sx;
        ly = startPBuf->sy;
        if (++startPBuf == &paintBuffer[PBMPBUFSIZE]) /* ring buffer */
            startPBuf = paintBuffer;

        /* ignore the cell that is already been processed */
        if (pbmPoint(pixel, lx, ly) != col)
            continue;

        /* search right border */
        while (rx < PBMMAXX - 1) {
            if (pbmPoint(pixel, rx + 1, ly) != col) break;
            rx++;
        }
        /* search left border */
        while (lx > PBMMINX) {
            if (pbmPoint(pixel, lx - 1, ly) != col) break;
            lx--;
        }
        /* lx-rx */
        for ( i = lx ; i <= rx ; i++ )
            pbmColorSet(pixel, i, ly, paintCol);

        if ( ly - 1 >= PBMMINY )
            pbmScanLine(pixel, lx, rx, ly - 1, col);

        if ( ly + 1 <= PBMMAXY )
            pbmScanLine(pixel, lx, rx, ly + 1, col);

    } while ( startPBuf != endPBuf);
    free(paintBuffer);
}


void saveHist(const char *filename)
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


    fclose(fp);
}

void load_history(const char *);

void show_history(Node *begin)
{
    int histnum = 0;
    if (begin == NULL) {
        printf("There is no history\n");
        return;
    }

    while (begin != NULL) {
        printf("\t%d: %s", histnum++, begin->str); // Node string includes new line at the end
        begin = begin->next;
    }
    return;
}

void export2Pbm(const int resolution);

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

    if (s == NULL)
        return 1;

    if (s[0] == '#') // comment out: ignored but remembered
        return 0;

    if (strcmp(s, "line") == 0) {
        int x0, y0, x1, y1;
        char *s;
        color_t color = black;
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        x0 = atoi(s);
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        y0 = atoi(s);
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        x1 = atoi(s);
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        y1 = atoi(s);
        s = strtok(NULL, " ");
        if (s != NULL) {
            if (strcmp(s, "white") == 0)
                color = white;
            else if (strcmp(s, "clear") == 0)
                return EOF; // error: clear line is a mysterious concept
        }

        drawLine(x0, y0, x1, y1, color);
        return 0;
    }

    if (strcmp(s, "rect") == 0) {
        int x0, y0, width, height;
        color_t color = clear;
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        if (strcmp(s, "-h") == 0 || strcmp(s, "--help") == 0) {
            printf("Draw Horizontal Rectangular Command\n"
                   "\n"
                   "\tUsage: rect x y width height [color]\n"
                   "\n");
            return 1;
        }
        x0 = atoi(s);
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        y0 = atoi(s);
        if (s == NULL) {
            return EOF;
        }
        width = atoi(s);
        if (s == NULL) {
            return EOF;
        }
        height = atoi(s);
        s = strtok(NULL, " ");
        if (s != NULL) {
            if (strcmp(s, "white") == 0)
                color = white;
            else if (strcmp(s, "black") == 0)
                color = black;
        }
        draw_rect(x0, y0, width, height, color);
        return 0;
    }

    if (strcmp(s, "circle") == 0) {
        int x0, y0, radius;
        color_t color = clear;
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        if (strcmp(s, "-h") == 0 || strcmp(s, "--help") == 0) {
            printf("Draw Circle Command\n"
                   "\n"
                   "\tUsage: circle x y radius [color]\n"
                   "\n");
            return 1;
        }
        x0 = atoi(s);
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        y0 = atoi(s);
        if (s == NULL) {
            return EOF;
        }
        radius = atoi(s);
        s = strtok(NULL, " ");
        if (s != NULL) {
            if (strcmp(s, "white") == 0)
                color = white;
            else if (strcmp(s, "black") == 0)
                color = black;
        }
        drawCircle(x0, y0, radius, color);
        return 0;
    }

    if (strcmp(s, "curl") == 0) {
        int x0, y0, radius;
        double dig0, dig1;
        color_t color = black;
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        if (strcmp(s, "-h") == 0 || strcmp(s, "--help") == 0) {
            printf("Draw Curl Command\n"
                   "\n"
                   "\tUsage: curl x y radius digree0 digree1 [color]\n"
                   "\n");
            return 1;
        }
        x0 = atoi(s);
        s = strtok(NULL, " ");
        if (s == NULL) {
            return EOF;
        }
        y0 = atoi(s);
        if (s == NULL) {
            return EOF;
        }
        radius = atoi(s);
        if (s == NULL) {
            return EOF;
        }
        dig0 = atof(s);
        if (s == NULL) {
            return EOF;
        }
        dig1 = atof(s);
        s = strtok(NULL, " \n");
        if (s != NULL) {
            if (strcmp(s, "white") == 0)
                color = white;
        }
        drawCurl(x0, y0, radius, MIN(dig0, dig1), MAX(dig0, dig1), color);
        return 0;
    }

    if (strcmp(s, "fill") == 0) {
        int x0, y0;
        color_t color = black; // default: fill black

        x0 = atoi(strtok(NULL, " \n"));
        y0 = atoi(strtok(NULL, " \n"));
        s = strtok(NULL, " \n");
        if (s != NULL) {
            if (strcmp(s, "gray") == 0)
                color = gray;
            else if (strcmp(s, "silver") == 0)
                color = silver;
            else if (strcmp(s, "dim") == 0)
                color = dim;
            else if (strcmp(s, "white") == 0)
                color = white;
        }
        fill(x0, y0, color);
        return 0;
    }

    if (strcmp(s, "save") == 0) {
        s = strtok(NULL, " ");
        saveHist(s);
        printf("saved as \"%s\"\n", s);
        return 1;
    }

    if (strcmp(s, "load") == 0) {
        s = strtok(NULL, " ");
        load_history(s);
        return 1;
    }

    if (strcmp(s, "hist") == 0 || strcmp(s, "history") == 0) {
        s = strtok(NULL, " ");
        if (s == NULL) {
            printf("History Command\n"
                   "Utility to manage command history\n"
                   "\n"
                   "Usage: hist[ory] <verb>, where <verb> is as follows:\n"
                   "\n"
                   "\tlist\t(List all objects)\n"
                   "\tdelete\t(Delete specific object\n"
                   "\n");
        } else if (strcmp(s, "list") == 0) {
            show_history(begin);
        } else if (strcmp(s, "delete") == 0) {
            if ((s = strtok(NULL, " ")) != NULL) {
                const int num = atoi(s);
                begin = remove_num(begin, num);
                refresh();
//              saveHist("hist.tmp");
//              begin = remove_all(begin);
//              init_canvas();
//              load_history("hist.tmp");
//              remove("hist.tmp");
            } else {
                return EOF;
            }
        }
        return 1;
    }

    if (strcmp(s, "export") == 0 || strcmp(s, "exp") == 0) {
        s = strtok(NULL, " ");
        int res;
        if (s == NULL) {
            export2Pbm(1024);
        } else if (strcmp(s, "-h") == 0 || strcmp(s, "--help") == 0) {
            printf("Export Command\n"
                   "Utility to export drawing in a high resolution\n"
                   "\n"
                   "Usage: export <resolution>\n"
                   "\n"
                   "\tresolution\t(default resolution is 1024)\n"
                   "\n");
        } else if ((res = atoi(s)) >= 640) {
            export2Pbm(res);
        } else {
            printf("resolution is too small or invalid value\n");
            return EOF;
        }
        return 1;
    }

    if (strcmp(s, "undo") == 0 || strcmp(s, "u") == 0) {
        init_canvas();
        begin = pop_back(begin);
        Node *p;
        for (p = begin; p != NULL; p = p->next) {
            interpret_command(p->str);
        }
        return 1;
    }

    if (strcmp(s, "clear") == 0) {
        begin = remove_all(begin);
        refresh();
        return 1;
    }

    if (strcmp(s, "quit") == 0 || strcmp(s, "exit") == 0 || strcmp(s, "q") == 0) {
        return 2;
    }

    printf("error: unknown command.\n");

    return EOF;
}

double dist_line_dot(const int x0, const int y0, const int x1, const int y1, const int xa, const int ya, const double weight);
void pbmLine(char *pixel, const int x0, const int y0, const int x1, const int y1, const color_t color, const double weight);
void pbm_curl(char *pixel, const int x0, const int y0, const int radius, const double dig0, const double dig1, const color_t color, const double weight);



int main(void)
{
    const char *filename = "canvas.txt";
    char buf[BUFSIZE];

    if ((canvasfp = fopen(filename, "a")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return 1;
    }

    Node *p;
    init_canvas();
    print_canvas(canvasfp);

    while (1) {

        int hsize = 0;
        for (p = begin; p != NULL; p = p->next) hsize++;
        printf("simple_paint %3d > ", hsize);

        if (fgets(buf, BUFSIZE, stdin) == NULL) break;

        const int r = interpret_command(buf);
        if (r == 2) break;
        if (r == 0) {
            begin = push_back(begin, buf);
        }
        if (r == EOF) {
            printf("something went wrong\n");
        }

        print_canvas(canvasfp);
    }

    fclose(canvasfp);

    return 0;
}

void load_history(const char *filename)
{
    if (filename == NULL)
        filename = default_history_file;

    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr , "error: cannot open %s.\n", filename);
        return;
    }

    char buf[BUFSIZE];

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        interpret_command(buf);
        begin = push_back(begin, buf);
        print_canvas(canvasfp);
    }

    fclose(fp);
}

void refresh(void)
{
    init_canvas();
    Node *p;
    for (p = begin; p != NULL; p = p->next) {
        interpret_command(p->str);
    }
}

double dist_line_dot(const int x0, const int y0, const int x1, const int y1, const int xa, const int ya, const double weight)
{
    if ((MIN(x0, x1) - weight / 2 <= xa && MAX(x0, x1) + weight / 2 >= xa) &&
        (MIN(y0, y1) - weight / 2 <= ya && MAX(y0, y1) + weight / 2 >= ya)   ) {
        //IOLog1("%lf ", fabs((double)((x1 - x0) * (ya - y0) - (y1 - y0) * (xa - x0))) / sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2)));
        return fabs((double)((x1 - x0) * (ya - y0) - (y1 - y0) * (xa - x0))) / sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
    } else {
        return MIN(sqrt( pow(xa - x0, 2) + pow(ya - y0, 2) ), sqrt( pow(xa - x1, 2) + pow(ya - y1, 2) ));
    }
}
void pbmLine(char *pixel, const int x0, const int y0, const int x1, const int y1, const color_t color, const double weight)
{
    int resolution = 1024;
    for (int i = MAX(0, MIN(x0, x1) - weight); i < MIN(resolution, MAX(x0, x1) + weight); i++) {
        for (int j = MAX(0, MIN(y0, y1) - weight); j < MIN(resolution, MAX(y0, y1) + weight); j++) {
//          if (i == 20 && j == 31) {
//              IOLog1("[[[20, 31]]]->%lf\n", dist_line_dot(x0, y0, x1, y1, i, j, weight));
//          }
            if (weight / 2 >= dist_line_dot(x0, y0, x1, y1, i, j, weight)) {
                //IOLog1("[%d %d] ", i, j);
                switch (color) {
                    case black:
                        pixel[i + j * resolution] = 1;
                        break;
                    case white:
                        pixel[i + j * resolution] = 0;
                        break;
                    case gray:
                        pixel[i + j * resolution] = 2;
                        break;
                    case silver:
                        pixel[i + j * resolution] = 3;
                        break;
                    case dim:
                        pixel[i + j * resolution] = 4;
                        break;
                    default:
                        printf("pbmLine:UNKNOWN_COLOR\n");
                        exit(1);
                }
            }
        }
    }
}

void pbm_curl(char *pixel, const int x0, const int y0, const int radius, const double dig0, const double dig1, const color_t color, const double weight)
{
    int resolution = 1024;
    const double rdig0 = dig0 / 180.0 * M_PI;
    const double rdig1 = dig1 / 180.0 * M_PI;

    for (int i = MAX(0, x0 - radius - weight); i <= MIN(x0 + radius + weight, resolution - 1); i++) {
        for (int j = MAX(0, y0 - radius - weight); j <= MIN(y0 + radius + weight, resolution - 1); j++) {
            //double cdig = fmod(atan((double)(j - y0) / (i - x0)) + 2 * M_PI, M_PI);
            double cdig = acos((double)(i - x0) / (sqrt(pow(i - x0, 2) + pow(j - y0, 2))));
            if (-(j - y0) <= 0) // many people feels default y-axis is upside down.
                cdig = 2 * M_PI - cdig;
            if ((int)sqrt(pow(i - x0, 2) + pow(j - y0, 2)) >= radius - weight / 2 &&
                (int)sqrt(pow(i - x0, 2) + pow(j - y0, 2)) <= radius + weight / 2    ) {
                if (cdig >= rdig0 && cdig <= rdig1) {
                    //IOLog1("[cdig%lf] ", cdig);
                    pixel[i + j * resolution] = (color == white) ? 0 : 1;
                }
            }
        }
    }
}

void colorlizePixel(FILE *fp, char *pixel)
{
    int resolution = 1024;
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            switch (pixel[i * resolution + j]) {
                case 1:
                    fputc('1', fp);
                    break;
                case 0:
                    fputc('0', fp);
                    break;
                case 2: // gray
                    if (i % 2 == 0) {
                        if (j % 2 == 0)
                            fputc('1', fp);
                        else
                            fputc('0', fp);
                    } else {
                        if (j % 2 != 0)
                            fputc('1', fp);
                        else
                            fputc('0', fp);
                    }
                    break;
                case 3:
                    switch (i % 3) {
                        case 0:
                            if (j % 6 == 0 || j % 6 == 4)
                                fputc('1', fp);
                            else
                                fputc('0', fp);
                            break;
                        case 1:
                            if (j % 6 == 2 || j % 6 == 5)
                                fputc('1', fp);
                            else
                                fputc('0', fp);
                            break;
                        case 2:
                            if (j % 6 == 1 || j % 6 == 3)
                                fputc('1', fp);
                            else
                                fputc('0', fp);
                            break;
                    }
                    break;
                case 4:
                    switch (i % 3) {
                        case 0:
                            if (j % 6 == 0 || j % 6 == 4)
                                fputc('0', fp);
                            else
                                fputc('1', fp);
                            break;
                        case 1:
                            if (j % 6 == 2 || j % 6 == 5)
                                fputc('0', fp);
                            else
                                fputc('1', fp);
                            break;
                        case 2:
                            if (j % 6 == 1 || j % 6 == 3)
                                fputc('0', fp);
                            else
                                fputc('1', fp);
                            break;
                    }
                    break;
                default:
                    IOLog1("Not supported color output\n");
            }
            if (j != resolution - 1)
                fputc(' ', fp);
        }
        fputc('\n', fp);
    }
}

void export2Pbm(const int resolution)
{
    FILE *pbmfp;
    if ((pbmfp = fopen("mypict.pbm", "w")) == NULL) {
        perror("mypict.pbm");
        return ;
    }
    fprintf(pbmfp, "P1\n"
                   "%d %d\n", resolution, resolution);
    fflush(pbmfp);
    const double weight = 3.;
    char *pixel = malloc(sizeof(char) * resolution * resolution);

    /* init */
    for (int i = 0; i < resolution * resolution; i++)
        pixel[i] = 0;

    for (Node *p = begin; p != NULL; p = p->next) {
        char buf[BUFSIZE];
        strcpy(buf, p->str); // command is to be copied to buf cause strtok destorys source char string.
        char *s = strtok(buf, " ");

        if (s[0] == '#') {
            /* comment out: nothing to do */
        } else if (strcmp(s, "line") == 0) {
            const int x0 = atoi(strtok(NULL, " "));
            const int y0 = atoi(strtok(NULL, " "));
            const int x1 = atoi(strtok(NULL, " "));
            const int y1 = atoi(strtok(NULL, " "));
            color_t color = black;
            s = strtok(NULL, " \n");
            if (s != NULL) {
                if (strcmp(s, "white") == 0) {
                    color = white;
                } else if (strcmp(s, "black") == 0) {
                    color = black;
                } else if (strcmp(s, "gray") == 0)
                    color = gray;
                else if (strcmp(s, "silver") == 0)
                    color = silver;
                else if (strcmp(s, "dim") == 0)
                    color = dim;
            }
            pbmLine(pixel, x0, y0, x1, y1, color, weight);

        } else if (strcmp(s, "circle") == 0) {
            int x0, y0, radius;
            color_t color = clear;
            x0 = atoi(strtok(NULL, " "));
            y0 = atoi(strtok(NULL, " "));
            radius = atoi(strtok(NULL, " "));
            s = strtok(NULL, " \n");
            if (s != NULL) {
                if (strcmp(s, "white") == 0) {
                    color = white;
                } else if (strcmp(s, "black") == 0) {
                    color = black;
                } else if (strcmp(s, "gray") == 0)
                    color = gray;
                else if (strcmp(s, "silver") == 0)
                    color = silver;
                else if (strcmp(s, "dim") == 0)
                    color = dim;
            }
            if (color == clear) {
                for (int i = MAX(0, x0 - radius - weight); i <= MIN(resolution, x0 + radius + weight); i++) {
                    for (int j = MAX(0, y0 - radius - weight); j <= MIN(resolution, y0 + radius + weight); j++) {
                        if (sqrt(pow(i - x0, 2) + pow(j - y0, 2)) >= radius - weight / 2 && 
                            sqrt(pow(i - x0, 2) + pow(j - y0, 2)) <= radius + weight / 2) {
                            pixel[i + j * resolution] = 1;
                        }
                    }
                }
            } else {
                for (int i = MAX(x0 - radius - weight, 0); i <= MIN(resolution - 1, x0 + radius + weight); i++) {
                    for (int j = MAX(y0 - radius - weight, 0); j <= MIN(resolution - 1, y0 + radius + weight); j++) {
                        if (sqrt(pow(i - x0, 2) + pow(j - y0, 2)) <= radius + weight / 2) {
                            pixel[i + j * resolution] = (color == black) ? 1 : 0;
                        }
                    }
                }
            }
        } else if (strcmp(s, "rect") == 0) {
            const int x0 = atoi(strtok(NULL, " "));
            const int y0 = atoi(strtok(NULL, " "));
            const int width = atoi(strtok(NULL, " "));
            const int height = atoi(strtok(NULL, " "));
            color_t color = clear;
            if (color == clear) {
                pbmLine(pixel, x0        , y0         , x0 + width, y0         , black, weight);
                pbmLine(pixel, x0        , y0         , x0        , y0 + height, black, weight);
                pbmLine(pixel, x0 + width, y0         , x0 + width, y0 + height, black, weight);
                pbmLine(pixel, x0 + width, y0 + height, x0        , y0 + height, black, weight);
            }
        } else if (strcmp(s, "curl") == 0) {
            int x0, y0, radius;
            double dig0, dig1;
            color_t color = black;
            x0 = atoi(strtok(NULL, " \n"));
            y0 = atoi(strtok(NULL, " \n"));
            radius = atoi(strtok(NULL, " \n"));
            dig0 = atof(strtok(NULL, " \n"));
            dig1 = atof(strtok(NULL, " \n"));
            s = strtok(NULL, " \n");
            if (s != NULL) {
                if (strcmp(s, "white") == 0)
                    color = white;
            }
            pbm_curl(pixel, x0, y0, radius, MIN(dig0, dig1), MAX(dig0, dig1), color, weight);
        } else if (strcmp(s, "fill") == 0) {
            int x0, y0;
            color_t color = black;
            x0 = atoi(strtok(NULL, " \n"));
            y0 = atoi(strtok(NULL, " \n"));
            s = strtok(NULL, " \n");
            if (s != NULL) {
                if (strcmp(s, "gray") == 0)
                    color = gray;
                else if (strcmp(s, "silver") == 0)
                    color = silver;
                else if (strcmp(s, "dim") == 0)
                    color = dim;
                else if (strcmp(s, "white") == 0)
                    color = white;
            }
            pbmFill(pixel, x0, y0, color);
        } else {
            printf("UNKNOWN_COMMAND_INCLUEDE\n");
        }
    }

    colorlizePixel(pbmfp, pixel);
//  for (int i = 0; i < resolution; i++) {
//      int j = 0;
//      fprintf(pbmfp, "%d", pixel[i * resolution + j]);
//      for (j = 1; j < resolution; j++) {
//          fprintf(pbmfp, " %d", pixel[i * resolution + j]);

//          /* color sample: gray */
//          if (i % 2 == 0) {
//              if (j % 2 == 0)
//                  fprintf(pbmfp, " %d", 1);
//              else
//                  fprintf(pbmfp, " %d", 0);
//          } else {
//              if (j % 2 != 0)
//                  fprintf(pbmfp, " %d", 1);
//              else
//                  fprintf(pbmfp, " %d", 0);
//          }
//      }
//      for (int j = 0; j < resolution; j++) {
//          fprintf(pbmfp, "%d", pixel[i * resolution + j]);
//          if (j != resolution - 1)
//              fputc(' ', pbmfp);
//      }
//      fputc('\n', pbmfp);
//  }
    free(pixel);
    fclose(pbmfp);
    return ;
}
