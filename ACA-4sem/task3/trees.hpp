#include <ncurses.h>
namespace tree {

struct treeNode {
        int inf;
        treeNode *right;
        treeNode *left;
        treeNode *parent;
};

// Создание узла
treeNode *nodeCreate(int x);

// Вставка элемента
void insert(treeNode *&tr, int x);

// Обходы дерева
void inorder(treeNode *tr);
void preorder(treeNode *tr);
void postorder(treeNode *tr);

// Поиск элементов
treeNode *find(treeNode *tr, int x);
treeNode *Min(treeNode *tr);
treeNode *Max(treeNode *tr);
treeNode *Next(treeNode *tr, int x);
treeNode *Prev(treeNode *tr, int x);

// Удаление узла
void Delete(treeNode *&tr, treeNode *v);

void printTree(treeNode *root, int k, int row, int col);

void printTree(treeNode *root);
} // namespace tree
