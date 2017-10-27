/*
 * Heap's Algorithm
 * heapsAlg.c
 * 2017-10-25
 *
 */

#include "./heapsAlg.h"

/***************************************************************************
 * Heap's Algorithm for permuting a set;
 * =====================================
 * n is the size of the current data set; the data set must be a list of
 * strings, with the final element in the set a NULL value.  This allows for
 * variable-sized sets to be sent in.
 * Takes a function pointer as the final argument; this function makes a
 * call with the current permutation as its argument.  In the normal
 * this can be a call to the display() function to print out the permutation,
 * or it can be to a function that peforms some kind of calculation on the
 * current permutation.
 ***************************************************************************/
void
permute(int n, char **s, void (*fn)(char **)) {
  if (n == 1) {
    (*fn)(s);
  }

  else {
    for (int i = 0; i < n - 1; i++) {

      permute(n - 1, s, fn);

      swap( isodd(n) ? 0 : i, n - 1, s);
    }
    permute(n - 1, s, fn);
  }
}
/***************************************************************************/

void
swap(int a, int b, char **s) {
  char *t = s[a];
  s[a] = s[b];
  s[b] = t;
}

int
isodd(int a) {
  return (int)fmod((double)a, 2.0);
}

/* this will  return the number of  elements of both rows  and columns
 * provided the size represents the  size of an element, i.e., MAXSIZE
 * (10) for rows and 1 for columns */
int
elements(char *s[], int size) { /* size represents the max size of an element */
  int i = 0;
  while (s[i++ * size] != NULL)
    ;
  return i - 1;
}

void
display(char *s[]) {
  int size = elements(s, 1);

  printf("[");
  for (int i = 0; i < size; i++)
    printf("%s%s", i>0?", ":"",s[i]);
  printf("]\n");
}