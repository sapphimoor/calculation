#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int BOARD_SHOW_OPT = 1;
int COORDINATE_SHOW_OPT = 1;
int NUMBER_SHOW_OPT = 1;

int count = 0;
int n;
int **Q;

void checkOptions(int, char **);
void sort(char **, int, int);
char *mid(char *, char *, char *);
void init();
void final();
void putQ(int);
void makeAnser();

int main(int argc, char **argv) {
  checkOptions(argc, argv);

  init();
  putQ(1);
  final();

  return 0;
}

void checkOptions(int argc, char **argv) {
  int argi = 0, opti = 0, optc = 6, tmp, i;
  char *opt[] = {"--board", "--coordinate", "--number", "-b", "-count", "-n"};
  char **myargv;

  if (argc == 1) {
    return;
  }

  myargv = (char **)malloc(sizeof(char *) * argc);
  for (i = 0; i < argc; i++) {
    myargv[i] = (char *)malloc(sizeof(char) * 20);
    strcpy(myargv[i], argv[i]);
  }

  sort(myargv, 0, argc - 1);

  BOARD_SHOW_OPT = 0;
  COORDINATE_SHOW_OPT = 0;
  NUMBER_SHOW_OPT = 0;
  while ((argi < argc) && (opti < optc)) {
    tmp = strcmp(myargv[argi], opt[opti]);
    if (!tmp) {
      switch (opti) {
      case 0:
      case 3:
        BOARD_SHOW_OPT = 1;
        break;

      case 1:
      case 4:
        COORDINATE_SHOW_OPT = 1;
        break;

      case 2:
      case 5:
        NUMBER_SHOW_OPT = 1;
        break;
      }
      argi++;
      opti++;
    } else if (tmp < 0) {
      argi++;
    } else {
      opti++;
    }
  }

  for (i = 0; i < argc; i++) {
    free(myargv[i]);
  }
  free(myargv);
}

void sort(char **s, int left, int right) {
  if (left < right) {
    int i = left, j = right;
    char tmp[20];
    char *pivot = mid(s[i], s[i + (j - i) / 2], s[j]);

    while (1) {
      while (strcmp(s[i], pivot) < 0) {
        i++;
      }
      while (strcmp(pivot, s[j]) < 0) {
        j--;
      }
      if (i >= j) {
        break;
      }

      strcpy(tmp, s[i]);
      strcpy(s[i], s[j]);
      strcpy(s[j], tmp);

      i++;
      j--;
    }
    sort(s, left, i - 1);
    sort(s, j + 1, right);
  }
}

char *mid(char *s1, char *s2, char *s3) {
  if (strcmp(s1, s2) < 0) {
    if (strcmp(s2, s3) < 0) {
      return s2;
    }
    if (strcmp(s1, s3) < 0) {
      return s3;
    }
    return s1;
  }
  if (strcmp(s2, s3) > 0) {
    return s2;
  }
  if (strcmp(s1, s3) > 0) {
    return s3;
  }
  return s1;
}

void init() {
  int i, j;

  printf("Input problem size: n = ");
  scanf("%d", &n);
  while (n <= 0) {
    printf("\n **Error: init: Input a number MORE THAN 0.\n");
    printf("Input problem size AGAIN: n = ");
    scanf("%d", &n);
  }
  printf("\n");

  Q = (int **)malloc(sizeof(int *) * (n + 2));
  for (i = 0; i <= n + 1; i++) {
    Q[i] = (int *)malloc(sizeof(int) * (n + 2));
    for (j = 0; j <= n + 1; j++) {
      Q[i][j] = 0;
    }
  }
}

void final() {
  int i;
  for (i = 0; i <= n + 1; i++) {
    free(Q[i]);
  }
  free(Q);
}

void putQ(int i) {
  int j, i2, j2;

  for (j = 1; j <= n; j++) {
    for (i2 = i - 1; i2 <= i + 1; i2++) {
      for (j2 = j - 1; j2 <= j + 1; j2++) {
        if (Q[i2][j2]) {
          break;
        }
      }
      if (j2 != j + 2) {
        break;
      }
    }
    if (j2 != j + 2) {
      continue;
    }

    for (i2 = 1; i2 < i; i2++) {
      if (Q[i2][j]) {
        break;
      }
    }
    if (i2 != i) {
      continue;
    }

    Q[i][j] = 1;
    if (i == n) {
      makeAnser();
      Q[i][j] = 0;

      if (n == 1) {
        break;
      }
      return;
    }

    putQ(i + 1);
    Q[i][j] = 0;
  }

  if (NUMBER_SHOW_OPT && i == 1) {
    printf("\nNumber of answers is %d\n", count);
  }
}

void makeAnser() {
  int i, j;

  if (COORDINATE_SHOW_OPT) {
    for (i = 1; i < n; i++) {
      for (j = 1; j <= n; j++) {
        if (Q[i][j]) {
          printf("(%d, %d), ", i, j);
          break;
        }
      }
    }
    for (j = 1; j <= n; j++) {
      if (Q[i][j]) {
        printf("(%d, %d)\n", i, j);
        break;
      }
    }
  }

  if (BOARD_SHOW_OPT) {
    for (i = 1; i <= n; i++) {
      for (j = 0; j < n; j++) {
        printf("+-");
      }
      printf("+\n");
      for (j = 1; j <= n; j++) {
        Q[i][j] ? printf("|o") : printf("| ");
      }
      printf("|\n");
    }
    for (j = 0; j < n; j++) {
      printf("+-");
    }
    printf("+\n\n");
  }

  count++;
}
