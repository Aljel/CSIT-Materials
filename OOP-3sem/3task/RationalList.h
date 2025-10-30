#pragma once
#include "Rational.h"

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
    RationalList();
    
    // Геттеры головы и хвоста
    node* getHead();
    node* getTail();
    
    // Добавление элемента в конец списка
    void push(Rational* r);
    
    // Вставка элемента
    void insertAfter(node* n, Rational& r);
    
    // Удаление элемента
    void remove(node* r);
    
    // Поиск элемента
    bool find(Rational r);
    
    // Поиск всех элементов по условию
    bool findAll(bool condition(Rational& r));
    
    // Вывод всех элементов списка
    void print() const;
    
    // Очистка списка
    void clear();
};
