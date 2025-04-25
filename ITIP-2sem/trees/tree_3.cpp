#include "tree.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ncurses.h>

// ДЛЯ КОМПИЛЯЦИИ ИСПОЛЬЗУЙ КОМАНДУ:
// g++ tree_3.cpp -Iinclude -L. -ltree -lncurses -o tree_3
// ./tree_3

// Функция для расчета глубины дерева
int treeDepth(tree* root) {
    if (!root) return 0;
    return 1 + std::max(treeDepth(root->left), treeDepth(root->right));
}

void setTree(tree* root, int row, int col, int colOffset) {
    if (!root) return;
    // Отрисовка текущего узла
    mvprintw(row, col, "%d", root->inf);
    // Расчет позиций потомков
    if (root->left) {
        mvprintw(row+1, col-1, "/");
        setTree(root->left, row+2, col-colOffset, colOffset/2);
    }
    if (root->right) {
        mvprintw(row+1, col+1, "\\");
        setTree(root->right, row+2, col+colOffset, colOffset/2);
    }
}

// Функция для вывода дерева
void printTree(tree* root) {
    initscr();  // Инициализация ncurses
    cbreak();   // Режим cbreak
    noecho();   // Не отображать ввод
    curs_set(0); // Скрыть курсор

    // int depth = treeDepth(root);
    int startCol = COLS / 8;  // Начинаем с середины экрана
    setTree(root, 2, startCol, startCol/2);
}

// Функция для вывода списка
void printVec(std::vector<int> b){
    for (int i = 0; i < b.size(); i++)
        std::cout << b[i];
    std:: cout << "\n";
}

void collectLeaves(tree* tr, std::vector<tree*>& leaves) {
    if (!tr) return;
    if (!tr->left && !tr->right) {
        leaves.push_back(tr);
    }
    collectLeaves(tr->left, leaves);
    collectLeaves(tr->right, leaves);
}


int main(){
    int n, x, a, b;
    std::cout <<"Enter the number of nodes\n";
    std::cin >> n;
    tree *tr = NULL;
    std::cout <<"Enter nodes\n";
    for (int i = 0; i < n; i++){
        std::cin >> x;
        insert(tr,x);
    }
    
    int depth = treeDepth(tr) * 2 + 3;

    std::vector<tree*> leaves;
    collectLeaves(tr, leaves);
    sort(leaves.begin(), leaves.end(), [](tree* a, tree* b) {return a->inf < b->inf;}); // компаратор сортирующий по возрастанию
    tree* middleLeaf = leaves[leaves.size() / 2];

    printTree(tr);
    mvprintw(0, 0, "first tree");
    mvprintw(depth, 0, "middle leaf is %d", middleLeaf->inf);
    refresh();  // Обновить экран
    getch();    // Ожидание нажатия клавиши
    clear();
    endwin();   // Завершение работы с ncursess

    if (!middleLeaf->parent || !middleLeaf->parent->parent) {
        std::cout << "There is no grandpa :(\n";
        return 0;
    }

    Delete(tr, middleLeaf->parent->parent);
    printTree(tr);
    mvprintw(0, 0, "second tree. Dont mind if its dead");
    std::cout << "yep, thats it\n";

    refresh();  // Обновить экран
    getch();    // Ожидание нажатия клавиши
    endwin();   // Завершение работы с ncursess
    return 0;
}