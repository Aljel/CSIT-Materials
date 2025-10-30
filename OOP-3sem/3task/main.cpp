#include "Rational.h"
#include "RationalList.h"
#include <iostream>
#include <string>
#include <exception>
#include <fstream>
using namespace std;

class err : public exception {
private:
    string message;
public:
    err(string msg) : message(msg) {};

    string getMessage() { return message; };

    void print() {
        cerr << "Ошибка:" << message << "\n";
    };
};

// Функции для проверки условий
bool isProperFraction(Rational& r) {
    return r.getIsProper();
}

bool isPositive(Rational& r) {
    return !r.getIsNegative();
}

bool hasWholePart(Rational& r) {
    return r.getWholePart() > 0;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    try {
        ifstream in("input.txt");
        if (!in) {
            throw err("Файл не открылся!");
        }
        string line;
        getline(in, line);
        cout << "Тест создания:\n";
        cout << "Введите числитель и знаменатель первого рационального числа\n";
        int n1, n2;
        cin >> n1 >> n2;
        try {
            Rational r1(n1, n2);
            r1.Print();
            
            cout << "Введите числитель и знаменатель второго рационального числа\n";
            cin >> n1 >> n2;
            Rational r2(n1, n2);
            r2.Print();
            
            cout << "Введите числитель и знаменатель третьего рационального числа\n";
            cin >> n1 >> n2;
            Rational r3(n1, n2);
            r3.Print();

            cout << "\nТест арифметики:\n";
            Rational sum = r1 + r2;
            cout << "первое + второе: ";
            sum.Print();
            
            Rational diff = r2 - r1;
            cout << "второе - первое: ";
            diff.Print();
            
            Rational prod = r1 * r2;
            cout << "первое * второе: ";
            prod.Print();
            
            Rational quot = r2 / r1;
            cout << "второе / первое: ";
            quot.Print();
            
            cout << "\nТест сравнения:\n";
            cout << "первое == второе: " << (r1 == r2 ? "да" : "нет") << "\n";
            cout << "первое < второго: " << (r1 < r2 ? "да" : "нет") << "\n";
            cout << "второе > первого: " << (r2 > r1 ? "да" : "нет") << "\n";
            
            cout << "\nТест списка:\n";
            RationalList list;
            
            // Добавление элементов
            list.push(&r1);
            list.push(&r2);
            list.push(&r3);
            
            cout << "Исходный список:\n";
            list.print();
            
            // Поиск элементов по условию
            cout << "\nПравильные дроби в списке:\n";
            list.findAll(isProperFraction);
            
            cout << "\nПоложительные числа в списке:\n";
            list.findAll(isPositive);
            
            cout << "\nЧисла с целой частью:\n";
            list.findAll(hasWholePart);
            
            node* n = list.getHead();
            
            // Вставка и удаление
            cout << "\nТест вставки и удаления\n";
            try {
                if (n == NULL) {
                    throw err("Удаление несуществующего узла\n");
                }
                if (!list.find(*(n->inf))) {
                    throw err("Удаление несуществующего элемента!\n");
                }
                list.remove(n);
                list.insertAfter(list.getHead(), r3);
                list.insertAfter(list.getTail(), r3);
                list.print();
            }
            catch(err aboba) {
                aboba.print();
            }
        }
        catch (int) {
            cerr << "Товарищ пользователь, у Вас знаменатель 0!\n";
        }
    }
    catch (err aboba) {
        aboba.print();
    }
    catch (...) {
        cerr << "Неизвестная ошибка\n";
    }
    return 0;
}
