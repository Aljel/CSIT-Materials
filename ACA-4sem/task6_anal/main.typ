#import "conf.typ" : conf
#show: conf.with(
  title: [Оценка сложности префикс-функции, Z-функции и алгоритма КМП],
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

= Префикс-функция

== Код программы

```cpp
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
```

== Анализ сложности



= Z-функция

== Код программы

```cpp
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
```

== Анализ сложности

= Алгоритм КМП

== Код программы

```cpp
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
```

== Анализ сложности