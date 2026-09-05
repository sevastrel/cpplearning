#include <iostream>
#include <string>

int main() {
    std::string name;
    int age=0;
    std::cout << "Hello, what's your name? ";
    std::cin >> name;
    std::cout << "How old are you? ";
    std::cin >> age;
    std:: cout << "Glad to see you " << name <<"\nYour age is " <<age <<"\n";
}