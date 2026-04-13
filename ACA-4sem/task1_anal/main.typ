#import "conf.typ" : conf
#show: conf.with(
  title: [Анализ сложности для сортировок, не использующих сравнение элементов],
  type: "referat",
  info: (
      author: (
        name: [Железко Александра Дмитриевича],
        faculty: [КНиИТ],
        group: "251",
        sex: "male"
      ),
      inspector: (
        degree: "Старший преподаватель",
        name: "М. И. Сафрончик"
      )
  ),
  settings: (
    title_page: (
      enabled: true
    ),
    contents_page: (
      enabled: true
    )
  )
)

= Сортировка подсчетом

== Код программы

```cpp
void countingSort(std::vector<int> &array) {
    int min = INT_MAX;
    int max = INT_MIN;

    for (size_t i = 0; i < array.size(); i++) { // O(n)
        if (array[i] < min)
            min = array[i];
        if (array[i] > max)
            max = array[i];
    }

    size_t length = max - min + 1;
    std::vector<size_t> count(length, 0);

    for (size_t i = 0; i < array.size(); i++) { // O(n)
        ++count[array[i] - min];
    }

    int index = 0;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < count[i]; j++) {
            array[index] = i + min;
            ++index;
        }
    }
}
```

== Анализ сложности

Первый цикл программы, созданный для определения максимального и минимального элементов занимает $O(n)$, так как просто перебирает и сравнивает элементы, где $n$ --- длина данного массива.

Далее цикл для подсчета количества разных элементов в данном нам массиве также занимает $O(n)$.

Далее для записи элементов в исходный массив требуется $n + k$ шагов, а то есть асимптотика $O(n + k)$

Итоговая сложность: $O(n) + O(n) + O(n + k) = O(n + k)$

= Поразрядная сортировка

== Код прогрмаммы

```cpp
void radixSort(std::vector<int> &array) {
    int max_rank = INT_MIN;
    for (size_t i = 0; i < array.size(); i++) {
        int num_rank = numberRank(array[i]);
        if (num_rank > max_rank)
            max_rank = num_rank;
    }

    std::vector<std::vector<int>> helpers(10);

    for (int i = 0, place = 1; i < max_rank; place *= 10, i++) {
        for (size_t j = 0; j < array.size(); j++) {
            helpers[array[j] / place % 10].push_back(array[j]);
        }

        int pos = 0;
        for (int j = 0; j < 10; j++) {
            for (size_t k = 0; k < helpers[j].size(); k++) {
                array[pos++] = helpers[j][k];
            }
            helpers[j].clear();
        }
    }
}

void LSD(std::vector<int> &array) {
    std::vector<int> neg_part;
    std::vector<int> pos_part;

    for (size_t i = 0; i < array.size(); i++) {
        if (array[i] >= 0)
            pos_part.push_back(array[i]);
        if (array[i] < 0)
            neg_part.push_back(-(array[i]));
    }

    radixSort(pos_part);
    radixSort(neg_part);

    std::reverse(neg_part.begin(), neg_part.end());

    int safe_index = 0;
    for (size_t i = 0; i < neg_part.size(); i++) {
        array[safe_index++] = -(neg_part[i]);
    }
    for (size_t i = 0; i < pos_part.size(); i++) {
        array[safe_index++] = pos_part[i];
    }
}
```

== Анализ сложности

Основная функция `LSD`: выполняется 1 цикл для сортировки значений в зависимости от знака за $O(n)$, где $n$ --- длина массива. Далее вызываются функции для самой сортировки, а массив с отрицательными элементами "разворачивается" функцией `std::reverse`, которая работает за линейное время

Далее два массива (с отсортированными отрицательными и положительными элементами) объединяется в один за $n$ шагов

Функция `radixSort`: сначала находит максимальное число разрядов из чисел массива за $n$ шагов. Далее создает вспомогательный массив, и в вложенном цикле элементы первоначального массива попадают в соответствующие $j$-му разряду ячейки, после чего в начальный массив последовательно возвращаются значения из вспомогательного. Все данные операции занимают $O(n)$, при это повторяются $k$ раз, где $k$ --- максимальный разряд. Соответственно, итоговая сложность $O(n dot k)$

Разбиение массива на 2 части с отрицательными и неотрицательными числами не влияет на итоговую асимптотику, которая получается равной $O(n dot k)$