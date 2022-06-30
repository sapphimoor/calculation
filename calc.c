#include <stdio.h>

#define N 100
#define CSIZE 16

typedef enum {
  LPAREN,
  NUMBER_LIKE,
  OPERATOR
} OBJ;

typedef struct {
  int dnm, nmr;
} FRC;

typedef struct {
  char stack[N][CSIZE];
  int sp;
} POR;

typedef struct {
  char stack[N];
  int sp;
} OSTK;

typedef struct {
  FRC stack[N];
  int sp;
} STK;

int RPN_SHOW_OPT = 0;

FRC ERROR_FRC = {0, 0};
OBJ prePushedObj = LPAREN;
POR por;
OSTK ostack;
STK stack;

void checkOptions(int, char**);
int strcmp(char*, char*);
FRC newFrc(int, int);
FRC frcPlus(FRC, FRC);
FRC frcMinus(FRC, FRC);
FRC frcMul(FRC, FRC);
FRC frcDiv(FRC, FRC);
FRC nrml(FRC);
int eucl(int, int);
int isZero(FRC);
int isInt(FRC);
void printAns(FRC);
int revp(char);
int calc(char *);
FRC power(FRC, int);
FRC fact(int);
FRC perm(int, int);
FRC comb(int, int);
int ppush(char *);
void ppop(char *);
int opush(char);
char opop();
int spush(FRC);
FRC spop();

int main(int argc, char **argv) {
  int i, j, b = 0, e;
  char x[N];

  checkOptions(argc, argv);

  por.sp = 0;
  ostack.sp = 0;
  stack.sp = 0;

  printf("Input calculation fomura:\n  ");

  if (revp(getchar())) {
    return -1;
  }

  x[1] = '\0';
  while (ostack.sp) {
    x[0] = opop();
    ppush(x);
  }

  if (RPN_SHOW_OPT) {
    printf("\nReverse polish notation is: \n");
    for (i = 0; i < por.sp; i++) {
      printf(" %s", por.stack[i]);
    }
    printf("\n\n");
  }

  for (i = 0; i < por.sp; i++) {
    if (calc(por.stack[i])) {
      return -1;
    }
  }

  printAns(spop());

  return 0;
}

void checkOptions(int argc, char **argv) {
  if (argc == 2 && (!strcmp(argv[1], "-r") || !strcmp(argv[1], "--rpn"))) {
    RPN_SHOW_OPT = 1;
  }
}

int strcmp(char *a, char *b) {
  int i;
  for (i = 0; a[i] != '\0' && b[i] != '\0'; i++) {
    if (a[i] != b[i]) {
      break;
    }
  }
  return a[i] - b[i];
}

FRC newFrc(int d, int n) {
  FRC frc = {d, n};
  return nrml(frc);
};

FRC nrml(FRC a) {
  int gcd;
  if (a.nmr == 1) {
    return a;
  }
  if (a.dnm == 0) {
    a.nmr = 1;
    return a;
  }
  if (a.nmr < 0) {
    a.dnm = -a.dnm;
    a.nmr = -a.nmr;
  }

  gcd = eucl(a.dnm, a.nmr);
  a.dnm = a.dnm / gcd;
  a.nmr = a.nmr / gcd;
  return a;
};

int eucl(int x, int y) {
  int tmp;
  while (y) {
    tmp = y;
    y = x % y;
    x = tmp;
  }
  return x;
};

FRC frcPlus(FRC a, FRC b) {
  return newFrc(a.dnm * b.nmr + a.nmr * b.dnm, a.nmr * b.nmr);
};

FRC frcMinus(FRC a, FRC b) {
  return newFrc(a.dnm * b.nmr - a.nmr * b.dnm, a.nmr * b.nmr);
};

FRC frcMul(FRC a, FRC b) { return newFrc(a.dnm * b.dnm, a.nmr * b.nmr); };

FRC frcDiv(FRC a, FRC b) { return newFrc(a.dnm * b.nmr, a.nmr * b.dnm); };

int isZero(FRC frc) { return frc.dnm == 0 ? 1 : 0; };

int isInt(FRC frc) { return nrml(frc).nmr == 1 ? 1 : 0; };

void printAns(FRC frc) {
  if (frc.nmr == 1) {
    printf("= %d\n", frc.dnm);
  } else {
    printf("= %d/%d (%f)\n", frc.dnm, frc.nmr, (double)frc.dnm / frc.nmr);
  }
};

int revp(char x) {
  int i = 0, flag = 0;
  char dummy[CSIZE];

  if (x == '=' || x == 10) { // 10 == Enter-key
    return 0;
  }
  while (x == ' ') {
    x = getchar();
  }

  if (x == '+' || x == '-') {
    dummy[1] = '\0';
    while (ostack.sp) {
      dummy[0] = opop();
      if (dummy[0] == '(') {
        opush(dummy[0]);
        break;
      } else if (dummy[0] == '*' || dummy[0] == 47 || dummy[0] == '^' ||
                  dummy[0] == 'P' || dummy[0] == 'C') {

        if (ppush(dummy)) {
          return -1;
        }
      } else if (dummy[0] == 'm' || dummy[0] == '+' || dummy[0] == '-') {
        if (ppush(dummy)) {
          return -1;
        }
        break;
      }
    }
    if (prePushedObj == LPAREN) {
      if (x == '-') {
        if (opush('m')) {
          return -1;
        }
        prePushedObj = OPERATOR;
      }
    } else if (prePushedObj == NUMBER_LIKE) {
      if (opush(x)) {
        return -1;
      }
      prePushedObj = OPERATOR;
    } else {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
  } else if (x == '*' || x == 47) { // 47 == '/'
    dummy[1] = '\0';
    while (ostack.sp) {
      dummy[0] = opop();
      if (dummy[0] == 'm' || dummy[0] == '+' || dummy[0] == '-' || dummy[0] == '(') {
        opush(dummy[0]);
        break;
      } else if (dummy[0] == '^' || dummy[0] == 'P' || dummy[0] == 'C') {
        if (ppush(dummy)) {
          return -1;
        }
      } else if (dummy[0] == '*' || dummy[0] == 47) {
        if (ppush(dummy)) {
          return -1;
        }
        break;
      }
    }
    if (prePushedObj == NUMBER_LIKE) {
      if (opush(x)) {
        return -1;
      }
      prePushedObj = OPERATOR;
    } else {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
  } else if (x == '(') {
    if (prePushedObj != NUMBER_LIKE) {
      if (opush(x)) {
        return -1;
      }
      prePushedObj = LPAREN;
    } else {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
  } else if (x == ')') {
    dummy[1] = '\0';
    while ((dummy[0] = opop()) != '(') {
      if (ppush(dummy)) {
        return -1;
      }
    }
    if (ostack.sp) {
      dummy[0] = opop();
      if (dummy[0] == 'm') {
        if (ppush(dummy)) {
          return -1;
        }
      } else {
        opush(dummy[0]);
      }
    }
    prePushedObj = NUMBER_LIKE;
  } else if (x == '^' || x == 'P' || x == 'C') {
    if (prePushedObj == NUMBER_LIKE) {
      if (opush(x)) {
        return -1;
      }
      prePushedObj = OPERATOR;
    } else {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
  } else if (x == '!') {
    if (prePushedObj == NUMBER_LIKE) {
      if (ppush("!")) {
        return -1;
      }
    } else {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
  } else if (('0' <= x && x <= '9') || x == '.') {
    while (i < CSIZE && '0' <= x && x <= '9') {
      dummy[i++] = x;
      x = getchar();
    }
    if (i == CSIZE && '0' <= x && x <= '9') {
      printf("\033[31m **Error: Detect too large number.\n");
      return -1;
    }
    if (x == '.') {
      if (!i) {
        dummy[i++] = '0';
      }
      if (i < CSIZE) {
        dummy[i++] = x;
      }
      x = getchar();
      while (i < CSIZE && '0' <= x && x <= '9') {
        dummy[i++] = x;
        x = getchar();
      }
      if (i == CSIZE) {
        while ('0' <= x && x <= '9') {
          x = getchar();
        }
        i--;
      }
      if (dummy[i - 1] == '.') {
        i--;
      }
    }
    dummy[i] = '\0';
    if (prePushedObj != NUMBER_LIKE) {
      if (ppush(dummy)) {
        return -1;
      }
      prePushedObj = NUMBER_LIKE;
    } else {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    if (revp(x)) {
      return -1;
    }
    return 0;
  } else {
    printf("\033[31m **Error: Incorrect calculation formula.\n");
    return -1;
  }

  if (revp(getchar())) {
    return -1;
  }

  return 0;
}

int calc(char *x) {
  int i = 0, n = 0, m = 1, flag = 0;
  FRC a, b;

  if (x[0] == 'm') {
    if (stack.sp < 1) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    a = spop();
    if (spush(frcMinus(newFrc(0, 1), a))) {
      return -1;
    }
  } else if (x[0] == '+') {
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (spush(frcPlus(a, b))) {
      return -1;
    }
  } else if (x[0] == '-') {
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (spush(frcMinus(a, b))) {
      return -1;
    }
  } else if (x[0] == '*') {
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (spush(frcMul(a, b))) {
      return -1;
    }
  } else if (x[0] == 47) { // 47 == '/'
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (isZero(b)) {
      printf("\033[31m **Error: Cannot devine by 0.\n");
      return -1;
    }
    if (spush(frcDiv(a, b))) {
      return -1;
    }
  } else if (x[0] == '^') {
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (!isInt(b)) {
      printf("\033[31m **Error: Cannot calculation a fractional power.\n");
      return -1;
    }
    if (!(a = power(a, b.dnm)).nmr) {
      return -1;
    }
    if (spush(a)) {
      return -1;
    }
  } else if (x[0] == '!') {
    if (!stack.sp) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    a = spop();
    if (!isInt(a)) {
      printf("\033[31m **Error: Cannot calculation the factorial of fraction.\n");
      return -1;
    }
    if (a.dnm < 0) {
      printf("\033[31m **Error: Cannot calculation the factorial of minus value\n");
      return -1;
    }
    if (spush(fact(a.dnm))) {
      return -1;
    }
  } else if (x[0] == 'P') {
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (!isInt(a) || !isInt(b)) {
      printf("\033[31m **Error: Cannot calculate the permutation of fractions.\n");
      return -1;
    }
    if (a.dnm < b.dnm || b.dnm < 0) {
      printf("\033[31m **Error: Cannot calculate the permutation.\n");
      return -1;
    }
    if (spush(perm(a.dnm, b.dnm))) {
      return -1;
    }
  } else if (x[0] == 'C') {
    if (stack.sp < 2) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return -1;
    }
    b = spop();
    a = spop();
    if (!isInt(a) || !isInt(b)) {
      printf("\033[31m **Error: Cannot calculate the combination of fractions.\n");
      return -1;
    }
    if (a.dnm < b.dnm || b.dnm < 0) {
      printf("\033[31m **Error: Cannot calculate the combination.\n");
      return -1;
    }
    if (spush(comb(a.dnm, b.dnm))) {
      return -1;
    }
  } else {
    while (i < CSIZE && x[i] != '.' && x[i] != '\0') {
      n = n * 10 + x[i++] - '0';
    }
    if (x[i] == '.') {
      i++;
      while (i < CSIZE && x[i] != '\0') {
        n = n * 10 + x[i++] - '0';
        m *= 10;
      }
    }
    if (spush(newFrc(n, m))) {
      return -1;
    }
  }

  return 0;
}

FRC power(FRC x, int n) {
  FRC ans = newFrc(1, 1);

  if (n < 0) {
    if (isZero(x)) {
      printf("\033[31m **Error: Incorrect calculation formula.\n");
      return ERROR_FRC;
    }
    while (n++) {
      ans = frcDiv(ans, x);
    }
  } else {
    while (n--) {
      ans = frcMul(ans, x);
    }
  }

  return ans;
}

FRC fact(int n) {
  double ans = 1;
  while (n) {
    ans *= n--;
  }
  return newFrc(ans, 1);
}

FRC perm(int n, int r) {
  int ans = 1;
  while (r--) {
    ans *= n--;
  }
  return newFrc(ans, 1);
}

FRC comb(int n, int r) {
  int ans = 1, div = 1;
  r = (n - r) < r ? n - r : r;
  while (r--) {
    ans = ans * n-- / div++;
  }
  return newFrc(ans, 1);
}

int ppush(char *x) {
  int i;
  if (por.sp == N) {
    printf("\033[31m **Error: PStack is FULL\n");
    return -1;
  }

  for (i = 0; i < CSIZE; i++) {
    por.stack[por.sp][i] = x[i];
    if (x[i] == '\0') {
      break;
    }
  }
  por.sp++;

  return 0;
}

void ppop(char *x) {
  int i;
  por.sp--;
  for (i = 0; i < CSIZE; i++) {
    x[i] = por.stack[por.sp][i];
    if (x[i] == '\0') {
      break;
    }
  }

  return;
}

int opush(char c) {
  if (ostack.sp == N) {
    printf("\033[31m **Error: OStack is FULL\n");
    return -1;
  }
  ostack.stack[ostack.sp++] = c;

  return 0;
}

char opop() { return ostack.stack[--ostack.sp]; }

int spush(FRC a) {
  if (stack.sp == N) {
    printf("\033[31m **Error: SStack is FULL!\n");
    return -1;
  }
  stack.stack[stack.sp++] = a;

  return 0;
}

FRC spop() { return stack.stack[--stack.sp]; }
