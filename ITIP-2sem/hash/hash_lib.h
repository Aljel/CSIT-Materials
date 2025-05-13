#ifndef HASH_LIB_H
#define HASH_LIB_H

#include <iomanip>
#include <string>
#include <vector>

struct date {
        int dd, mm, yy;
};

struct people {
        std::string surname;
        std::string work;
        date dateOfBirth;
        int timeOfWork;
        int salary;
};

struct list {
        people inf;
        list *next;
        list *prev;
};      

bool operator==(people a, people b);
date strToDate(std::string str);
void push(list *&h, list *&t, people x);
list *find(list *h, list *t, people x);
void listInsertAfter(list *&h, list *&t, list *r, people y);
void del_node(list *&h, list *&t, list *r);
std::vector<people> makePeopleVec();
void printPeople(people x);
void printListOfPeople(list *h);
void listInsert(list*& head, list*& tail,people person);
void printHashTable(std::vector<list*> T);

#endif
