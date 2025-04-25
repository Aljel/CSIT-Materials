#include "tree.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ncurses.h>

// ДЛЯ КОМПИЛЯЦИИ ИСПОЛЬЗУЙ КОМАНДУ:
// g++ tree_2.cpp -Iinclude -L. -ltree -lncurses -o tree_2
// ./tree_2

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

int sumEvenLeaves(tree* tr){
    if (!tr) return 0;
    // Если это лист (нет детей)
    if (!tr->left && !tr->right) {
        if (tr->inf % 2 == 0) {
            return tr->inf;
        }
        return 0;
    }
    
    // Рекурсивно суммируем четные листья в левом и правом поддеревьях
    return sumEvenLeaves(tr->left) + sumEvenLeaves(tr->right);
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
    printTree(tr);

    if (sumEvenLeaves(tr) == 0)
        mvprintw(depth, 0, "There is no even leaves\n");
    else{
        mvprintw(depth, 0, "Sum of even leaves: %d", sumEvenLeaves(tr));
    }

    refresh();  // Обновить экран
    getch();    // Ожидание нажатия клавиши
    endwin();   // Завершение работы с ncurses
    return 0;
}