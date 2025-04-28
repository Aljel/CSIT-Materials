#include <iostream>
#include <string>
#include <set>
#include <map>
#include <cctype> // для tolower
using namespace std;

// Функция для перевода строки в нижний регистр
string lower(string a) {
    string b = a;
    for (size_t i = 0; i < a.length(); i++) {
        b[i] = tolower(a[i]);
    }
    return b;
}

int main() {
    setlocale(LC_ALL, "russian");
    string text;
    cout << "Введите текст:\n";
    getline(cin, text); // Используем getline для чтения всей строки

    set<string> pov; // Множество для слов из повествовательных предложений
    size_t pos = 0;  // Позиция начала текущего предложения

    while (pos < text.length()) {
        // Ищем следующий знак препинания
        size_t k = text.find_first_of("!?.", pos);
        if (k == string::npos) {
            break; // Если знаков препинания больше нет, выходим
        }

        // Извлекаем предложение
        string temp = text.substr(pos, k - pos);
        char tmp = text[k]; // Сохраняем знак препинания

        // Обрабатываем слова в предложении
        size_t pos1 = 0;
        size_t k1 = temp.find_first_of(" ", pos1);
        while (pos1 != string::npos) {
            string word = lower(temp.substr(pos1, k1 - pos1));
            if (tmp == '.') { // Если предложение повествовательное
                pov.insert(word);
            }
            pos1 = (k1 == string::npos) ? k1 : k1 + 1;
            k1 = temp.find_first_of(" ", pos1);
        }

        pos = k + 1; // Переходим к следующему предложению
    }

    // Выводим слова, которые встречаются только в повествовательных предложениях
    cout << "Слова из повествовательных предложений:\n";
    for (auto it = pov.begin(); it != pov.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
    return 0;
}