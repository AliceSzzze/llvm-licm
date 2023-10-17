#include <stdio.h>

int f(int arg) {
    int l = arg;

        for (int j = 0; j < arg; j++) {

            int k = 8;
            l = k;
        }
    return l;
}

int main() {
    int l = 29;
    // for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {

            int k = 8;
            l = k;
        }
    // }
    printf("The value of the number is: %d\n", l);
    
    return f(l);
}



