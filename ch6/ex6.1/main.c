/*
 * Exercise 6-1. Our version of getword does not properly handle underscores,
 * string constants, comments, or preprocessor control lines. Write a better
 * version.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXWORD 100
#define BUFSIZE 100
#define NKEYS (sizeof keytab / sizeof(struct key))

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

struct key {
  char *word;
  int count;
} keytab[] = {"auto", 0, "break", 0, "case", 0, "char", 0, "const", 0,
              "continue", 0, "default", 0,
              /* ... */
              "unsigned", 0, "void", 0, "volatile", 0, "while", 0};

int getword(char *, int);
int binsearch(char *, struct key *, int);
int getch(void);
void ungetch(int);
int comment(void);

/* getword: get next word or character from input */
int getword(char *word, int lim) {
  int c, d, getch(void);
  void ungetch(int);
  char *w = word;

  while (isspace(c = getch()))
    ;
  if (c != EOF)
    *w++ = c;
  if (isalpha(c) || c == '_' || c == '#') {
    for (; --lim > 0; w++) {
      if (!isalnum(*w = getch()) && *w != '_') {
        ungetch(*w);
        break;
      }
    }
  } else if (c == '\'' && c == '"') {
    for (; --lim > 0; w++) {
      if ((*w = getch()) == '\\') {
        *++w = getch();
      } else if (*w == c) {
        w++;
        break;
      } else if (*w == EOF) {
        break;
      } else if (c == '/') {
        if ((d = getch()) == '*') {
          c = comment();
        } else {
          ungetch(d);
        }
      }
    }
  }
  *w = '\0';
  return c;
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n) {
  int cond;
  int low, high, mid;
  low = 0;
  high = n - 1;
  while (low <= high) {
    mid = (low + high) / 2;
    if ((cond = strcmp(word, tab[mid].word)) < 0)
      high = mid - 1;
    else if (cond > 0)
      low = mid + 1;
    else
      return mid;
  }
  return -1;
}

/* getch: get a (possibly pushed back) character */
int getch(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }

/* ungetch: push character back to input */
void ungetch(int c) {
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

/* comment: skip over commnet and return a character */
int comment() {
  int c;
  while ((c = getch()) != EOF) {
    if (c == '*') {
      if ((c = getch()) == '/') {
        break;
      } else {
        ungetch(c);
      }
    }
  }
  return c;
}

/* count C keywords */
int main() {
  int n;
  char word[MAXWORD];
  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0]))
      if ((n = binsearch(word, keytab, NKEYS)) >= 0)
        keytab[n].count++;
  for (n = 0; n < NKEYS; n++)
    if (keytab[n].count > 0)
      printf("%4d %s\n", keytab[n].count, keytab[n].word);
  return 0;
}
