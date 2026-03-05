#include "trees.hpp"
#include <iostream>
#include <ncurses.h>

void testCout(tree::treeNode *tr) {
    std::cout << "\nINORDER\n";
    tree::inorder(tr);

    std::cout << "\nPREORDER\n";
    tree::preorder(tr);

    std::cout << "\nPOSTORDER\n";
    tree::postorder(tr);
    std::cout << "\n";
    printTree(tr);
    getch();
    refresh();
    endwin();
}

int main() {
    int n, x;
    std::cout << "Enter the number of nodes\n";
    std::cin >> n;
    tree::treeNode *tr = nullptr;

    std::cout << "Enter nodes\n";
    for (int i = 0; i < n; i++) {
        std::cin >> x;
        tree::insert(tr, x);
    }

    tree::treeNode *elem = tree::find(tr, 9);
    tree::Delete(tr, elem);

    testCout(tr);

    endwin(); // Завершение работы с ncurses
    return 0;
}
