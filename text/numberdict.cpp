#include <iostream>
#include <fstream>
#include <string>
#include <map>

class Contact {
private:
    std::string number;
    std::string person;
public:
    Contact() {
        number = "xxxxxxxx";
        person = "unknown";
    }

    void set_number(std::string new_number) {
        number=new_number;
    }
    void set_person(std::string new_person) {
        person=new_person;
    }
    std::string get_number() const {
        return number;
    }

    std::string get_person() const {
        return person;
    }
    void print() const {
        std::cout << "Name: " << person << "\n";
        std::cout << "Number: " << number << "\n";
    }

};

int main(){
    Contact n1;
    n1.set_number("8914479");
    n1.set_person("Danila");
    std::cout << "Name: " << n1.get_person() << "\n";
    std::cout << "Phone: " << n1.get_number() << "\n";
    n1.print();
    return 0;


}