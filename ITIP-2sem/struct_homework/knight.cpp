#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// Структура клетки для определении на шахматном поле
struct cell{ 
    int x, y;
    cell(int x = 0, int y = 0) : x(x), y(y) {} // Конструктор с параметрами по умолчанию (0). создан для быстрого создания "клеток"
};

struct queue{
    cell inf;
    queue *next;
};

void push(queue *&h, queue *&t, cell x){ //вставка элемента в очередь
    queue *r = new queue; //создаем новый элемент
    r->inf = x; //с значением x
    r->next = NULL; //всегда последний
    if(!h && !t)
        h = t = r; //если готова и хвост ссылаются на NULL, то новый элемент это и голова и хвост
    else{
        t->next = r; //переопределяем хвост 
        t = r;
    }
}


cell pop(queue *&h, queue *&t){ //удаление элемента из очереди
    queue *r = h; //создаем указатель на голову
    cell i = h->inf; //сохраняем значение головы
    h = h->next; //сдвигаем указатель на следующий элемент
    if (!h) //если удаляем последний элемент из очереди
        t = NULL;
    delete r; //удаляем первый элемент
    return i;
}  

// Перегруз оператора для типа cell
bool operator == (cell a, cell b){
    return (a.x == b.x) && (a.y == b.y);
};

bool operator < (cell a, cell b){
    return (a.x < b.x ) || (a.x == b.x && a.y < b.y);
};

// Проверка, находится ли клетка в пределах доски (8x8)
bool isValid(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// Перевод строки в клетку
cell sToCell (std::string a){
    char col = a[0];
    char row = a[1];
    int x = col - 'A'; // По асхи даст при col = A x = 0, col = B x = 1  и т. д. 
    int y = row - '1';
    return cell(x, y);
}

// Перевод клетки в строку
std::string cToString (cell a){
    std::string res;
    res += a.x + 'A';
    res += a.y + '1';
    return res;
}

// Вектора, хранящие возможные ходы коня
const std::vector<int> dx = {2, 2, 1, 1, -1, -1, -2, -2};
const std::vector<int> dy = {1, -1, 2, -2, 2, -2, 1, -1};

// По сути обход графа в ширину 
std::vector<cell> BFS(cell start, cell end){
    queue *head = NULL, *tail = NULL; // Создаем очередь и записываем туда старт
    push(head, tail, start);

    std::map<cell, cell> parent; // Создаем словарь с отношениями клеток
    parent[start] = cell(-1, -1); // У старта нет родителя

    while (head) {
        cell current = pop(head, tail); // Берем нынешнюю клетку из очереди 
        if (current == end){
            break;
        }

        // Проверяем все 8 возможных ходов
        for (int i = 0; i < 8; ++i) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValid(newX, newY)){
                cell neighbor(newX, newY);

                if (parent.find(neighbor) == parent.end()){ // Если эту клетку еще не посещали (в случае неудачи find возвращает итератор на .end() - клетка за последним элементом словаря)
                    parent[neighbor] = current; // Запоминаем родителя 
                    push(head, tail, neighbor); // Убираем в очередь
                }
            }
        }
    }

    // Очистка оставшейся очереди
    while (head) {
        pop(head, tail);
    }

    std::vector<cell> path; // Вектор, который будет хранить путь от начала до конца 
    if (parent.find(end) != parent.end()){ // Если конечная клетка была достигнута
        cell current = end;
        
        // Идем от конца к началу по цепочке родителей
        while (!(current == start)) {
            path.push_back(current); // Добавляем текущую клетку в путь
            current = parent[current]; // Переходим к родителю
        }
        path.push_back(start); // Добавляем стартовую клетку
        
        reverse(path.begin(), path.end());   // Разворачиваем путь (от начала к концу)
    }

    return path;

}

int main(){
    std::string st, en;
    std::cout << "Enter the start and end points\n";
    std::cin >> st >> en;

    cell start = sToCell(st);
    cell end = sToCell(en);
    if (!(isValid(start.x, start.y) && isValid(end.x, end.y))){
        std::cout << "Incorrect points\n";
        return 0;
    }

    std::vector<cell> answer = BFS(start, end);
    if (answer.empty())
        std::cout<< "There is no possible way!\n";
    else
        for (int i = 0; i < answer.size(); i++)
            std:: cout << cToString(answer[i]) << " ";

    return 0;
}   
