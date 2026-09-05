#include <iostream>

namespace company {
    namespace hr {
        namespace payroll {
            int calculate_salary(int age, int exp, int base) {
                base = 60000;
                return base + base/(50*age*1.2) + base*0.1*exp;
            }
        }
    }
}

int main() { 
    using namespace company::hr;
    std::cout << company::hr::payroll::calculate_salary(30,6,60000);
    return 0;
}