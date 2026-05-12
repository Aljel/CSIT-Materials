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

= Быстрая сортировка

== Код программы

```cpp
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
```

== Анализ сложности

Найдем сложность алгоритма с помощью следствия из первой теоремы.

Согласно теореме $ t(n) = cases(c\, &"если" n = 1, a t (n/k) + b n^tau\, &"если" n > 1) $. В нашем случае получаем формулу $ t(n) = cases(1\, &"если" n = 1, 2 t (n/2) + n^1\, &"если" n > 1) $, так как опорный элемент всегда является серединой данного нам массива, $k=2$, и мы делим задачу на 2 подзадачи --- $a = 2$. Также циклы `while` в коде работают за линейное время. Так как мы двигаемся по элементам слева от опорного и справа от опорного, суммарно в цикле мы пройдем всю длинну массива, поэтому $tau = 1$ (ну или потому что $log_k a = 1$). 

Получается, что $a = k^tau$ и по следствию из теоремы, мы получаем сложность $O(n^tau log_k n) = O(n log_2 n) = O(n log n)$

= Пиромидальная сортировка

== Код программы

```cpp
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
    int len = array.size();

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
```

== Анализ сложности

Так как сортировка основана на построении кучи, рассмотрим свойства этой структуры:
1. Дерево почти полное, то есть, заполнено на всех уровнях, кроме последнего. На последнем уровне элементы заполняются слева направо, пока не закончатся элементы.
2. Значение узла всегда больше значений его потомков.

Так как куча – это бинарное дерево, то его высота не больше, чем $log_2 n$. Всего элементов $n$. Следовательно,
максимальное количество шагов, которое необходимо сделать, чтобы получить кучу, $n log n$.
