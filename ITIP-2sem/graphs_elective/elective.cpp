#include <iostream>
#include <vector>
#include <string>
#include <queue>       // Для priority_queue
#include <climits>     // Для INT_MAX
#include <algorithm>   // Для reverse
#include <fstream>     // Для работы с файлами

/**
 * Структура для хранения пар (расстояние, вершина) 
 * Используется в приоритетной очереди алгоритма Дейкстры
 */
struct pair {
    int first;   // Расстояние (для приоритетной очереди)
    int second;  // Номер вершины
};

/**
 * Оператор сравнения для структуры pair
 * Необходим для корректной работы priority_queue
 */
bool operator>(const pair& a, const pair& b) {
    return a.first > b.first;
}

/**
 * Структура для хранения информации о ребре графа
 * point - вершина, в которую ведет ребро
 * weight - вес ребра
 */
struct Edge {
    int point;
    int weight;
};

/**
 * Перегрузка оператора вывода для структуры Edge
 * Позволяет выводить ребра в формате (point, weight)
 */
std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << "(" << e.point << ", " << e.weight << ")";
    return os;
}

// Функция для построения графа из файла
std::vector<std::vector<Edge>> graphBuildFromFile(std::string filename) {
    std::vector<std::vector<Edge>> adjlist;  // Список смежности
    std::ifstream in(filename);              // Поток для чтения файла
    
    // Проверка открытия файла
    if (!in.is_open()) {
        std::cout << "Can't find the input file!\n";
        return adjlist;
    }

    std::string line;
    // Построчное чтение файла
    while (getline(in, line)) {
        if (line.empty()) continue;  // Пропускаем пустые строки

        // Поиск разделителя ':'
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) {
            std::cerr << "Invalid line format: " << line << "\n";
            continue;
        }

        // Получаем номер вершины
        int main_point = std::stoi(line.substr(0, colon_pos));
        // Увеличиваем размер списка при необходимости
        if (main_point >= adjlist.size()) 
            adjlist.resize(main_point + 1);

        size_t pos = colon_pos + 1;  // Позиция после ':'
        
        // Обработка списка рёбер
        while (pos < line.size()) {
            // Пропускаем пробелы
            while (pos < line.size() && line[pos] == ' ') 
                pos++;
            if (pos >= line.size()) 
                break;

            // Проверяем формат - должно быть '('
            if (line[pos] != '(') {
                std::cerr << "Expected '(' at position " << pos << " in line: " << line << "\n";
                break;
            }
            pos++; // Пропускаем '('

            // Извлекаем номер соседней вершины
            size_t comma_pos = line.find(',', pos);
            if (comma_pos == std::string::npos) {
                std::cerr << "Expected ',' in line: " << line << "\n";
                break;
            }
            Edge edge;
            edge.point = std::stoi(line.substr(pos, comma_pos - pos));
            pos = comma_pos + 1;

            // Пропускаем пробелы после запятой
            while (pos < line.size() && line[pos] == ' ') 
                pos++;

            // Извлекаем вес ребра
            size_t closing_brace_pos = line.find(')', pos);
            if (closing_brace_pos == std::string::npos) {
                std::cerr << "Expected ')' in line: " << line << "\n";
                break;
            }
            edge.weight = std::stoi(line.substr(pos, closing_brace_pos - pos));
            pos = closing_brace_pos + 1;

            // Добавляем ребро в список смежности
            adjlist[main_point].push_back(edge);
        }
    }
    in.close();
    return adjlist;
}

/**
 * Алгоритм Дейкстры для поиска кратчайших путей
 * @param graph Граф в виде списка смежности
 * @param start Стартовая вершина
 * @param distances Вектор для хранения расстояний (будет заполнен)
 * @param predecessors Вектор для хранения предшественников (будет заполнен)
 */
void dijkstra(const std::vector<std::vector<Edge>>& graph, int start, 
              std::vector<int>& distances, std::vector<int>& predecessors) {
    int n = graph.size();
    distances.assign(n, INT_MAX);  // Изначально все расстояния "бесконечны"
    predecessors.assign(n, -1);    // Предшественники неизвестны
    distances[start] = 0;          // Расстояние до стартовой вершины = 0

    // Приоритетная очередь для выбора вершины с минимальным расстоянием
    std::priority_queue<pair, std::vector<pair>, std::greater<pair>> pq;
    pq.push({0, start});  // Добавляем стартовую вершину

    while (!pq.empty()) {
        int current_dist = pq.top().first;  // Текущее минимальное расстояние
        int u = pq.top().second;            // Номер вершины
        pq.pop();

        // Если нашли более короткий путь - пропускаем
        if (current_dist > distances[u]) 
            continue;

        // Обход всех соседей текущей вершины
        for (const Edge& edge : graph[u]) {
            int v = edge.point;
            int weight = edge.weight;

            // Если найден более короткий путь до соседа
            if (distances[v] > distances[u] + weight) {
                distances[v] = distances[u] + weight;  // Обновляем расстояние
                predecessors[v] = u;                  // Запоминаем предшественника
                pq.push({distances[v], v});           // Добавляем в очередь
            }
        }
    }
}

// Восстановление пути от стартовой вершины до заданной
std::vector<int> reconstructPath(int start, int end, const std::vector<int>& predecessors) {
    std::vector<int> path;
    // Идем от конечной вершины к начальной по цепочке предшественников
    for (int v = end; v != -1; v = predecessors[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());  // Разворачиваем путь
    
    // Проверяем, что путь начинается со стартовой вершины
    if (path.empty() || path[0] != start) {
        return {};  // Путь не существует
    }
    return path;
}

int main() {
    // Построение графа из файла
    std::vector<std::vector<Edge>> graph = graphBuildFromFile("input.txt");
    if (graph.empty()) {
        std::cerr << "Error: Graph is empty" << std::endl;
        return 1;
    }
    
    // Вывод списка смежности для проверки
    std::cout << "Graph adjacency list:" << std::endl;
    for (size_t i = 0; i < graph.size(); ++i) {
        std::cout << i << ": ";
        for (const Edge& e : graph[i]) {
            std::cout << e << " ";
        }
        std::cout << "\n";
    }

    // Ввод стартовой вершины
    int start;
    std::cout << "\nEnter starting point: ";
    std::cin >> start;

    // Вычисление кратчайших путей
    std::vector<int> distances, predecessors;
    dijkstra(graph, start, distances, predecessors);

    // Вывод результатов
    std::cout << "\nShortest paths from vertex " << start << ":\n";
    for (size_t i = 0; i < graph.size(); ++i) {
        if (i == start) continue;  // Пропускаем стартовую вершину
        
        if (distances[i] == INT_MAX) {
            std::cout << "Vertex " << i << ": unreachable\n";
        } else {
            std::cout << "Vertex " << i << ": distance = " << distances[i] << ", path: ";
            std::vector<int> path = reconstructPath(start, i, predecessors);
            
            // Вывод пути
            for (int v : path) {
                std::cout << v << " ";
            }
            std::cout << "\n";
        }
    }

    return 0;
}