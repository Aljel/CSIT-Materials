#include "tree.h"
#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>

// ДЛЯ КОМПИЛЯЦИИ ИСПОЛЬЗУЙ КОМАНДУ:
// g++ tree_1.cpp -Iinclude -L. -ltree -lncurses -o tree_1
// ./tree_1

// Функция для расчета глубины дерева
int treeDepth(tree* root) {
    if (!root) return 0;
    return 1 + std::max(treeDepth(root->left), treeDepth(root->right));
}

// Функция для подготовки вывода дерева
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

// Функция для поиска пути от узла до корня
std::vector<int> pathToRoot(tree* node){
    std::vector<int> path;
    while (node) {
        path.push_back(node->inf);
        node = node->parent;
    }
    return path;
}

// Функция для нахождения длины пути между двумя узлами
int findPathLength(tree* root, int a, int b){
    tree* nodeA = find(root, a);
    tree* nodeB = find(root, b);
    if (!nodeA || !nodeB) 
        return -1; // один из узлов не найден
    
    std::vector<int> pathA = pathToRoot(nodeA);
    std::vector<int> pathB = pathToRoot(nodeB);
    
    // Проверка на нахождение в одном поддереве 
    if (!find(nodeA, b) && !find(nodeB,a))
        return -1;
    // Находим общего предка
    int i = pathA.size() - 1;
    int j = pathB.size() - 1;
    
    while (i >= 0 && j >= 0 && pathA[i] == pathB[j]) {
        i--;
        j--;
    }

    // Длина пути = (путь от A до LCA) + (путь от B до LCA)
    return (i + 1) + (j + 1);
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

    std::cout <<"Enter two nodes\n";
    std::cin >> a >> b;

    printTree(tr);

    int pathLength = findPathLength(tr, a, b);
    if (pathLength == -1){
        mvprintw(depth, 0, "There is no way\n");
        if (!find(tr, a) && !find(tr,b))
            mvprintw(depth + 1, 0, "Because both nodes are not in the tree\n");
        else if (!find(tr, a))
            mvprintw(depth + 1, 0, "Because first node is missing\n");
        else if (!find(tr,b))
            mvprintw(depth + 1, 0, "Because second node is missing\n");
        else 
            mvprintw(depth + 1, 0, "Because nodes are on different sides of the tree\n");
    }
    else
        mvprintw(depth, 0, "The length of the way is %d", pathLength);
    
    refresh();  // Обновить экран
    getch();    // Ожидание нажатия клавиши
    endwin();   // Завершение работы с ncurses
    return 0;
}