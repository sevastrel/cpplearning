#include <iostream>
#include <map>
#include <string>

// class word {
// private:
//     std::string rus;
//     std::string eng;
// public:
//     word(std::string new_rus, std::string new_eng) {
//         rus=new_rus;
//         eng=new_eng;
//     }
//     void set trans
int main() {
    std::map<std::string, std::string> dict;
    dict["молоко"]="milk";
    dict["хлеб"]="bread";
    dict["яблоко"]="apple";
    std::string word;
    std::cout << "Какое слово? ";
    std::cin >> word;
    if (dict.find(word)!=dict.end()) {
        std::cout << "Перевод: " << dict[word] << "\n";
    }
    else {
        std::cout << "Слово не найдено\n";
    }
}