#import "conf.typ" : conf
#show: conf.with(
  title: [Анализ сложности алгоритма Бойера-Мура],
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

= 

== Код программы

```cpp
std::map<char, size_t> Badsyms(std::string &searchStr) {
    std::map<char, size_t> badsym;
    size_t m = searchStr.size();

    for (size_t i = 0; i < m; ++i) {
        badsym[searchStr[i]] = i;
    }
    return badsym;
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

std::vector<size_t> Suffshift(std::string &s) {
    size_t m = s.size();
    std::vector<size_t> suffshift(m, m);

    std::string reverse_s = s;
    reverse(reverse_s.begin(), reverse_s.end());

    std::vector<size_t> z = zFunc(reverse_s);
    reverse(z.begin(), z.end());

    for (size_t i = 0; i < m - 1; ++i) {
        size_t len = z[i];
        if (len > 0) {
            suffshift[m - 1 - len] = m - 1 - i;
        }
    }

    size_t j = 0;
    for (size_t k = m; k > 0; --k) {
        size_t i_idx = k - 1;

        if (z[i_idx] == i_idx + 1) {
            size_t shift = m - 1 - i_idx;
            while (j <= shift) {
                if (suffshift[j] == m) {
                    suffshift[j] = shift;
                }
                j++;
            }
        }
    }

    suffshift[m - 1] = 1;
    return suffshift;
}

void boyerMoure(std::string &text, std::string &searchStr) {
    int n = text.size();
    int m = searchStr.size();

    std::vector<size_t> suffshift = Suffshift(searchStr);
    std::map<char, size_t> badsym = Badsyms(searchStr);

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;

        while (j >= 0 && searchStr[j] == text[i + j]) {
            --j;
        }

        if (j < 0) {
            std::cout << i << "\n";
            i += suffshift[0];
        } else {
            char mismatched_char = text[i + j];
            int bad_char_shift;
            if (badsym.count(mismatched_char)) {
                bad_char_shift =
                    std::max((size_t)1, j - badsym[mismatched_char]);
            } else {
                bad_char_shift = j + 1;
            }

            int good_suffix_shift = suffshift[j];

            i += std::max(good_suffix_shift, bad_char_shift);
        }
    }
}
```

== Анализ сложности

Оценим сложность функции `Suffshift`. Здесь `m` обозначает длину шаблона.
Сначала формируем обратную копию строки и считаем Z‑функцию — каждая из этих операций работает за $O(m)$.
Полученный массив разворачиваем, тоже за $O(m)$.
Далее идёт цикл от 0 до `m‑2`, выполняющий линейную работу — еще $O(m)$.
После этого двойной цикл: внешний перебирает `k` от `m` до 1, внутренний while увеличивает переменную `j` от 0 до `m` суммарно за все итерации, поэтому общая сложность этого участка тоже $O(m)$.
В конце ставим последний элемент массива — константная операция.
Складывая все части, получаем $O(m)$.

Функция `Badsyms` проходит по шаблону длиной `m` один раз, помещая каждый символ в map, что в среднем случае требует $O(1)$ на вставку, итого $O(m)$.

Теперь рассматриваем основную процедуру `boyerMoure`. Предобработка состоит из вызовов `Suffshift` и `Badsyms`, что даёт $O(m)+O(m)=O(m)$.

Внешний цикл while выполняется пока `i <= n - m`, то есть в худшем случае делает $O(n)$ итераций (`n` — длина текста).
На каждой итерации:
внутренний while сравнивает символы шаблона и текста справа налево до первого несовпадения или полного совпадения;
после этого вычисляются два кандидата на сдвиг:
  - сдвиг по правилу плохого символа: один запрос в map и несколько арифметических операций;
  - сдвиг по правилу хорошего суффикса: один доступ к массиву `suffshift` по известному индексу;
выбирается максимум из них и `i` увеличивается на это значение.

Ключевое свойство: благодаря совместному использованию обеих эвристик каждый символ текста участвует в успешных сравнениях не более одного раза за весь алгоритм. Следовательно суммарное число выполненных сравнений (как успешных, так и неуспешных) ограничено линейной функцией от `n`; формально можно показать, что оно не превышает $2n$.
Поэтому сложность поискового цикла равна $O(n)$.

Сложность всего алгоритма — сумма сложностей предобработки и поиска: $O(m)+O(n)=O(n+m)$.

В типичных случаях, когда длина текста существенно превышает длину шаблона (`n >> m`), доминирующим членом является $O(n)$.