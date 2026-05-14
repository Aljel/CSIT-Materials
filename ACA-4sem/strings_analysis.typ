#import "@preview/codly:1.3.0": *
#import "@preview/codly-languages:0.1.1": *
#show: codly-init
= Префикс-функция, Z-функция, алгоритм КМП

== Код программы

#codly(languages: codly-languages)
```cpp
std::vector<size_t> prefix(std::string &str) {
    size_t n = str.size();
    std::vector<size_t> arr(n + 1, 0);

    for (size_t i = 1; i < n; ++i) {
        size_t j = arr[i - 1];

        while (j > 0 && str[i] != str[j]) {
            j = arr[j - 1];
        }

        if (str[i] == str[j]) {
            ++j;
        }

        arr[i] = j;
    }

    return arr;
}

std::vector<size_t> z_func(std::string &str) {
    size_t n = str.size();
    std::vector<size_t> arr(n + 1, 0);

    for (size_t i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) {
            if (r - i + 1 < arr[i - l])
                arr[i] = r - i + 1;
            else
                arr[i] = arr[i - l];
        }

        while (i + arr[i] < n && str[arr[i]] == str[i + arr[i]]) {
            ++arr[i];
        }

        if (i + arr[i] - 1 > r) {
            l = i;
            r = i + arr[i] - 1;
        }
    }
    return arr;
}


std::vector<size_t> kmp(std::string &text, std::string &pattern) {
    std::vector<size_t> occurrences;
    if (pattern.empty())
        return occurrences;

    std::string combined = pattern + "#" + text;
    std::vector<size_t> pi = prefix_func(combined);

    size_t len = pattern.size();

    for (size_t i = len + 1; i < pi.size(); i++) {
        if (pi[i] == len) {
            occurrences.push_back(i - 2 * len);
        }
    }

    return occurrences;
}
```

== Анализ сложности

=== Префикс-функция

Здесь $n$ --- длина входной строки.

Количество итераций во внешнем цикле --- $n$.

На каждом шаге префикс-функция возрастает не более, чем на единицу.
Значение переменной  `j`, которая указывает на длину префикса между итерациями внешнего цикла не меняется, а суммарно увеличивается не более чем на $n - 1$.

В цикле `while` оно только уменьшается, значит оно может уменьшится суммарно не более чем на $n - 1$.
Тогда количество итераций в `while` не может быть больше, чем $n$.

Значит сложность алгоритма: $O(n) + O(n) = O(n)$.

=== Z-функция

Здесь $n$ --- длина входной строки.

Количество итераций во внешнем цикле --- $n$.

Правая граница Z-блока $r$ сдвигается максимум до $n-1$, причём если она сдвигается, то мы считаем значение Z-функции наивным способом, а если нет, то мы это значение для этой позиции уже знаем.
Так каждая позиция участвует в наивном подсчёте Z-функции ровно один раз, и суммарно цикл `while` выполнится не более чем $n$ раз.

Значит сложность алгоритма: $O(n) + O(n) = O(n)$.

=== КМП


Здесь $n$ --- длина входной строки, а $m$ ---  длина шаблона.

Мы конкатенируем эти строки и вызываем для них префикс-функцию, что занимает $O(n + m)$ времени.

Далее в цикле проходимся по части префикс-функции, относящейся к исходной строке, что занимает ещё $O(n)$ времени.

Значит сложность алгоритма: $O(n+m) + O(n) = O(n + m)$.
