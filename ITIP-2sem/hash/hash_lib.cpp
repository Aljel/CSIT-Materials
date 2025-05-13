#include "hash_lib.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath> 

bool people::operator==(const people& other) const {
    return surname == other.surname &&
           work == other.work &&
           dateOfBirth.dd == other.dateOfBirth.dd &&
           dateOfBirth.mm == other.dateOfBirth.mm &&
           dateOfBirth.yy == other.dateOfBirth.yy &&
           timeOfWork == other.timeOfWork &&
           salary == other.salary;
}


// перевод строки в дату
date strToDate(std::string str) { // из строки в дату
    date a;
    std::string temp = str.substr(0, 2); // день
    a.dd = atoi(temp.c_str());
    temp = str.substr(3, 2); // месяц
    a.mm = atoi(temp.c_str());
    temp = str.substr(6, 4); // год
    a.yy = atoi(temp.c_str());
    return a;
}


// вставка элемента в конец списка
void push(list *&h, list *&t, people x) {
    list *r = new list; // создаем новый элемент
    r->inf = x;
    r->next = NULL; // всегда последний

    if (!h && !t) {     // если список пуст
        r->prev = NULL; // первый элемент
        h = r;          // это голова
    } else {
        t->next = r; // r - следующий для хвоста
        r->prev = t; // хвост - предыдущий для r
    }
    t = r; // r теперь хвост
}

// поиск элемента
list *find(list *h, list *t, people x) { // поиск элементов списка
    list *p = h;                         // указатель на голову
    while (p) {                          // пока не дошли до конца списка
        if (p->inf == x)
            break;   // если нашли, прекращаем цикл
        p = p->next; // переход к следующему элементу
    }
    return p; // возвращаем указатель
}

// вставка элемента после другого
void listInsertAfter(list *&h, list *&t, list *r, people y) { // вставляем после r
    list *p = new list; // создаем новый элемент
    p->inf = y;
    if (r == t) {       // если вставляем после хвоста
        p->next = NULL; // вставляемый эл-т - последний
        p->prev = r;    // вставляем после r
        r->next = p;
        t = p;             // теперь хвост - p
    } else {               // вставляем в середину списка
        r->next->prev = p; // для следующего за r эл-та предыдущий - p
        p->next = r->next; // следующий за p - следующий за r
        p->prev = r;       // p вставляем после r
        r->next = p;
    }
}

// добавление элемента в начало списка
void listInsert(list*& head, list*& tail, people person) {
    list *newNode = new list{person, nullptr, nullptr};
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}
// удаление элемента списка
void del_node(list *&h, list *&t, list *r) { // удаляем после r
    if (r == h && r == t)                    // единственный элемент списка
        h = t = NULL;
    else if (r == h) { // удаляем голову списка
        h = h->next;   // сдвигаем голову
        h->prev = NULL;
    } else if (r == t) { // удаляем хвост списка
        t = t->prev;     // сдвигаем хвост
        t->next = NULL;
    } else {
        r->next->prev =
            r->prev; // для следующего от r предыдущим становится r->prev
        r->prev->next =
            r->next; // для предыдущего от r следующим становится r->next
    }
    delete r; // удаляем r
}

std::ifstream in("input.txt");
std::ofstream out("output.txt");

// создание списка людей из файла input.txt
std::vector<people> makePeopleVec() { // ввод из файла
    std::vector<people> x;
    people temp;
    while (in.peek() != EOF) { // считываем по пробелам
        in >> temp.surname;    // фамилия
        in >> temp.work;       // место работы

        std::string tmp; // дата рождения
        in >> tmp;
        temp.dateOfBirth = strToDate(tmp);

        in >> temp.timeOfWork; // стаж работы
        in >> temp.salary;     // зарплата

        x.push_back(temp);
    }
    return x;
}

// вывод структуры people
void printPeople(people x) {
    out << std::setw(11) << std::left << x.surname; // фамилия
    out << std::setw(10) << std::left << x.work;    // работа

    if (x.dateOfBirth.dd < 10)
        out << '0' << x.dateOfBirth.dd << '.'; // добавляем 0
    else
        out << x.dateOfBirth.dd << '.';
    if (x.dateOfBirth.mm < 10)
        out << '0' << x.dateOfBirth.mm << '.';
    else
        out << x.dateOfBirth.mm << '.';
    out << x.dateOfBirth.yy << "  ";

    out << std::setw(4) << std::left << x.timeOfWork;      // стаж работы
    out << std::setw(10) << std::left << x.salary << "\n"; // запрлата
}

// печать элементов списка
void printListOfPeople(list *h) {
    list *p = h; // укзатель на голову
    while (p) {  // пока не дошли до конца списка
        out << "\t";
        printPeople(p->inf);
        p = p->next; // переход к следующему элементу
    }
}

// печать открытой хэш-таблицы
void printOpenHashTable(std::vector<list*> T){
    for (size_t i = 0; i < T.size(); ++i) {
        out << "Bucket " << i << ":\n";
        printListOfPeople(T[i]);
    }
    out << "\n \n \n";
}

// создание открытой хэш-таблицы через метод деления по зарплате 
std::vector<list*> createOpenHashTable(std::vector<people> A, int M){
    std::vector<list*> T(M, NULL);
    std::vector<list*> tails(M, nullptr); // Для хранения хвостов списков

    for (int i = 0; i < A.size(); i++){
        int k = A[i].salary % M;
        listInsert(T[k], tails[k], A[i]);
    }

    return T;
}

// удаляет элемент из хэш-таблицы
void deleteOpenHashTable(std::vector<list*>& T, people P){
    int k = P.salary % T.size();
    list* current = T[k];
    while (current){
        if (current->inf == P){
            if (current->prev)
                current->prev->next = current->next;
            else
                T[k] = current->next;
            if (current->next)
                current->next->prev = current->prev;
            list* deletingNode = current;
            current = current->next;
            delete deletingNode;
        }
        else
            current = current->next;
    }
}

// вставка в открытыю хэш-таблицу
void insertOpenHashTable(std::vector<list*>& T, people P){
    int k = P.salary % T.size();
    list *newNode = new list{P, nullptr, nullptr};
    if (T[k]->next){
        newNode->next = T[k];
        T[k]->prev = newNode;
        T[k] = newNode;
    }
    else
        T[k] = newNode;
}

// возвращает человека по его зарплате 
people searchOpenHashTable(std::vector<list*> T, int salary){
    int k = salary % T.size();
    list* current = T[k];
    while (current){
        if (current->inf.salary == salary)
            return current->inf;
        current = current->next;
    }
    out << "Could not find a guy you are looking for!\n";
    people P;
    P.surname = "mistake";
    return P;
}

// печать закрытой хэш-таблицы
void printClosedHashTable(std::vector<people> T){
    for (size_t i = 0; i < T.size(); ++i) {
        out << "Cell " << i << ": ";
        printPeople(T[i]);
    }
    out << "\n \n \n";
}

// вспомогательная хэш-функция для закрытой хэш-таблицы
int hashClosedHelpFunction(int M, int salary){
    double A = 0.61803;
    int h1 = floor(M * fmod(salary * A, 1.0));
    return h1;
}

// основная хэш-функция для закрытой хэш-таблицы
int hashClosedFunction(int M, int salary, int c1, int c2, int i){
    int h1 = hashClosedHelpFunction(M, salary);
    return ((h1 + c1 * i + c2 * i * i) % M);
}

// создание закрытой хэш-таблицы
std::vector<people> createClosedHashTable(std::vector<people> A, int M, int c1, int c2){
    std::vector<people> T(M);
    for (int i = 0; i < A.size(); i++){
        int j = 0;
        for (int l = 0; l < M; l++){
            int p = hashClosedFunction(M, A[i].salary, c1, c2, j);
            if (T[p].surname.empty()){
                T[p] = A[i];
                break;
            }
            else 
                j += 1;
        }
    }
    return T;
}

// поиск в закрытой хэш-таблице
people searchClosedHashTable(std::vector<people> T, int salary, int M, int c1, int c2){
    people P;
    P.surname = "mistake";
    for (int i = 0; i < M; i++){
        if (T[hashClosedFunction(M, salary, c1, c2, i)].surname.empty())
            return P;
        if (T[hashClosedFunction(M, salary, c1, c2, i)].salary == salary)
            return T[hashClosedFunction(M, salary, c1, c2, i)];
    }
    return P;
}

// вставка в закрытую хэш-таблицу
void insertClosedHashTable(std::vector<people>& T, people P, int M, int c1, int c2){
    for (int i = 0; i < M; i++){
        int p = hashClosedFunction(M, T[i].salary, c1, c2, i);
        if (T[p].surname.empty()){
                T[p] = P;
                return;
            }
    }
    out << "Table is filled, unable to insert\n";
}