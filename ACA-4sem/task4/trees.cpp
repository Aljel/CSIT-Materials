#include "trees.hpp"
#include <iostream>
#include <ncurses.h>
namespace tree {

// создание узла дерева
treeNode *createNode(treeNode *p, int x) {
    treeNode *n = new treeNode;
    n->inf = x;
    n->col = 'r';
    n->left = n->right = nullptr;
    n->parent = p;
    return n;
}

// создание корня дерева
treeNode *createRoot(int x) {
    treeNode *n = new treeNode;
    n->inf = x;
    n->col = 'b';
    n->left = n->right = n->parent = nullptr;
    return n;
}

void rotateRight(treeNode *&root, treeNode *x) {
    treeNode *y = x->left;

    // делаем правого ребенка Y левым ребенком X
    x->left = y->right;
    if (y->right)
        x->left->parent = x;

    y->parent = x->parent;

    // делаем подмену сына: Y становится ребенком отца X
    // если у X не было отца => он был корнем => им становится Y
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else if (x->parent->right == x)
            x->parent->right = y;
    } else {
        y->col = 'b';
        root = y;
    }

    // Y становится отцом X (сын стал папой папы)
    x->parent = y;
    y->right = x;
}

void rotateLeft(treeNode *&root, treeNode *x) {
    treeNode *y = x->right;

    // делаем левого ребенка Y правым ребенком X
    x->right = y->left;
    if (y->left)
        x->right->parent = x;

    y->parent = x->parent;

    // делаем подмену сына: Y становится ребенком отца X
    // если у X не было отца => он был корнем => им становится Y
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else if (x->parent->right == x)
            x->parent->right = y;
    } else {
        y->col = 'b';
        root = y;
    }

    // Y становится отцом X (сын стал папой папы)
    x->parent = y;
    y->left = x;
}

// папа папы
treeNode *grandfather(treeNode *x) {
    if (x && x->parent)
        return x->parent->parent;
    return nullptr;
}

// брат папы
treeNode *uncle(treeNode *x) {
    treeNode *g = grandfather(x);
    if (!g)
        return nullptr;
    if (x->parent == g->left)
        return g->right;
    return g->left;
}

// другой сын родителя
treeNode *sibling(treeNode *x) {
    if (x && x->parent) {
        if (x == x->parent->left)
            return x->parent->right;
        return x->parent->left;
    }
    return nullptr;
}

void insertNode5(treeNode *&tr, treeNode *x);
void insertNode4(treeNode *&tr, treeNode *x);
void insertNode3(treeNode *&tr, treeNode *x);
void insertNode2(treeNode *&tr, treeNode *x);
void insertNode1(treeNode *&tr, treeNode *x);

// родитель красный, дядя черный. Дед, брат и дети черные
void insertNode5(treeNode *&tr, treeNode *x) {
    treeNode *g = grandfather(x);
    treeNode *p = x->parent;
    p->col = 'b';
    g->col = 'r';
    if (x == p->left && p == g->left)
        rotateRight(tr, g);
    else
        rotateLeft(tr, g);
}

// родитель красный, дядя черный
void insertNode4(treeNode *&tr, treeNode *x) {
    treeNode *g = grandfather(x);
    treeNode *p = x->parent;
    if (x == p->right && p == g->left) {
        rotateLeft(tr, p);
        x = x->left;
    } else if (x == p->left && p == g->right) {
        rotateRight(tr, p);
        x = x->right;
    }
    insertNode5(tr, x);
}

// родитель и дядя красные
void insertNode3(treeNode *&tr, treeNode *x) {
    treeNode *u = uncle(x);
    treeNode *g = grandfather(x);
    treeNode *p = x->parent;
    if (u && u->col == 'r' && p->col == 'r') {
        p->col = 'b';
        u->col = 'b';
        g->col = 'r';
        insertNode1(tr, g);
    } else
        insertNode4(tr, x);
}

// родитель красный
void insertNode2(treeNode *&tr, treeNode *x) {
    if (x->parent->col == 'r')
        insertNode3(tr, x);
}

// проверка на корень
void insertNode1(treeNode *&tr, treeNode *x) {
    if (!x->parent)
        x->col = 'b';
    else
        insertNode2(tr, x);
}

void insertNodePrep(treeNode *&tr, treeNode *prev, int x) {
    if (x < prev->inf && !prev->left) {
        prev->left = createNode(prev, x);
        insertNode1(tr, prev->left);
    } else if (x > prev->inf && !prev->right) {
        prev->right = createNode(prev, x);
        insertNode1(tr, prev->right);
    } else if (x < prev->inf && prev->left) {
        insertNodePrep(tr, prev->left, x);
    } else if (x > prev->inf && prev->right) {
        insertNodePrep(tr, prev->right, x);
    }
}

void insertNode(treeNode *&tr, int x) { insertNodePrep(tr, tr, x); }

void deleteNode1(treeNode *&tr, treeNode *x);
void deleteNode2(treeNode *&tr, treeNode *x);
void deleteNode3(treeNode *&tr, treeNode *x);
void deleteNode4(treeNode *&tr, treeNode *x);
void deleteNode5(treeNode *&tr, treeNode *x);
void deleteNode6(treeNode *&tr, treeNode *x);

// x - корень дерева, одна ветка
void deleteNode1(treeNode *&tr, treeNode *x) {
    if (!x->parent) {
        if (x->left)
            tr = x->left;
        else
            tr = x->right;
        tr->col = 'b';
    } else
        deleteNode2(tr, x);
}

void deleteNode2(treeNode *&tr, treeNode *x) {
    treeNode *s = sibling(x);
    treeNode *p = x->parent;
    if (s->col == 'r') {
        p->col = 'r';
        s->col = 'b';
        if (p->left == x)
            rotateLeft(tr, p);
        else
            rotateRight(tr, p);
    }
    deleteNode3(tr, x);
}

void deleteNode3(treeNode *&tr, treeNode *x) {
    treeNode *s = sibling(x);
    treeNode *p = x->parent;
    if (p->col == 'b' && s->col == 'b' && (!s->left || s->left->col == 'b') &&
        (!s->right || s->right->col == 'b')) {
        s->col = 'r';
        deleteNode1(tr, x);
    } else
        deleteNode4(tr, x);
}

void deleteNode4(treeNode *&tr, treeNode *x) {
    treeNode *s = sibling(x);
    treeNode *p = x->parent;
    if (p->col == 'r' && s->col == 'b' && (!s->left || s->left->col == 'b') &&
        (!s->right || s->right->col == 'b')) {
        p->col = 'b';
        s->col = 'r';
    } else
        deleteNode5(tr, x);
}

void deleteNode5(treeNode *&tr, treeNode *x) {
    treeNode *s = sibling(x);
    treeNode *p = x->parent;
    if (s->col == 'b') {
        if (x == p->left && (s->left && s->left->col == 'r') &&
            (!s->right || s->right->col == 'b')) {
            s->col = 'r';
            s->left->col = 'b';
            rotateRight(tr, s);
        } else if (x == p->right && (s->right && s->right->col == 'r') &&
                   (!s->left || s->left->col == 'b')) {
            s->col = 'r';
            s->right->col = 'b';
            rotateLeft(tr, s);
        }
    }
    deleteNode6(tr, x);
}

void deleteNode6(treeNode *&tr, treeNode *x) {
    treeNode *s = sibling(x);
    treeNode *p = x->parent;
    s->col = p->col;
    p->col = 'b';
    if (x == p->left) {
        s->right->col = 'b';
        rotateLeft(tr, p);
    } else {
        s->left->col = 'b';
        rotateRight(tr, p);
    }
}

void replace(treeNode *&tr, treeNode *x) {
    treeNode *p = x->parent;
    if (x->left) {
        treeNode *ch = x->left;
        ch->parent = p;
        if (p) {
            if (x == p->left)
                p->left = ch;
            else
                p->right = ch;
        }
    } else {
        treeNode *ch = x->right;
        ch->parent = p;
        if (p) {
            if (x == p->left)
                p->left = ch;
            else
                p->right = ch;
        }
    }
}

treeNode *min(treeNode *tr) {
    if (!tr->left) {
        return tr;
    } else {
        return min(tr->left);
    }
}

treeNode *max(treeNode *tr) {
    if (!tr->right) {
        return tr;
    } else {
        return max(tr->right);
    }
}

void deleteNode(treeNode *&tr, treeNode *x) {
    treeNode *buf = nullptr;
    if (x->right && x->left) {
        if (x->inf <= tr->inf)
            buf = max(x->left);
        else
            buf = min(x->right);
        std::swap(x->inf, buf->inf);
        x = buf;
    }
    if (x->right || x->left) {
        treeNode *ch = nullptr;
        if (x->left && !x->right)
            ch = x->left;
        if (x->right && !x->left)
            ch = x->right;
        replace(tr, x);
        if (x->col == 'b') {
            if (ch->col == 'r')
                ch->col = 'b';
            else
                deleteNode1(tr, x);
        }
    } else {
        if (x->col == 'b')
            deleteNode1(tr, x);
        else {
            treeNode *p = x->parent;
            if (x == p->left)
                p->left = nullptr;
            else
                p->right = nullptr;
        }
    }
    if (!x->left && !x->right) {
        treeNode *p = x->parent;
        if (x == p->left)
            p->left = nullptr;
        else if (x == p->right)
            p->right = nullptr;
    }
    delete x;
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

treeNode *Next(treeNode *tr, int x) {
    treeNode *n = find(tr, x);
    if (n->right) {
        return min(n->right);
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
        return max(n->left);
    }
    treeNode *y = n->parent;
    while (y && n == y->left) {
        n = y;
        y = y->parent;
    }
    return y;
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
    // Выбираем цветовую пару: 1 для красного, 2 для черного (белый цвет)
    int color_pair = (root->col == 'r') ? 1 : 2;
    // Включаем нужный цвет
    attron(COLOR_PAIR(color_pair));
    mvprintw(row, col, "%d", root->inf);
    // Выключаем цвет, чтобы ветви (линии) рисовались стандартным цветом
    attroff(COLOR_PAIR(color_pair));
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
    // Инициализация цветов
    if (has_colors()) {
        start_color();
        // Создаем цветовые пары (ID, цвет_текста, цвет_фона)
        init_pair(1, COLOR_RED, COLOR_BLACK); // Красный текст на черном фоне
        init_pair(
            2, COLOR_WHITE,
            COLOR_BLACK); // Белый текст на черном фоне (для "черных" узлов)
    }
    // int depth = treeDepth(root);
    int startCol = COLS / 2;

    setTree(root, 2, startCol, startCol / 2);

    refresh(); // Обновляем экран, чтобы показать нарисованное
    getch();   // Ждем нажатия любой клавиши, чтобы окно не закрылось сразу
    endwin();  // Корректно завершаем работу ncurses
}

} // namespace tree
