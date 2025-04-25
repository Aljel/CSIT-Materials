#ifndef MYLIBRARY_H
#define MYLIBRARY_H

struct tree {
    int inf;
    tree* right;
    tree* left;
    tree* parent;
};

// Создание узла
tree* node(int x);

// Вставка элемента
void insert(tree*& tr, int x);

// Обходы дерева
void inorder(tree* tr);

// Поиск элементов
tree* find(tree* tr, int x);
tree* Min(tree* tr);
tree* Max(tree* tr);
tree* Next(tree* tr, int x);
tree* Prev(tree* tr, int x);

// Удаление узла
void Delete(tree*& tr, tree* v);

#endif 