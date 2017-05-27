#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
  int id;
  char *name;
} Student;

int comp_id(const void *x0, const void *x1)
{
  const Student *p0 = x0; 
  const int y0 = p0->id;  

  const Student *p1 = x1;
  const int y1 = p1->id;

  if (y0 > y1) return  1;
  if (y0 < y1) return -1;
  return 0;
}

int comp_name(const void *x0, const void *x1)
{
    const Student *p0 = x0;
    const Student *p1 = x1;

    const char *str0 = p0->name;
    const char *str1 = p1->name;

    return strcmp(str0, str1);
}

int main()
{
  Student v[] = { 
    { 125, "John" },
    { 123, "Mary" },
    { 127, "James" },
    { 124, "David" },
  };
  
  const int n = sizeof(v) / sizeof(v[0]);

  //qsort(v, n, sizeof(v[0]), comp_id);
  qsort(v, n, sizeof(v[0]), comp_name); // Complete comp_name() and try this.

  int i;
  for (i = 0; i < n; i++) {
    printf("v[%d] = %d %s\n", i, v[i].id, v[i].name);
  }

  return 0;
}
