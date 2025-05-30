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
    std::cout << "Enter the size of the table\n";
    int M;
    std::cin >> M;

    std::vector<people> A = makePeopleVec();

    std::vector<list*> T = createOpenHashTable(A, M);
    printOpenHashTable(T);

    std::cout<< "Enter people type guy to test deletion\n";
    people delguy = peopleMaker();
    deleteOpenHashTable(T, delguy);

    std::cout<< "Enter people type guy to test insertion\n";
    people insertguy = peopleMaker();
    insertOpenHashTable(T, insertguy);

    std::cout<< "Enter guy salary to test search\n";
    int salary;
    std::cin >> salary;
    people P = searchOpenHashTable(T, salary);
    if (P.surname != "mistake")
        printPeople(P);

    printOpenHashTable(T);

    return 0;
}