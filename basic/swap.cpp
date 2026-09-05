#include <iostream>
void swap(int &a,int &b);
int main() {
    int a=5;
    int b=10;
    swap(a,b);
    std::cout << "a=" << a << "\nb=" << b;

}

void swap(int &a, int &b) {
    int temp = a;
    a=b;
    b=temp;
}