#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    std::map<std::string, int> candidates;
    std::vector<std::string> votes = {"Alice", "Bob", "Charlie", "Bill", "Alice"};
    for (std::string& vote : votes) {
        candidates[vote]+=1;
    }
    for (const auto& candidat: candidates) {
        std::cout << "Candidat: " << candidat.first 
        <<"\tvotes: " << candidat.second << "\n";
    }
}