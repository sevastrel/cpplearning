#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    std::ifstream in("input1.txt");
    if (!in.is_open()) {
        std::cout << "Error with read file!\n";
        return 1;
    }

    size_t count_lines=0;
    size_t count_words=0;
    size_t count_letters=0;
    std::string line;
    std::string word;
    while (std::getline(in, line)) {
        count_lines+=1;
        count_letters+=line.size();
        std::istringstream line_stream(line);
        while (line_stream >> word) {
            count_words+=1;
        }
    }
    
    std::cout << "Count of lines: " << count_lines <<"\n";
    std::cout << "Count of words: " << count_words <<"\n";
    std::cout << "Count of letter: " << count_letters <<"\n";
    return 0;
}