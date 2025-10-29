#include <iostream>
#include <string>

using namespace std;

class Rational {
private:
    int numerator;      // числитель
    int denominator;    // знаменатель
    bool isProper;      // флаг правильной дроби
    int wholePart;      // целая часть для смешанной дроби
    bool isNegative;    // флаг отрицательного числа. Ну а как еще 5 атрибутов наскрести? 
    
    // Вспомогательный метод для нахождения НОД. Классический алгоритм Евклида. Решение наиболее оптимальное
    int gcd(int a, int b) {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    
    // Вспомогательный метод для взятия модуля от представителя Rational
    int abs(int a) {
        return std::abs(a);
    }

    // Приведение дроби к каноническому виду
    void normalize() {
        if (denominator == 0) {
            cout << "Товарищ пользователь, у Вас знаменатель 0!\n";
            throw invalid_argument("Знаменатель 0! Покайтесь!\n");
        }
        
        // Определяем знак
        isNegative = (numerator < 0) || (denominator < 0);
        numerator = abs(numerator);
        denominator = abs(denominator);
        
        // Сокращаем дробь
        int g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        
        // Определяем тип дроби
        isProper = (numerator < denominator) && (numerator != 0);
        
        // Вычисляем целую часть
        if (!isProper) {
            wholePart = numerator / denominator;
            numerator = numerator % denominator;
        } else {
            wholePart = 0;
        }
    }

public:
    // Конструктор по умолчанию
    Rational() {
        numerator = 0;
        denominator = 1;
        isProper = true;
        wholePart = 0;
        isNegative = false;
    }
    
    // Конструктор с параметрами
    Rational(int num, int den = 1) {
        numerator = num;
        denominator = den;
        normalize();
    }
    
    // Геттеры для всех атрибутов
    int getNumerator() {return numerator;}
    int getDenominator() {return denominator;}
    bool getIsProper() {return isProper;}
    int getWholePart() {return wholePart;}
    bool getIsNegative() {return isNegative;}
    
    // Сеттеры не для всех атрибутов
    void setNumerator(int num) { 
        numerator = num; 
        normalize(); 
    }
    
    void setDenominator(int den) { 
        denominator = den; 
        normalize(); 
    }
    
    void setRational(int num, int den) {
        numerator = num;
        denominator = den;
        normalize();
    }
    
    // Получение десятичного значения
    double getValue() const {
        double result = wholePart + (double)numerator / denominator; // Для корректного деления меняем тип числителя на double
        return isNegative ? -result : result; // 5 атрибутов есть 5 атрибутов. Терпим
    }
    
    // Преобразование к смешанной дроби. Строки для красивого вывода
    string toMixed() const {
        string result = "";
        if (isNegative) {
            result += "-";
        }

        if (wholePart != 0) { 
            result += to_string(wholePart) + " "; 
        }
        result += to_string(numerator) + "/" + to_string(denominator);
        
        
        return result;
    }
    
    // Операции сравнения
    bool operator==(const Rational other) const { // Сравниваем, приводя к общему знаменателю
        return (numerator * other.denominator == other.numerator * denominator) &&
               (isNegative == other.isNegative) && (wholePart == other.wholePart);
    }
    
    bool operator!=(const Rational other) const {
        return !(*this == other);
    }
    
    bool operator<(const Rational other) const {
        double thisValue = getValue();
        double otherValue = other.getValue();
        return thisValue < otherValue;
    }
    
    bool operator>(const Rational other) const {
        return other < *this;
    }
    
    bool operator<=(const Rational other) const {
        return *this < other || *this == other;
    }
    
    bool operator>=(const Rational other) const {
        return *this > other || *this == other;
    }
    
    // Арифметические операции
    Rational operator+(const Rational other) const {
        int newNum = (isNegative ? -1 : 1) * (wholePart * denominator + numerator) * other.denominator +
                     (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator) * denominator; // Приводим у общему знаменателю, используем тернарный оператор для определения знака в формуле
        int newDen = denominator * other.denominator;
        return Rational(newNum, newDen);
    }
    
    Rational operator-(const Rational other) const {
        int newNum = (isNegative ? -1 : 1) * (wholePart * denominator + numerator) * other.denominator -
                     (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator) * denominator;
        int newDen = denominator * other.denominator;
        return Rational(newNum, newDen);
    }
    
    Rational operator*(const Rational other) const {
        int thisTotal = (isNegative ? -1 : 1) * (wholePart * denominator + numerator);
        int otherTotal = (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator);
        return Rational(thisTotal * otherTotal, denominator * other.denominator);
    }
    
    Rational operator/(const Rational other) const {
        int thisTotal = (isNegative ? -1 : 1) * (wholePart * denominator + numerator);
        int otherTotal = (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator);
        return Rational(thisTotal * other.denominator, denominator * otherTotal);
    }

    // Метод для красивого вывода. Так как идут дополнительные фразы, написал заново, вместо перевода в строку 
    void Print() {
        cout << "Рациональное число: ";
        if (isNegative) cout << "-";
        
        if (wholePart != 0) {
            cout << wholePart << " ";
        } else if (numerator != 0) {
            cout << numerator << "/" << denominator;
        } else {
            cout << 0;
        }
        
        cout << " (десятичное: " << (isNegative ? -1 : 1) * 
               (wholePart + (double)numerator / denominator) << ")";
        cout << " [" << (isProper ? "правильная" : "неправильная") << " дробь]\n";
    }
};


// Структура узла списка
struct node {
    Rational* inf;
    node* next = nullptr;
    node* prev = nullptr;
};

class RationalList {
private:
    node* head;
    node* tail;
    
public:
    // Конструктор по умолчанию
    RationalList() { 
        head = nullptr; 
        tail = nullptr;
    }
    
    // Геттеры головы и хвоста
    node* getHead() {return head;}
    node* getTail() {return tail;}

    // Добавление элемента в конец списка
    void push(Rational* r) {
        node* rNode = new node;
        rNode->inf = r;
        rNode->next = nullptr;
        
        if (!head && !tail) {
            rNode->prev = nullptr;
            head = rNode;
        } else {
            tail->next = rNode;
            rNode->prev = tail;
        }
        tail = rNode;
    }
    
    // Вставка элемента
    void insertAfter(node* n, Rational& r) {
        if (!find(*(n->inf))) {
            cout << "Пользователь дурак, такого элемента нет!\n";
            return;
        }
        node *rNode = new node;

        rNode->inf = &r;
        rNode->next = n->next;
        rNode->prev = n;

        if (n->next) {
            n->next->prev = rNode;
        } else {
            tail = rNode;
        }

        n->next = rNode;  
    }
    
    // Удаление элемента
    void remove(node *r) {
        if (r == NULL) {
            cout << "Пользователь абсолютный дурак\n";
            return;
        }

        if (!find(*(r->inf))) {
            cout << "Пользователь дурак, такого элемента нет!\n";
            return;
        }
        
        if (head == r && tail == r) {
            head = tail = NULL;
        } else if (r == head) {
            head = head->next;
            head->prev = NULL;
        } else if (r == tail) {
            tail = tail->prev;
            tail->next = NULL;
        } else {
            r->next->prev = r->prev;
            r->prev->next = r->next;
        }
        delete r;
    }
    
    // Поиск элемента
    bool find(Rational r)  {
        node* current = head;
        while (current != nullptr) {
            if (*(current->inf) == r) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Поиск всех элементов по условию
    bool findAll(bool condition(Rational& r)) {
        node* r = head;
        while (r != NULL) {
            if (condition(*(r->inf))) {
                (*(r->inf)).Print();
            }
            r = r->next;
        }
        return (r != NULL);
    }
    
    // Вывод всех элементов списка
    void print() const {
        node* current = head;
        int index = 0;
        while (current != nullptr) {
            cout << "Элемент " << index++ << ": ";
            (*(current->inf)).Print();
            current = current->next;
        }
    }
    
    // Очистка списка
    void clear() {
        node* current = head;
        while (current != nullptr) {
            node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
    }
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
    
    cout << "Тест создания:\n";
    cout << "Введите числитель и знаменатель первого рационального числа\n";
    int n1, n2;
    cin >> n1 >> n2;
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

    
    node* n;

    // Вставка и удаление
    cout << "\nТест вставки и удаления\n";
    list.remove(nullptr); // 
    list.remove(n);
    list.remove(list.getHead()->next);
    list.insertAfter(list.getHead(), r3);
    list.insertAfter(list.getTail(), r3);
    list.print();

    return 0;
}
