#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cctype>

int main() {
    std::string str;
    std::cout << "Введите строку: ";
    std::getline(std::cin,str);
    std::map<char, int> symbols;
    for (char sym : str) {
        if (!std::isspace(sym) && !std::ispunct(sym)) {
            symbols[sym]++;
        }
    }
    std::ofstream out("output.csv");
    if (!out.is_open()) {
        std::cerr << "Error: file cannot open";
        return 1;
    }
    out<<"letter,frequency\n";
    for (const auto& sym : symbols) {
        std::cout << sym.first << ": " 
         << sym.second << "\n";
        out << sym.first << ',' <<sym.second<<'\n';
    }
    return 0; 
}