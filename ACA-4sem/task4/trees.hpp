#include <ncurses.h>
namespace tree {

struct treeNode {
        int inf;
        char col;
        treeNode *right;
        treeNode *left;
        treeNode *parent;
};

// Создание узла
treeNode *createNode(int x);
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

void printTree(treeNode *root, int k, int row, int col);

void printTree(treeNode *root);
} // namespace tree
