#include "RationalList.h"
#include <iostream>

using namespace std;

RationalList::RationalList() {
    head = nullptr;
    tail = nullptr;
}

node* RationalList::getHead() { return head; }
node* RationalList::getTail() { return tail; }

void RationalList::push(Rational* r) {
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

void RationalList::insertAfter(node* n, Rational& r) {
    node* rNode = new node;
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

void RationalList::remove(node* r) {
    if (r == NULL) return;  // Безопасность
    
    if (head == r && tail == r) {
        // Единственный элемент в списке
        head = tail = NULL;
    } 
    else if (r == head) {
        // Удаляем головной узел
        head = head->next;
        if (head != NULL) {
            head->prev = NULL;
        }
    } 
    else if (r == tail) {
        // Удаляем хвостовой узел
        tail = tail->prev;
        if (tail != NULL) {
            tail->next = NULL;
        }
    } 
    else {
        // Удаляем средний узел
        if (r->prev != NULL) {
            r->prev->next = r->next;
        }
        if (r->next != NULL) {
            r->next->prev = r->prev;
        }
    }
}

bool RationalList::find(Rational r) {
    node* current = head;
    while (current != nullptr) {
        if (*(current->inf) == r) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool RationalList::findAll(bool condition(Rational& r)) {
    node* r = head;
    while (r != NULL) {
        if (condition(*(r->inf))) {
            (*(r->inf)).Print();
        }
        r = r->next;
    }
    return (r != NULL);
}

void RationalList::print() const {
    node* current = head;
    int index = 0;
    while (current != nullptr) {
        cout << "Элемент " << index++ << ": ";
        (*(current->inf)).Print();
        current = current->next;
    }
}

void RationalList::clear() {
    node* current = head;
    while (current != nullptr) {
        node* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
}
