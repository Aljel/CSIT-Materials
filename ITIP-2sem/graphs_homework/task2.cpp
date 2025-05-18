#include <iostream>
#include <vector> 
#include <string>
#include <fstream>
#include <stack>
#include <algorithm>

std::vector<std::vector<int>> graphBuildFromFile(std::string filename){
    std::vector<std::vector<int>> Grlist;
    std::string line;

    std::ifstream in(filename);
    if (!in.is_open()){
        std::cout << "Cant find the input file!\n";
        return Grlist;}

        while (getline(in, line)) {
            if (line.empty()) continue;

            size_t colon_pos = line.find(':');
            int main_point = std::stoi(line.substr(0, colon_pos));
            
            // Убедимся, что вектор достаточно большой
            if (main_point >= Grlist.size()) {
                Grlist.resize(main_point + 1);
            }
    
            size_t pos = colon_pos + 1;
            while (pos < line.size()) {
                // Пропускаем пробелы
                while (pos < line.size() && line[pos] == ' ') pos++;
                if (pos >= line.size()) break;

                // Находим конец числа
                size_t end_pos = pos;
                while (end_pos < line.size() && line[end_pos] != ' ') end_pos++;

                // Извлекаем число
                int neighbor = std::stoi(line.substr(pos, end_pos - pos));
                Grlist[main_point].push_back(neighbor);
                pos = end_pos;
            }
        }
        in.close();
        return Grlist;
}

// Вывод списка смежности
void graphPrint(std::vector<std::vector<int>> Gr){
    int N = Gr.size();
    for (int i = 0; i < N; i++){
        std::cout << i << ": ";
        for(auto j = Gr[i].begin(); j != Gr[i].end(); j++)
            std::cout << *j << " ";
        std::cout << "\n";
    }
}

// Проверка на циклы
bool cycleCheck(int current, std::vector<std::vector<int>>& Gr, std::vector<bool>& visited, std::vector<bool>& visited2, int last) {
    visited[current] = true;
    visited2[current] = true;

    for (int neighbor : Gr[current]) { //проходимся по вершинам, в которые можем попасть из данной 
        if (visited2[neighbor] && neighbor != last)
            return true;
        if (!visited[neighbor]) 
            cycleCheck(neighbor, Gr, visited, visited2, current);
    }
    return false;
}

// обход в глубину с сохранением порядка посещений
void dfs(int current, const std::vector<std::vector<int>>& Gr, std::vector<bool>& visited, std::vector<int>& order) {
    visited[current] = true;
    for (int neighbor : Gr[current]) { //проходимся по вершинам, в которые можем попасть из данной 
        if (!visited[neighbor]) {
            dfs(neighbor, Gr, visited, order);
        }
    }
    order.push_back(current);
}

// Топологическая сортировка
std::vector<int> topologicalSort(std::vector<std::vector<int>>& Gr) {
    int N = Gr.size();
    std::vector<bool> visited(N, false);
    std::vector<int> order;

    // Запускаем DFS для всех непосещенных вершин
    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            dfs(i, Gr, visited, order);
        }
    }
    // Разворачиваем порядок, так как вершины добавлялись в обратном порядке
    reverse(order.begin(), order.end());

    return order;
}

int main(){
    std::vector<std::vector<int>> Gr = graphBuildFromFile("input.txt");
    int N = Gr.size();


    std::vector<bool> visited(N, false), visited2(N, false);
    if (cycleCheck(Gr[0][0], Gr, visited, visited2, -1)){
        std::cout<< "There is a cycle. Sort is impossible!\n";
        return 0;
    }

    std::vector<int> sorted = topologicalSort(Gr);
    std::cout<<"Result: ";
    for (int i : sorted)
        std::cout<< i << " ";
    
    return 0;
}