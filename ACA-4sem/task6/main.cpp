#include <iostream>
#include <string>
#include <vector>

void printVector(std::vector<size_t> &array) {
    std::cout << "[";
    for (size_t i = 0; i < array.size() - 1; i++) {
        std::cout << array[i] << ", ";
    }
    std::cout << array[array.size() - 1] << "]\n";
}

// префикс функция
std::vector<size_t> prefixFunc(std::string &str) {
    size_t n = str.size();
    std::vector<size_t> arr(n, 0);

    for (size_t i = 1; i < n; i++) {
        size_t j = arr[i - 1];

        while (j > 0 && str[i] != str[j]) {
            j = arr[j - 1];
        }

        if (str[i] == str[j]) {
            j++;
        }

        arr[i] = j;
    }

    return arr;
}

// z-функция
std::vector<size_t> zFunc(std::string &str) {
    size_t n = str.size();
    std::vector<size_t> arr(n, 0);

    // l и r - границы самого правого совпавшего префикса
    for (size_t i = 1, l = 0, r = 0; i < n; i++) {
        // если мы внутри z-блока, используем уже вычисленные данные
        if (i <= r) {
            if (r - i + 1 < arr[i - l])
                arr[i] = r - i + 1;
            else
                arr[i] = arr[i - l];
        }

        // пытаемся увеличить z[i] наивным сравнением
        while (i + arr[i] < n && str[arr[i]] == str[i + arr[i]]) {
            arr[i]++;
        }

        // если мы вышли за границу текущего блока [l, r], обновляем ее
        if (i + arr[i] - 1 > r) {
            l = i;
            r = i + arr[i] - 1;
        }
    }
    return arr;
}

int main() {
    std::string main;
    std::getline(std::cin, main);

    std::vector<size_t> res = prefixFunc(main);
    printVector(res);

    res = zFunc(main);
    printVector(res);

    return 0;
}
