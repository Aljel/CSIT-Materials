#include "trees.hpp"
#include <algorithm>
#include <iostream>
#include <ncurses.h>

namespace tree {

int height(treeNode *n) { return n ? n->height : 0; }

int bfactor(treeNode *n) { return n ? height(n->right) - height(n->left) : 0; }

void fixHeight(treeNode *n) {
    if (n) {
        n->height = 1 + std::max(height(n->left), height(n->right));
    }
}

treeNode *createNode(treeNode *p, int x) {
    treeNode *n = new treeNode;
    n->inf = x;
    n->height = 1; // У новых узлов высота 1
    n->left = n->right = nullptr;
    n->parent = p;
    return n;
}

treeNode *createRoot(int x) { return createNode(nullptr, x); }

void rotateRight(treeNode *&root, treeNode *x) {
    treeNode *y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;

    y->parent = x->parent;
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        root = y;
    }

    x->parent = y;
    y->right = x;

    fixHeight(x);
    fixHeight(y);
}

void rotateLeft(treeNode *&root, treeNode *x) {
    treeNode *y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        root = y;
    }

    x->parent = y;
    y->left = x;

    fixHeight(x);
    fixHeight(y);
}

void balance(treeNode *&root, treeNode *p) {
    fixHeight(p);
    if (bfactor(p) == 2) {
        if (bfactor(p->right) < 0) {
            rotateRight(root, p->right);
        }
        rotateLeft(root, p);
    } else if (bfactor(p) == -2) {
        if (bfactor(p->left) > 0) {
            rotateLeft(root, p->left);
        }
        rotateRight(root, p);
    }
}

void insertNodePrep(treeNode *&tr, treeNode *prev, int x, treeNode *&inserted) {
    if (x < prev->inf) {
        if (!prev->left) {
            prev->left = createNode(prev, x);
            inserted = prev->left;
        } else
            insertNodePrep(tr, prev->left, x, inserted);
    } else if (x > prev->inf) {
        if (!prev->right) {
            prev->right = createNode(prev, x);
            inserted = prev->right;
        } else
            insertNodePrep(tr, prev->right, x, inserted);
    }
}

void insertNode(treeNode *&tr, int x) {
    if (!tr) {
        tr = createRoot(x);
        return;
    }
    treeNode *inserted = nullptr;
    insertNodePrep(tr, tr, x, inserted);

    // Проходим вверх и балансируем дерево от добавленного элемента к корню
    treeNode *curr = inserted;
    while (curr) {
        balance(tr, curr);
        curr = curr->parent;
    }
}

treeNode *min(treeNode *tr) {
    if (!tr->left)
        return tr;
    else
        return min(tr->left);
}

treeNode *max(treeNode *tr) {
    if (!tr->right)
        return tr;
    else
        return max(tr->right);
}

treeNode *find(treeNode *tr, int x) {
    if (!tr || x == tr->inf)
        return tr;
    if (x < tr->inf)
        return find(tr->left, x);
    else
        return find(tr->right, x);
}

treeNode *Next(treeNode *tr, int x) {
    treeNode *n = find(tr, x);
    if (!n)
        return nullptr;
    if (n->right)
        return min(n->right);
    treeNode *y = n->parent;
    while (y && n == y->right) {
        n = y;
        y = y->parent;
    }
    return y;
}

treeNode *Prev(treeNode *tr, int x) {
    treeNode *n = find(tr, x);
    if (!n)
        return nullptr;
    if (n->left)
        return max(n->left);
    treeNode *y = n->parent;
    while (y && n == y->left) {
        n = y;
        y = y->parent;
    }
    return y;
}

void deleteNode(treeNode *&tr, treeNode *x) {
    if (!x)
        return;

    treeNode *parent_to_balance = nullptr;
    treeNode *node_to_delete = x;

    // Если у узла два потомка, находим преемника
    if (node_to_delete->left && node_to_delete->right) {
        treeNode *succ = min(node_to_delete->right);
        node_to_delete->inf = succ->inf; // Заменяем данные
        node_to_delete = succ;           // Удалять будем преемника
    }

    // У node_to_delete теперь не больше одного потомка
    treeNode *child =
        node_to_delete->left ? node_to_delete->left : node_to_delete->right;
    if (child) {
        child->parent = node_to_delete->parent;
    }

    if (!node_to_delete->parent) {
        tr = child; // Удаляем корень
        parent_to_balance = nullptr;
    } else {
        if (node_to_delete == node_to_delete->parent->left) {
            node_to_delete->parent->left = child;
        } else {
            node_to_delete->parent->right = child;
        }
        parent_to_balance = node_to_delete->parent;
    }

    delete node_to_delete;

    // Проходим вверх и балансируем дерево после удаления
    treeNode *curr = parent_to_balance;
    while (curr) {
        balance(tr, curr);
        curr = curr->parent;
    }
}

void preorder(treeNode *tr) {
    if (tr) {
        std::cout << tr->inf << " ";
        preorder(tr->left);
        preorder(tr->right);
    }
}

void postorder(treeNode *tr) {
    if (tr) {
        postorder(tr->left);
        postorder(tr->right);
        std::cout << tr->inf << " ";
    }
}

void inorder(treeNode *tr) {
    if (tr) {
        inorder(tr->left);
        std::cout << tr->inf << " ";
        inorder(tr->right);
    }
}

void setTree(treeNode *root, int row, int col, int colOffset) {
    if (!root)
        return;

    mvprintw(row, col, "%d", root->inf);

    if (root->left) {
        mvprintw(row + 1, col - 1, "/");
        setTree(root->left, row + 2, col - colOffset, colOffset / 2);
    }
    if (root->right) {
        mvprintw(row + 1, col + 1, "\\");
        setTree(root->right, row + 2, col + colOffset, colOffset / 2);
    }
}

void printTree(treeNode *root) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int startCol = COLS / 2;
    setTree(root, 2, startCol, startCol / 2);

    refresh();
    getch();
    erase();
    endwin();
}

} // namespace tree
