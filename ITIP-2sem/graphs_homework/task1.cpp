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


int main(){
    std::vector<std::vector<int>> Gr = graphBuildFromFile("input.txt");
    
    int N = Gr.size();
    std::vector<int> powers(N);
    std::vector<int> res;
    std::stack<int> st;

    for (int i = 0; i < N; i++)
        powers[i] = Gr[i].size();
    
    int v1 = -1, v2 = -1; 

    // Находим вершины с нечетными степенями
    for (int i = 0; i < N; i++){
        if (powers[i] % 2 != 0){
            if (v1 == -1)
                v1 = i;
            else if (v2 == -1)
                v2 = i;
            else{
                std::cout << "There is no cycle!\n";
                return 0;
            }
        }
    }

    bool isCycle = (v1 == -1 && v2 == -1);

    // Если есть две вершины с нечетными степенями, добавляем фиктивное ребро
    bool hasFakeEdge = false;
    if (v1 != -1 && v2 != -1){
        Gr[v1].push_back(v2);
        Gr[v2].push_back(v1);
        powers[v1]++;
        powers[v2]++;
        hasFakeEdge = true;
    }
    else if (v1 != -1)
        // Граф имеет только одну вершину с нечетной степенью, Эйлерова цикла нет
        return {};

    // Начинаем с вершины v1, если она есть, иначе с 0
    int startVertex = (v1 != -1) ? v1 : 0;
    st.push(startVertex);

    while (!st.empty()) {
        int current = st.top();
        if (powers[current] != 0){
            int next = Gr[current].back(); // Ищем смежную вершину
            Gr[current].pop_back();

            auto it = std::find(Gr[next].begin(), Gr[next].end(), current); // Удаляем ребро из смежной вершины
            if (it != Gr[next].end())
                Gr[next].erase(it);

            powers[current]--;
            powers[next]--;
            st.push(next);
        } 
        else{
            res.push_back(current);
            st.pop();
        }
    }

     // Если было добавлено фиктивное ребро, корректируем результат
    if (hasFakeEdge) {
        for (int i = 0; i + 1 < res.size(); i++) {
            if ((res[i] == v1 && res[i+1] == v2) || (res[i] == v2 && res[i+1] == v1)) {
                std::vector<int> newRes;
                newRes.insert(newRes.end(), res.begin() + i + 1, res.end());
                newRes.insert(newRes.end(), res.begin(), res.begin() + i + 1);
                res = newRes;
                break;
            }
        }
        // Удаляем последний элемент, так как он дублирует первый
        if (!res.empty() && res.front() == res.back()) {
            res.pop_back();
        }
    }

    // Проверяем, остались ли ребра в графе (граф несвязный)
    for (int i = 0; i < N; i++)
        if (powers[i] != 0)
            return 0;

    if (isCycle)
        std::cout<< "Resulting cycle: ";
    else
        std::cout<< "Resulting way: ";
    for (int i : res){
        std::cout<< i << " ";
    }

    return 0;
}