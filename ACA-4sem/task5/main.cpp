#include "trees.hpp"
#include <iostream>

void testCout(tree::treeNode *tr) {
    std::cout << "\nINORDER\n";
    tree::inorder(tr);

    std::cout << "\nPREORDER\n";
    tree::preorder(tr);

    std::cout << "\nPOSTORDER\n";
    tree::postorder(tr);
    std::cout << "\n";

    std::cout << "\nPress Enter to view the tree...";
    std::cin.ignore(10000, '\n');
    std::cin.get();

    tree::printTree(tr);
}

int main() {
    int n, x;
    std::cout << "Enter the number of nodes\n";
    std::cin >> n;

    if (n <= 0)
        return 0;

    std::cout << "Enter nodes\n";
    std::cin >> x;
    tree::treeNode *tr = tree::createRoot(x);
    for (int i = 0; i < n - 1; i++) {
        std::cin >> x;
        tree::insertNode(tr, x);
    }

    testCout(tr);

    tree::treeNode *elem = tree::find(tr, 17);
    if (elem) {
        tree::deleteNode(tr, elem);
    } else {
        std::cout << "Node not found. Dummy user\n";
    }

    testCout(tr);

    return 0;
}
