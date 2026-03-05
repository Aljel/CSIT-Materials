#include "trees.hpp"
#include <iostream>
#include <ncurses.h>
namespace tree {

treeNode *nodeCreate(int x) {
    treeNode *n = new treeNode;
    n->inf = x;
    n->left = n->right = nullptr;
    n->parent = nullptr;
    return n;
}

void insert(treeNode *&tr, int x) {
    treeNode *n = nodeCreate(x);
    if (!tr) {
        tr = n;
    } else {
        treeNode *y = tr;
        while (y) {
            if (n->inf >= y->inf) {
                if (y->right) {
                    y = y->right;
                } else {
                    n->parent = y;
                    y->right = n;
                    break;
                }
            } else if (n->inf < y->inf) {
                if (y->left) {
                    y = y->left;
                } else {
                    n->parent = y;
                    y->left = n;
                    break;
                }
            }
        }
    }
}

treeNode *find(treeNode *tr, int x) {
    if (!tr || x == tr->inf) {
        return tr;
    }
    if (x < tr->inf) {
        return find(tr->left, x);
    } else {
        return find(tr->right, x);
    }
}

treeNode *Min(treeNode *tr) {
    if (!tr->left) {
        return tr;
    } else {
        return Min(tr->left);
    }
}

treeNode *Max(treeNode *tr) {
    if (!tr->right) {
        return tr;
    } else {
        return Max(tr->right);
    }
}

treeNode *Next(treeNode *tr, int x) {
    treeNode *n = find(tr, x);
    if (n->right) {
        return Min(n->right);
    }
    treeNode *y = n->parent;
    while (y && n == y->right) {
        n = y;
        y = y->parent;
    }
    return y;
}

treeNode *Prev(treeNode *tr, int x) {
    treeNode *n = find(tr, x);
    if (n->left) {
        return Max(n->left);
    }
    treeNode *y = n->parent;
    while (y && n == y->left) {
        n = y;
        y = y->parent;
    }
    return y;
}

void Delete(treeNode *&tr, treeNode *v) {
    treeNode *p = v->parent;
    if (!v->left && !v->right) {
        if (p->left == v) {
            p->left = nullptr;
        }
        if (p->right == v) {
            p->right = nullptr;
        }
        delete v;
    } else if (!v->left || !v->right) {
        if (!p) {
            if (!v->left) {
                tr = v->right;
                v->right->parent = nullptr;
            } else {
                tr = v->left;
                v->left->parent = nullptr;
            }
        } else {
            if (!v->left) {
                if (p->left == v) {
                    p->left = v->right;
                } else {
                    p->right = v->right;
                }
                v->right->parent = p;
            } else {
                if (p->left == v) {
                    p->left = v->left;
                } else {
                    p->right = v->left;
                }
                v->left->parent = p;
            }
        }
        delete v;
    } else {
        treeNode *succ = Next(tr, v->inf);
        v->inf = succ->inf;
        if (succ->parent->left == succ) {
            succ->parent->left = succ->right;
            if (succ->right) {
                succ->right->parent = succ->parent;
            }
        } else {
            succ->parent->right = succ->right;
            if (succ->right) {
                succ->right->parent = succ->parent;
            }
        }
        delete succ;
    }
}

// Функция для расчета глубины дерева
int treeDepth(treeNode *root) {
    if (!root)
        return 0;
    return 1 + std::max(treeDepth(root->left), treeDepth(root->right));
}

void preorder(treeNode *tr) { // прямой обход (К-Л-П)
    if (tr) {
        std::cout << tr->inf << " "; // корень
        preorder(tr->left);          // левое
        preorder(tr->right);         // правое
    }
}
void postorder(treeNode *tr) { // обратный обход (Л-П-К)
    if (tr) {
        postorder(tr->left);         // левое
        postorder(tr->right);        // правое
        std::cout << tr->inf << " "; // корень
    }
}

void inorder(treeNode *tr) { // симметричный обход (Л-К-П)
    if (tr) {
        inorder(tr->left);           // левое
        std::cout << tr->inf << " "; // корень
        inorder(tr->right);          // правое
    }
}

void setTree(treeNode *root, int row, int col, int colOffset) {
    if (!root)
        return;
    // Отрисовка текущего узла
    mvprintw(row, col, "%d", root->inf);
    // Расчет позиций потомков
    if (root->left) {
        mvprintw(row + 1, col - 1, "/");
        setTree(root->left, row + 2, col - colOffset, colOffset / 2);
    }
    if (root->right) {
        mvprintw(row + 1, col + 1, "\\");
        setTree(root->right, row + 2, col + colOffset, colOffset / 2);
    }
}

// Функция для вывода дерева
void printTree(treeNode *root) {
    initscr();   // Инициализация ncurses
    cbreak();    // Режим cbreak
    noecho();    // Не отображать ввод
    curs_set(0); // Скрыть курсор

    // int depth = treeDepth(root);
    int startCol = COLS / 8; // Начинаем с середины экрана
    setTree(root, 2, startCol, startCol / 2);
}
} // namespace tree
