#include "hash_lib.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

std::ifstream in("input.txt");
std::ofstream out("output.txt");

bool operator==(people a, people b) {
    return a.surname == b.surname && a.work == b.work &&
           a.timeOfWork == b.timeOfWork && a.salary == b.salary &&
           a.dateOfBirth.dd == b.dateOfBirth.dd &&
           a.dateOfBirth.mm == b.dateOfBirth.mm &&
           a.dateOfBirth.yy == b.dateOfBirth.yy;
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
void insert_after(list *&h, list *&t, list *r, people y) { // вставляем после r
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

// печать элементов списка
void printListOfPeople(list *h, list *t) {
    list *p = h; // укзатель на голову
    while (p) {  // пока не дошли до конца списка
        printPeople(p->inf);
        p = p->next; // переход к следующему элементу
    }
}
