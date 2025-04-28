#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countMaxNumbers(const vector<int> numbers) {
    if (numbers.empty()) {
        return 0;
    }
    int max = *max_element(numbers.begin(), numbers.end()); // Находим максимальное число в наборе (max_element возвращает итератор)
    int counts = count(numbers.begin(), numbers.end(), max); // Считаем количество максимальных чисел
    return counts;
}

int main() {
    setlocale(LC_ALL, "russian");
    int n;
    cout << "Введите количество чисел\n";
    cin >> n;

    vector<int> numbers;
    int x;
    for(int i = 0; i < n; i++){//создание вектора
        cout << "a[" << i << "] = ";
        cin >> x;
        numbers.push_back(x);
    }

    cout << "Количество максимальных чисел: " << countMaxNumbers(numbers);
    return 0;
}