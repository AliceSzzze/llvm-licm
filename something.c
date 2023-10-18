#include <stdio.h>

void squareIt(int n) {
    n = n * n;
}

int main() {
  int a = 9;
  int b = 5;
  for (int i = 0; i < 10; i++) {
    int c = a * b;
    squareIt(c);
    printf("%i\n", c);
  }
  return 0;
}


