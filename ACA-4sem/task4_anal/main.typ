#import "conf.typ" : conf
#show: conf.with(
  title: [Анализ сложности операция красно-чёрного дерева],
  type: "referat",
  info: (
      author: (
        name: [Железко Александра Дмитриевича],
        faculty: [КНиИТ],
        group: "251",
        sex: "male"
      ),
      inspector: (
        degree: "Старший преподаватель",
        name: "М. И. Сафрончик"
      )
  ),
  settings: (
    title_page: (
      enabled: true
    ),
    contents_page: (
      enabled: true
    )
  )
)

= Код программы

```cpp
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
    } else if (x >= prev->inf && !prev->right) {
        prev->right = createNode(prev, x);
        insertNode1(tr, prev->right);
    } else if (x < prev->inf && prev->left) {
        insertNodePrep(tr, prev->left, x);
    } else if (x >= prev->inf && prev->right) {
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

// родитель черный, сыновья брата черные, брат красный
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

// если узел, брат, родитель и дети брата черные
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

// родитель красный, дети брата черные
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

// если брат черный, дети его разноцветные, прилежащий племянник красный
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

// брат черный, противоположные племянник красный
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
```

= Анализ сложности

== Общие сведения

В дальнейших рассуждениях часто будет фигурировать высота дерева. Она не больше $2 log (n+1)$, где $n$ --- количество узлов. Почему?

Рассмотрим красно-чёрное дерево с высотой $h$. Так как у красной вершины чёрные дети количество красных вершин не больше $h/2$. Тогда чёрных вершин не меньше, чем $h/2 − 1$.

Для количества внутренних вершин в дереве $n$ выполняется неравенство:
$N >= 2^(h/2)−1$. Прологарифмировав неравенство, имеем:
$log(N+1)⩾ h 2$; $2log(N+1)⩾h$; $h⩽2log(N+1)$.

Операции поворотов выполняются за $O(1)$, так как просто меняют указатели определенно выбранных узлов. Также и перекрашивание узла.

== Вставка узла

При вставке узла сначала происходит сравнение элементов. Так как высота дерева не больше $2log (n+1)$, сложность нахождения места для вставки $O(log n)$. Далее идет восстановление свойств красно-чёрного дерева.

Оно представляет из себя комбинации перекрашивания и поворотов, без рекурсивных вызовов и просмотра ветвей дерева полностью, а то есть происходит за $O(1)$, вне зависимости от случая. Итоговая сложность вставки для всех случаем получается $O(log n)$

== Удаление узла

При удалении узла сначала происходит сравнение элементов. Так как высота дерева не больше $2log (n+1)$, сложность нахождения места для вставки $O(log n)$. Сама операция удаления занимает $O(1)$ (просто смена указателей и освобождение памяти). В случае наличия у узла обоих предков, происходит его замена на минимальное из правого поддерева или на максимальное из левого. Операция его нахождения также проходит за $O(log n)$. Сама замена происходит за $O(1)$.

Далее идет восстановление свойств красно-чёрного дерева.
Оно представляет из себя комбинации перекрашивания и поворотов, без рекурсивных вызовов и просмотра ветвей дерева полностью, а то есть происходит за $O(1)$, вне зависимости от случая. Итоговая сложность вставки для всех случаем получается $O(log n)$

== Поиск узла 

Для нахождения узла в дереве, высота которого гарантированно не больше $2log (n+1)$, средняя и худшая сложность будет $O(log n)$. В лучшем случае искомый элемент --- корень и сложность $O(1)$.

== Обходы

Подобно дереву бинарного поиска, сложность каждого обхода будет линейной, $O(n)$, так как:
1. Каждая вершина посещается ровно один раз.
2. В каждом узле выполняется константное количество действий (вывод значения и два рекурсивных вызова).

== Память 

Для хранения самого дерева требуется $O(n)$ памяти. Каждый узел занимает фиксированный объем: значение (`inf`) и три указателя (`left`, `right`, `parent`). В сумме это $n dot ("размер узла")$.

=== Операции (Поиск, Вставка, Удаление)

Функции `insert`, `find` и `delete` реализованы рекурсивно, из-за чего занимают память для стека вызова. Так как высота дерева не больше $2log (n+1)$, память, поддерживающая стек вызовов для этих функций $O(log n)$.

=== Обходы

Поскольку обходы реализованы рекурсивно, память тратится на поддержку стека вызовов функций. Так как высота дерева не больше $2log (n+1)$, память, поддерживающая стек вызовов для этих функций $O(log n)$.
