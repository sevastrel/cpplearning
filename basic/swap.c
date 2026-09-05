#include <stdio.h>
void swap (int *a, int *b);

int main () {
    int a=5;
    int b=10;
    printf("before swap a=%d\nb=%d",a,b);

    swap(&a,&b);
    printf("after swap a=%d\nb=%d",a,b);
    return 0;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}