#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

void printVector(std::vector<int> array) {
    std::cout << "[";
    // size_t -- по сути беззнаковый int, который используется в vector
    for (size_t i = 0; i < array.size() - 1; i++) {
        std::cout << array[i] << ", ";
    }
    std::cout << array[array.size() - 1] << "]\n";
}

int numberRank(int num) {
    int i = 0;
    while (num > 0) {
        num /= 10;
        i++;
    }
    return i;
}

void countingSort(std::vector<int> &array) {
    int min = INT_MAX;
    int max = INT_MIN;

    for (size_t i = 0; i < array.size(); i++) {
        if (array[i] < min)
            min = array[i];
        else if (array[i] > max)
            max = array[i];
    }

    size_t length = max - min + 1;
    std::vector<int> count(length, 0);

    for (size_t i = 0; i < array.size(); i++) {
        // вычитаем минимум, так как длина массива соответствуем количеству
        // элементов с диапазона
        ++count[array[i] - min];
    }

    int index = 0;
    for (int i = 0; i < length; i++) {
        // проходимся по массиву подсчетов. Индекс элемента = реальное число из
        // изначального массива - min. При этом в изначальный массив должно
        // встать count[i] элементов i-й ячейки. Тогда для i-й ячейки
        // мы вставляем в изначальный массив count[i] элементов i + min
        for (int j = 0; j < count[i]; j++) {
            array[index] = i + min;
            index += 1;
        }
    }
}

void radixSort(std::vector<int> &array) {
    // ищем максимальное количество разрядов
    int max_rank = INT_MIN;
    for (size_t i = 0; i < array.size(); i++) {
        int num_rank = numberRank(array[i]);
        if (num_rank > max_rank)
            max_rank = num_rank;
    }

    // двумерный массив, который будет хранить распределенные в соответствии с
    // цифрой разряда числа
    std::vector<std::vector<int>> helpers(10);

    // place отвечает за просматриваемый разряд
    for (int i = 0, place = 1; i < max_rank; place *= 10, i++) {
        for (size_t j = 0; j < array.size(); j++) {
            // array[j] / place % 10 -- так получаем цифру в нужном разряде
            helpers[array[j] / place % 10].push_back(array[j]);
        }

        int pos = 0;
        for (int j = 0; j < 10; j++) {
            for (size_t k = 0; k < helpers[j].size(); k++) {
                // засовываем обратно все элементы
                array[pos++] = helpers[j][k];
            }
            helpers[j].clear();
        }
    }
}

int main() {
    std::vector<int> array = {99,  82,   73234, 64, 2355, 32,
                              221, 3211, 965,   7,  5};
    printVector(array);
    countingSort(array);
    printVector(array);
    radixSort(array);
    printVector(array);
    return 0;
}
