#include <iostream>
#include <string>
#include <exception>

using namespace std;

// Задание 3: Создание пользовательского типа ошибки
class Error {
private:
    string message;
    int errorCode;
    
public:
    Error(string& msg, int code = 0) : message(msg), errorCode(code) {}
    
    string getMessage() { return message; }
    int getCode() { return errorCode; }
    
    void print() {
        cout << "Error [код " << errorCode << "]: " << message << endl;
    }
};

class Rational {
private:
    int numerator;      
    int denominator;    
    bool isProper;      
    int wholePart;      
    bool isNegative;    
    
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
    
    int abs(int a) {
        return std::abs(a);
    }

    void normalize() {
        if (denominator == 0) {
            cout << "Товарищ пользователь, у Вас знаменатель 0!\n";
            throw 0; // Исключение типа int
        }
        
        isNegative = (numerator < 0) || (denominator < 0);
        numerator = abs(numerator);
        denominator = abs(denominator);
        
        int g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        
        isProper = (numerator < denominator) && (numerator != 0);
        
        if (!isProper) {
            wholePart = numerator / denominator;
            numerator = numerator % denominator;
        } else {
            wholePart = 0;
        }
    }

public:
    Rational() {
        numerator = 0;
        denominator = 1;
        isProper = true;
        wholePart = 0;
        isNegative = false;
    }
    
    Rational(int num, int den = 1) {
        numerator = num;
        denominator = den;
        normalize();
    }
    
    int getNumerator() {return numerator;}
    int getDenominator() {return denominator;}
    bool getIsProper() {return isProper;}
    int getWholePart() {return wholePart;}
    bool getIsNegative() {return isNegative;}
    
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
    
    double getValue() const {
        double result = wholePart + (double)numerator / denominator;
        return isNegative ? -result : result;
    }
    
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
    
    bool operator==(const Rational other) const {
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
    
    Rational operator+(const Rational other) const {
        int newNum = (isNegative ? -1 : 1) * (wholePart * denominator + numerator) * other.denominator +
                     (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator) * denominator;
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
        // Задание 1: Генерация исключения типа const char* при делении на ноль
        int otherTotal = (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator);
        if (otherTotal == 0) {
            throw "Деление на ноль!"; // Исключение типа const char*
        }
        
        int thisTotal = (isNegative ? -1 : 1) * (wholePart * denominator + numerator);
        return Rational(thisTotal * other.denominator, denominator * otherTotal);
    }

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
    RationalList() { 
        head = nullptr; 
        tail = nullptr;
    }
    
    node* getHead() {return head;}
    node* getTail() {return tail;}

    void push(Rational* r) {
        // Задание 3: Генерация пользовательского исключения при добавлении nullptr
        if (r == nullptr) {
            throw Error("Попытка добавить nullptr в список!", 101);
        }
        
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
    
    void insertAfter(node* n, Rational& r) {
        // Задание 3: Генерация пользовательского исключения
        if (n == nullptr) {
            throw Error("Попытка вставки после nullptr узла!", 102);
        }
        
        if (!find(*(n->inf))) {
            throw Error("Узел для вставки не найден в списке!", 103);
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
    
    void remove(node *r) {
        // Задание 1: Генерация исключения типа float
        if (r == nullptr) {
            throw 1.0f; // Исключение типа float
        }

        if (!find(*(r->inf))) {
            throw Error("Попытка удалить несуществующий элемент!", 104);
        }
        
        if (head == r && tail == r) {
            head = tail = nullptr;
        } else if (r == head) {
            head = head->next;
            head->prev = nullptr;
        } else if (r == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            r->next->prev = r->prev;
            r->prev->next = r->next;
        }
        delete r;
    }
    
    bool find(Rational r) {
        node* current = head;
        while (current != nullptr) {
            if (*(current->inf) == r) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    bool findAll(bool condition(Rational& r)) {
        node* r = head;
        while (r != nullptr) {
            if (condition(*(r->inf))) {
                (*(r->inf)).Print();
            }
            r = r->next;
        }
        return (r != nullptr);
    }
    
    void print() const {
        node* current = head;
        int index = 0;
        while (current != nullptr) {
            cout << "Элемент " << index++ << ": ";
            (*(current->inf)).Print();
            current = current->next;
        }
    }
    
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
    
    // Задание 2: Перехват любой исключительной ситуации с помощью try-catch
    try {
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

        list.push(&r1);
        list.push(&r2);
        list.push(&r3);
        
        cout << "Исходный список:\n";
        list.print();
        
        cout << "\nПравильные дроби в списке:\n";
        list.findAll(isProperFraction);
        
        cout << "\nПоложительные числа в списке:\n";
        list.findAll(isPositive);
        
        cout << "\nЧисла с целой частью:\n";
        list.findAll(hasWholePart);

        node* n = nullptr;

        cout << "\nТест вставки и удаления\n";
        
        // Демонстрация исключений
        try {
            list.remove(nullptr); // Генерирует float исключение
        } catch (float f) {
            cout << "Перехвачено исключение типа float: " << f << " (попытка удалить nullptr)\n";
        }
        
        try {
            list.remove(n); // Тоже генерирует float исключение
        } catch (float f) {
            cout << "Перехвачено исключение типа float: " << f << " (попытка удалить неинициализированный узел)\n";
        }
        
        list.remove(list.getHead()->next);
        list.insertAfter(list.getHead(), r3);
        list.insertAfter(list.getTail(), r3);
        list.print();
        
        // Тест деления на ноль (const char*)
        try {
            Rational zero(0, 1);
            Rational result = r1 / zero;
        } catch (const char* msg) {
            cout << "Перехвачено исключение типа const char*: " << msg << "\n";
        }
        
        // Тест создания дроби с нулевым знаменателем (int)
        try {
            Rational invalid(5, 0);
        } catch (int errorCode) {
            cout << "Перехвачено исключение типа int: " << errorCode << " (нулевой знаменатель)\n";
        }
        
        // Тест пользовательского исключения Error
        try {
            list.push(nullptr);
        } catch (Error& e) {
            cout << "Перехвачено пользовательское исключение Error:\n";
            e.print();
        }
        
    } 
    // Задание 1: Перехват исключений типа int, const char*, float
    catch (int errorCode) {
        cout << "\n!!! Перехвачено исключение типа int: " << errorCode << "\n";
        cout << "Произошла ошибка при работе с рациональными числами (нулевой знаменатель)\n";
    }
    catch (const char* msg) {
        cout << "\n!!! Перехвачено исключение типа const char*: " << msg << "\n";
    }
    catch (float f) {
        cout << "\n!!! Перехвачено исключение типа float: " << f << "\n";
        cout << "Произошла ошибка при работе со списком (nullptr)\n";
    }
    // Задание 3: Перехват пользовательского типа Error
    catch (Error& e) {
        cout << "\n!!! Перехвачено пользовательское исключение:\n";
        e.print();
    }
    // Задание 2: Перехват любой неизвестной исключительной ситуации
    catch (...) {
        cout << "\n!!! Перехвачена неизвестная ошибка! Программа завершает работу некорректно.\n";
    }

    return 0;
}

