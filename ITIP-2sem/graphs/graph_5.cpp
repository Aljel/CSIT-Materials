#include <iostream>
#include <vector> 
#include <string>
#include <fstream>
#include <queue> 
#include <algorithm>

// функция для создания графа как списка смежности из файла 
std::vector<std::vector<int>> graphBuildFromFile(std::string filename){
    std::vector<std::vector<int>> adjlist;
    std::string line;

    std::ifstream in(filename);
    if (!in.is_open()){
        std::cout << "Cant find the input file!\n";
        return adjlist;}

        while (getline(in, line)) {
            if (line.empty()) continue;

            size_t colon_pos = line.find(':');
            int main_point = std::stoi(line.substr(0, colon_pos));
            
            // Убедимся, что вектор достаточно большой
            if (main_point >= adjlist.size()) {
                adjlist.resize(main_point + 1);
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
                adjlist[main_point].push_back(neighbor);
                pos = end_pos;
            }
        }
        in.close();
        return adjlist;
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

std::vector<int> bfs (std::vector<std::vector<int>> Gr, int start, int end){
    int n = Gr.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    std::vector<int> parent(n, -1); // Массив для восстановления пути
    visited[start] = 1;
    q.push(start);

    while(!q.empty()){
        int y = q.front(); // Извлекаем голову очереди
        q.pop();

        if (y == end) { // Если достигли целевой вершины
            // Восстанавливаем путь от end к x
            std::vector<int> shortest_path;
            for (int v = end; v != -1; v = parent[v]) {
                shortest_path.push_back(v);
            }
            std::reverse(shortest_path.begin(), shortest_path.end());
            return shortest_path;
        }

        for (int i = 0; i < Gr[y].size(); ++i) { // Цикл по соседям вершины y
            int neighbor = Gr[y][i];
            if (!visited[neighbor]) { // Если сосед не посещён
                visited[neighbor] = true; // Помечаем как посещённый
                parent[neighbor] = y; // Запоминаем родителя
                q.push(neighbor); // Помещаем в очередь
            }
        }
    }

    return {};
}


int main(){
    std::vector<std::vector<int>> Gr = graphBuildFromFile("input.txt");

    graphPrint(Gr);

    std::cout << "Enter two points\n";
    int A, B;
    std::cin >> A >> B;

    std::vector<int> path = bfs(Gr, A, B);

    if (path.empty())
        std::cout << "There is no way!\n";
    else{
        std::cout << "Shortest way is ";
        for (int a : path)
            std::cout << a << " ";
    }
    
    return 0;
}