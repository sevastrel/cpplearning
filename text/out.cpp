#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "Введите N: ";
    int N;
    std::cin >> N;
    std::ofstream out("out.csv");
    if (!out.is_open()) {
        std::cerr << "Error: the file cannot be openned";
    }
    out << "x,x^2,x^3" <<'\n';
    for (int i=0; i<=N; i++) {
        out << i << ','
        << i*i <<','
        << i*i*i <<'\n';
    }
    out.close();
    std::cout << "Запись завершена\n";
}