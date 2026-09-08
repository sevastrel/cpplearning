#include <iostream>
#include <string.h>
#include <fstream>

int main() {
    std::cout << "hello";
    std::ifstream in("input.txt");
    if (!in.is_open()) {
        std::cerr << "Ошибка открытия файла!\n";
        return 1;
    }
    char letter=0;
    size_t count=0;
    while (in.get(letter)) {
        count+=1;
    }
    std::cout << "\nCount of letters: " << count <<"\n";
    in.close();
    return 0;
}