#include <iostream>
#include <vector> 
#include <string>
#include <fstream>

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


int main(){
    std::vector<std::vector<int>> Gr = graphBuildFromFile("input.txt");

    graphPrint(Gr);

    int point1, point2;
    std::cout << "Enter the points\n";
    std::cin >> point1 >> point2;

    Gr[point1].insert(std::lower_bound(Gr[point1].begin(), Gr[point1].end(), point2), point2);
    Gr[point2].insert(std::lower_bound(Gr[point2].begin(), Gr[point2].end(), point1), point1);

    graphPrint(Gr);
    return 0;
}