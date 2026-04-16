#include <iostream>
#include <string>
#include <vector>

void printVector(std::vector<size_t> &array) {
    for (size_t i = 0; i < array.size() - 1; i++) {
        std::cout << array[i] << ", ";
    }
    std::cout << array[array.size() - 1] << "\n";
}

// префикс функция из прошлого задания
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

std::vector<size_t> kmpSearch(std::string &text, std::string &pattern) {
    std::vector<size_t> occurrences;
    if (pattern.empty())
        return occurrences;

    // # как разделитель для текста и строки, чтобы значение префикс функции не
    // превысило длину самой строки
    std::string combined = pattern + "#" + text;
    std::vector<size_t> pi = prefixFunc(combined);

    size_t patternLen = pattern.size();

    // если значение префикс-функции равно длине паттерна, значит мы нашли
    // полное вхождение. Начинаем проверку с индексов, соответствующих тексту
    // (после patternLen + 1)
    for (size_t i = patternLen + 1; i < pi.size(); i++) {
        if (pi[i] == patternLen) {
            // индекс начала вхождения в исходном тексте
            occurrences.push_back(i - 2 * patternLen);
        }
    }

    return occurrences;
}

void printResult(std::vector<size_t> &indices) {
    if (indices.empty()) {
        std::cout << "None\n";
        return;
    }
    std::cout << "Result: ";
    printVector(indices);
}

int main() {
    std::string text, pattern;

    std::cout << "Enter main text: ";
    std::getline(std::cin, text);
    std::cout << "Enter substring: ";
    std::getline(std::cin, pattern);

    std::vector<size_t> result = kmpSearch(text, pattern);
    printResult(result);

    return 0;
}
