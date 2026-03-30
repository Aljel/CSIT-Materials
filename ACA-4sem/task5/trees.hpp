#ifndef TREES_HPP
#define TREES_HPP

#include <ncurses.h>

namespace tree {

struct treeNode {
        int inf;
        int height; // В АВЛ-дереве используем высоту вместо цвета
        treeNode *right;
        treeNode *left;
        treeNode *parent;
};

// Создание узла
treeNode *createNode(treeNode *p, int x);
treeNode *createRoot(int x);

// Вставка элемента
void insertNode(treeNode *&tr, int x);

// Обходы дерева
void inorder(treeNode *tr);
void preorder(treeNode *tr);
void postorder(treeNode *tr);

// Поиск элементов
treeNode *find(treeNode *tr, int x);
treeNode *min(treeNode *tr);
treeNode *max(treeNode *tr);
treeNode *Next(treeNode *tr, int x);
treeNode *Prev(treeNode *tr, int x);

// Удаление узла
void deleteNode(treeNode *&tr, treeNode *x);

// Отрисовка
void printTree(treeNode *root);

} // namespace tree

#endif // TREES_HPP
