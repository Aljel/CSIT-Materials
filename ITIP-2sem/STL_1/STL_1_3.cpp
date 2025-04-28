#include <iostream>
#include <vector>
#include <numeric>   // для accumulate
#include <cmath>     // для log, exp
#include <functional> // для multiplies
using namespace std;

// Функция для вычисления среднего арифметического
double Arithmetic(const vector<double> vec) {
    if (vec.empty()) 
        return 0.0;

    double sum = accumulate(vec.begin(), vec.end(), 0.0);
    return sum / vec.size();
}

// Функция для вычисления среднего геометрического
double Geometric(const vector<double> vec) {
    if (vec.empty()) 
        return 0.0;
    double prod = accumulate(vec.begin(), vec.end(), 1.0, multiplies<double>()); // Считаем произведение
    return exp(log(prod) / vec.size()); // Применяем формулу для корня n-ой степени

    // Использование формулы избавляет от нужны пользоваться pow, совет был для этого?
}

int main() {
    int n;
    cout << "Введите количество чисел\n";
    cin >> n;
    vector<double> vec;
    double x;
    for(int i = 0; i < n; i++){ //создание вектора
        cout << "a[" << i << "] = ";
        cin >> x;
        vec.push_back(x);
    }
    double arithmeticMean = Arithmetic(vec);
    cout << "Среднее арифметическое: " << arithmeticMean << endl;
    double geometricMean = Geometric(vec);
    cout << "Среднее геометрическое: " << geometricMean << endl;

    return 0;
}