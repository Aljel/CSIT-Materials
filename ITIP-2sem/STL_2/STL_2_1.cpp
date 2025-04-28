#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

// возвращает множество двузначных чисел
set<int> doubleDigit(set<int> a){
    set<int> b;
    for (auto it = a.begin(); it != a.end(); it++)
        if (9 < *it  && *it < 100)
            b.insert(*it);
    return b;
}

// возвращает множество однозначных чисел из множества двузначных
set<int> singleDigit(set<int> a){
    set<int> b;
    for (auto it = a.begin(); it != a.end(); it++){
        b.insert(*it / 10);
        b.insert(*it % 10);
    }
    return b;
}

void print(set<int> a){
    for (auto it = a.begin(); it != a.end(); it++)
        cout << *it << " ";
}

int main(){
    setlocale(LC_ALL, "russian");
    set<int> a;
    int n;
    cout << "Введите количество чисел \n";
    cin >> n;
    for (int i = 0; i < n; i++){
        cout << "a[" << i << "] = ";
        int num;
        cin >> num;
        a.insert(num);
    }
    print(singleDigit(doubleDigit(a)));
    return 0;
}