#include <ctime>
#include <iostream>
#include <random>
#include <vector>

std::mt19937 rnd(time(NULL));

void printVector(std::vector<int> &array) {
    std::cout << "[";
    for (size_t i = 0; i < array.size() - 1; i++) {
        std::cout << array[i] << ", ";
    }
    std::cout << array[array.size() - 1] << "]\n";
}

// комментарии со второго семестра, их много, они странные, но они есть
void quickHelper(std::vector<int> &array, int low, int high) {
    int pivot = array[(low + high) / 2]; // берем точку опоры в середине
    int i = low; // создаем верхний и нижни указатель (потому что столбцы,
                 // так-то левый и правый)
    int j = high;
    while (i <= j) {
        // если элементы слева от середины меньше нее - то супер
        while (array[i] < pivot)
            i++;
        // если элемента справа от середины больше нее - то супер
        while (array[j] > pivot)
            j--;
        // получается наткнулись на элементы, которые не удовлетворяют условиям
        // выше - меняем их
        if (i <= j) {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    // получили массив, в котором слева от середины числа меньше, а справа -
    // больше. Теперь отдельно рекурсивно сортируем эти половинки
    if (j - low > 0)
        quickHelper(array, low, j);
    if (high - i > 0)
        quickHelper(array, j + 1, high);
}

void quickSort(std::vector<int> &array) {
    quickHelper(array, 0, static_cast<int>(array.size()) - 1);
}

void heapHelper(std::vector<int> &array, int index, int len) {
    // поднимаем ребенка пока он больше родителей
    while (true) {
        int max = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < len && array[left] > array[max]) {
            max = left;
        }
        if (right < len && array[right] > array[max]) {
            max = right;
        }
        if (max == index) {
            break;
        } else {
            int a = array[index];
            array[index] = array[max];
            array[max] = a;

            index = max;
        }
    }
}

void heapSort(std::vector<int> &array) {
    int len = static_cast<int>(array.size());

    for (int i = len / 2 - 1; i >= 0; i--) {
        heapHelper(array, i, len);
    }

    for (int i = len - 1; i >= 0; i--) {
        int a = array[i];
        array[i] = array[0];
        array[0] = a;
        heapHelper(array, 0, i);
    }
}

void mergeHelper(std::vector<int> &array, int l, int r, int m) {
    if (l >= r || m < l || m > r) {
        return;
    }

    if (array.size() == 2 && array[0] > array[1]) {
        int a = array[0];
        array[0] = array[1];
        array[1] = a;
        return;
    }

    std::vector<int> buf(r - l + 1, 0);
    int cur = 0, i = l, j = m + 1;

    // Слияние пока есть элементы в обеих частях
    while (i <= m && j <= r) {
        if (array[i] <= array[j]) {
            buf[cur++] = array[i++];
        } else {
            buf[cur++] = array[j++];
        }
    }
    // копируем остатки левой части
    while (i <= m) {
        buf[cur++] = array[i++];
    }
    // копируем остатки правой части
    while (j <= r) {
        buf[cur++] = array[j++];
    }
    // Возвращаем отсортированные элементы в исходный массив
    for (int t = 0; t < buf.size(); ++t) {
        array[l + t] = buf[t];
    }
}

void mergeHelper2(std::vector<int> &array, int l, int r) {
    if (l >= r)
        return;
    // разделяющий элемент
    int m = (l + r) / 2;

    mergeHelper2(array, l, m);
    mergeHelper2(array, m + 1, r);
    mergeHelper(array, l, r, m);
}

void mergeSort(std::vector<int> &array) {
    mergeHelper2(array, 0, static_cast<int>(array.size()) - 1);
}

std::vector<int> generateRandomVector(size_t size, int min, int max) {
    std::vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = rnd() % (max - min + 1) + min;
    }
    return result;
}

void testSort(void (*testFunc)(std::vector<int> &), int min, int max) {
    std::vector<int> sizes = {50000, 100000, 500000, 1000000, 10000000};
    for (int i = 0; i < 5; i++) {
        std::vector<int> array = generateRandomVector(sizes[i], min, max);
        double total = 0.0;
        std::cout << "Testing func on " << sizes[i] << "\n";

        for (int j = 0; j < 5; j++) {
            std::clock_t start = std::clock();
            heapSort(array);
            std::clock_t end = std::clock();
            double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            total += cpu_time_used;
            std::cout << j + 1 << " Time: " << cpu_time_used << "\n";
        }

        double avg = total / 5;
        std::cout << "Average Time: " << avg << "\n";
    }
}

int main() {
    std::vector<int> aboba = {325,  253521,   7675, 0,    5235, 5235,
                              5235, -4235235, 5235, 2352, -32,  4,
                              1,    45,       2,    5,    53,   0};
    std::cout << "\nMERGE SORT\n\n";
    testSort(mergeSort, -1000, 1000);
    std::cout << "\nHEAP SORT\n\n";
    testSort(heapSort, -1000, 1000);
    std::cout << "\nQUICK SORT\n\n";
    testSort(quickSort, -1000, 1000);

    return 0;
}
