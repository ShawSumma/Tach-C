#include <stdio.h>

int fib(int num) {
    if (num < 2) return num;
    return fib(num-2) + fib(num-1);
}

int main(int argc, char **argv) {
    printf("%d\n", fib(42));
}
