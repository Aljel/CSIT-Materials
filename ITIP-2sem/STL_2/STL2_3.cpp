#include <iostream>
#include <string>
#include <set>
#include <map>
using namespace std;

// фунцкия, чтобы перевести строку в нижний регистр
string lower(string a){ 
    string b = a;
    for(int i = 0; i < a.length(); i++)
        b[i] = tolower(a[i]);
    return b;
}

int main(){
    setlocale(LC_ALL, "russian");
    string text;
    cout << "Введите текст\n";
    getline(cin, text); // ввод текста как целой строки. Простым cinом не получится

    set<string> pov;
    size_t pos = 0;

    while (pos < text.length()){
        size_t k = text.find_first_of("!?.", pos); //находим конец первого предложения 
        char tmp = text[k]; // запоминаем его характер

        string temp = text.substr(pos, k - pos); // само предложение

        size_t pos1 = 0;
        size_t k1 = temp.find_first_of(" ", pos1); // конец слова в предложении 

        while (pos1 != string::npos){
            string word = lower(temp.substr(pos1, k1 - pos1)); // само слово
            if (tmp == '.')
                pov.insert(word); // сохраняем нужное

            pos1 = (k1 == string::npos) ? k1 : k1 + 1; // увеличиваем pos1 так, чтобы не войти в бесконечный цикл
            k1 = temp.find_first_of(" ", pos1);
        }

        pos = k + 1;
    }

    for (auto it = pov.begin(); it != pov.end(); it++)
        cout << *it << " ";
    return 0;
}