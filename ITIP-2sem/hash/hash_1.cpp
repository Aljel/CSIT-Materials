#include "hash_lib.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

int main() {
    std::cout << "Enter the size of the table\n";
    int M;
    std::cin >> M;

    std::vector<people> A = makePeopleVec();
    for (int i = 0; i < A.size(); i++){
        printPeople(A[i]);
    }

    return 0;
}
 