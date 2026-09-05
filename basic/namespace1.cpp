#include <iostream>

namespace calc_int {
    int sum(int a, int b) {
        return a+b;
    }
}
namespace calc_double {
    double sum(double a, double b) {
        return a+b;
    }
}

int main(){
    std::cout << calc_int::sum(5,67)<<'\n';
    std::cout << calc_double::sum(12.456,567.32)<<'\n';
}