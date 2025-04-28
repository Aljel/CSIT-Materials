#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
using namespace std;

int main(){
    setlocale(LC_ALL, "russian");
    ifstream in("file_to_read.txt");
    if (!in.is_open()){
        cout << "Не удалось открыть файл!\n";
        return 0;
    }

    map<string, int> wordCount; // слова и их количество
    map<string, int> numCount; // числа и их количество 
    string firstNum; 
    string line;
    while (getline(in, line, ' ')){ // проход по отдельным словам в файле
        if (line.find_first_not_of("0123456789") != string::npos) // проверка, что в line - слово 
            wordCount[line]++; 
        else{
            if (numCount.size() == 0) // проверка на первое число в файле
                if (line != "0")
                    firstNum = line;
                else{ // проверка на 0
                    cout << "Первое число = 0!\n";
                    return 0;
                }
            numCount[line]++;
        }}
    int firstCount = numCount[firstNum];
    for (auto it = wordCount.begin(); it != wordCount.end(); it++) // ищем слова, которых столько же, сколько "первых чисел"
        if (it -> second == firstCount)
            cout << it -> first << " ";
    return 0;
}