#include <iostream>
#include <string>

// Базовый абстрактный класс для работников
class Worker {
protected:
    int id;
    std::string name;

public:
    // Конструктор для списка
    Worker() {};
    // Конструктор
    Worker(const std::string workerName, int workerId) : id(workerId), name(workerName) {}

    // Конструктор копирования
    Worker(const Worker& other) : id(other.id), name(other.name) {}

    // Абстрактный метод для расчета среднемесячной зарплаты
    virtual double calculateMonthlySalary() const = 0;

    virtual void print() const {
        std::cout << "Работник " << name << " с ID: " << id;
    }

    // Геттеры
    std::string getName() const {return name;}
    int getId() const {return id;}

    // Оператор присваивания
    Worker& operator=(const Worker& other) {
        if (this != &other) {
            name = other.name;
            id = other.id;
        }
        return *this;
    }

    bool operator==(Worker& p) {
        return (name == p.getName() && id == p.getId());
    }
};

// Класс для работников с почасовой оплатой
class HourlyWorker : public Worker {
private:
    double hourlyRate;

public:
    // Конструктор
    HourlyWorker(std::string workerName, int workerId, double rate) : Worker(workerName, workerId), hourlyRate(rate) {}

    // Конструктор копирования
    HourlyWorker(const HourlyWorker& other) : Worker(other), hourlyRate(other.hourlyRate) {}

    // Реализация абстрактного метода. override, чтобы не вызывался метод родителя. Подсчет по формуле
    double calculateMonthlySalary() const override {
        return 20.8 * 8 * hourlyRate;
    }

    void print() const override {
        Worker::print();
        std::cout << ", Почасовая ставка: " << hourlyRate << ", Среднемесячная зарплата: " << calculateMonthlySalary() << "\n";
    }

    double getHourlyRate() const {return hourlyRate;}
};

// Класс для работников с фиксированной оплатой
class SalariedWorker : public Worker {
private:
    double fixedSalary;

public:
    // Конструктор
    SalariedWorker(std::string workerName, int workerId, double rate) : Worker(workerName, workerId), fixedSalary(rate) {}

    // Конструктор копирования
    SalariedWorker(const SalariedWorker& other) : Worker(other), fixedSalary(other.fixedSalary) {}

    // Реализация абстрактного метода
    double calculateMonthlySalary() const override {
        return fixedSalary;
    }

    void print() const override {
        Worker::print();
        std::cout << ", Среднемесячная зарплата: " << calculateMonthlySalary() << "\n";
    }

    double getFixedSalary() const { return fixedSalary; }
};

// Простая реализация списка без STL
struct node {
    Worker* inf;
    node* next;
    node* prev;
};

class workerList {
private:
    node* head;
    node* tail;
    
public:
    // Конструктор по умолчанию
    workerList() { 
        head = nullptr; 
        tail = nullptr;
    }
    
    // Геттеры головы и хвоста
    node* getHead() {return head;}
    node* getTail() {return tail;}

    // Добавление элемента в конец списка
    void push(Worker* r) {
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
    
    // Удаление элемента
    bool remove(Worker& worker) {
        node* current = head;
        while (current != NULL) {
            if (*(current->inf) == worker) {
                if (head == current && tail == current) {
                    head = tail = nullptr;
                } else if (current == head) {
                    head = head->next;
                    head->prev = nullptr;
                } else if (current == tail) {
                    tail = tail->prev;
                    tail->next = nullptr;
                } else {
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                }
                return true;
            }
            current = current->next;
        }
        return false;  // Элемент не найден
    }
    
    // Поиск элемента
    bool find(Worker& r) const {
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
    bool findAll(bool condition(Worker& r)) {
        node* r = head;
        while (r != NULL) {
            if (condition(*(r->inf))) {
                (*(r->inf)).print();
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
            std::cout << "Элемент " << index++ << ": ";
            (*(current->inf)).print();
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

// Демонстрационная версия с НЕ виртуальными методами (для эксперимента)
class NonVirtualWorker {
protected:
    std::string name;
    int id;

public:
    NonVirtualWorker(std::string workerName, int workerId) : id(workerId), name(workerName) {}

    // НЕ виртуальный метод 
    double calculateMonthlySalary() const {
        return 0.0; 
    }

    // НЕ виртуальный метод отображения
    void print() const {
        std::cout << "Работник " << name << " с ID: " << id;
    }
};

class NonVirtualHourlyWorker : public NonVirtualWorker {
private:
    double hourlyRate;

public:
    NonVirtualHourlyWorker(std::string workerName, int workerId, double rate) : NonVirtualWorker(workerName, workerId), hourlyRate(rate) {}

    // НЕ виртуальный переопределенный метод
    double calculateMonthlySalary() const {
        return 20.8 * 8 * hourlyRate;
    }

    void print() const {
        std::cout << "Работник " << name << " с ID: " << id  << ", Ставка: " << hourlyRate << "\n";
    }
};

int main() {
    std::setlocale(LC_ALL, "rus");
    // Создание списка работников
    workerList workers;
    
    // Создание объектов разных типов
    std::string name1, name2;
    int id1, id2;
    double s1, s2;

    std::cout << "Введите имена двух работников\n";
    std::cin >> name1 >> name2;
    std::cout << "Введите ID двух работников\n";
    std::cin >> id1 >> id2;
    std::cout << "Введите почасовую зарплату работника\n";
    std::cin >> s1;
    std::cout << "Введите ежемесячную зарплату работника\n";
    std::cin >> s2;

    HourlyWorker* hw1 = new HourlyWorker(name1, id1, s1);
    SalariedWorker* sw1 = new SalariedWorker(name2, id2, s2);
    
    // Добавление в список
    workers.push(hw1);
    workers.push(sw1);
    
    std::cout << "Работники из списка:\n";
    workers.print();
    
    // Демонстрация конструктора копирования
    HourlyWorker hw_copy(*hw1);
    std::cout << "Копия работника: ";
    hw_copy.print();
    
    SalariedWorker sw_copy(*sw1);
    std::cout << "Копия работника: ";
    sw_copy.print();
    
    // Демонстрация приведения типов в цикле
    node* current = workers.getHead();
    while (current != NULL) {
        Worker* worker = current->inf;
        
        // Попытка приведения к HourlyWorker
        HourlyWorker* hourlyPtr = dynamic_cast<HourlyWorker*>(worker);
        if (hourlyPtr != NULL) {
            std::cout << "Это почасовой работник с ставкой: " << hourlyPtr->getHourlyRate() << "\n";
        }
        
        // Попытка приведения к SalariedWorker
        SalariedWorker* salariedPtr = dynamic_cast<SalariedWorker*>(worker);
        if (salariedPtr != NULL) {
            std::cout << "Это работник с зарплатой: " << salariedPtr->getFixedSalary() << "\n";
        }
        
        current = current->next;
    }
    
    // Демонстрация поведения без виртуальных методов
    NonVirtualHourlyWorker* aboba1 = new NonVirtualHourlyWorker("Тест почасовой указывающий", 0002, 1244.0);
    std::cout << "Зарплата через указатель почасового: " << aboba1 -> calculateMonthlySalary() << "\n";
    aboba1->print();

    NonVirtualHourlyWorker aboba2("Тест почасовой", 0003, 1354.0);
    std::cout << aboba2.calculateMonthlySalary() << "\n";
    aboba2.print();
    
    return 0;
}