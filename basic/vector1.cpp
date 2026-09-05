#include <iostream>
#include <vector>

int sum(std::vector<int> v);
double avg(std::vector<int> v);

int main() {
    int N;
    std::cout << "Enter the number of numbers: ";
    std::cin >> N;
    std::vector<int> v(N);
    std::cout << "Текущий размер вектора: " << v.capacity() << '\n';
    for (int i=0; i<N; i++) {
        std::cin >> v[i];
    }

    // for (int x : v) {
    //     std::cout << x << " ";
    // }
    std::cout << "Сумма: " << sum(v) << "\n";
    std::cout << "Среднее значение: " << avg(v);
    return 0;
}

int sum(std::vector<int> v) {
    int sum = 0;
    for (int x : v){
        sum+=x;
    }
    return sum;

}

double avg(std::vector<int> v) {
    double sum = 0;
    for (int x : v) {
        sum+=x;
    }
    return sum/v.size();
}