#include "hash_lib.h"
#include <iostream>
#include <string>
#include <vector>

people peopleMaker(){
    people guy;
    std::cout<< "Enter guy's surname\n";
    std::cin >> guy.surname;
    std::cout<< "Enter guy's work\n";
    std::cin >> guy.work;
    std::cout<< "Enter guy's date of birth\n";
    std::string date;
    std::cin >> date;
    guy.dateOfBirth = strToDate(date);
    std::cout<< "Enter guy's time of work\n";
    std::cin >> guy.timeOfWork;
    std::cout<< "Enter guy's salary\n";
    std::cin >> guy.salary;

    return guy;
}

int main() {
    std::cout << "Enter the size of the table and c_1, c_2 for hash function\n";
    int M, c1, c2;
    std::cin >> M >> c1 >> c2;

    std::vector<people> A = makePeopleVec();

    std::vector<people> T = createClosedHashTable(A, M, c1, c2);
    printClosedHashTable(T);

    std::cout<< "Enter people type guy to test insertion\n";
    people insertguy = peopleMaker();
    insertClosedHashTable(T, insertguy, M, c1, c2);

    std::cout<< "Enter guy salary to test search\n";
    int salary;
    std::cin >> salary;
    people P = searchClosedHashTable(T, salary, M, c1, c2);
    if (P.surname != "mistake")
        printPeople(P);

    printClosedHashTable(T);

    return 0;
}