#include <iostream>
#include <vector> 


struct stack{
    int inf;
    stack *next;
};

void s_push(stack *&h, int x){
    stack *r = new stack; //создаем новый элемент
    r->inf = x; //он равен х
    r->next = h; //следующим элементом будет прошлый первый
    h = r; //r ставим первым
}

int s_pop (stack *&h){
    int i = h->inf; //значение первого элемента
    stack *r = h; //указатель на голову 
    h = h->next; //переносим указатель на новый элемент 
    delete r;
    return i;
}

struct queue{
    int inf;
    queue *next;
};

void q_push(queue *&h, queue *&t, int x){ //вставка элемента в очередь
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

int q_pop(queue *&h, queue *&t){ //удаление элемента из очереди
    queue *r = h; //создаем указатель на голову
    int i = h->inf; //сохраняем значение головы
    h = h->next; //сдвигаем указатель на следующий элемент
    if (!h) //если удаляем последний элемент из очереди
        t = NULL;
    delete r; //удаляем первый элемент
    return i;
}  

// Обход графа в глубину
void DFS(std::vector<std::vector<int>> Gr, std::vector<int>& A, int x) {
    stack* st = NULL; // Инициализация стека
    A[x] = 1; // Помечаем начальную вершину 
    s_push(st, x);
    std::cout << x << " "; // Выводим начальную вершину

    while (st != NULL) {
        int current = st->inf;
        bool found = false;  // Сбрасываем флаг для каждой вершины
        
        for (int i = 0; i < Gr[current].size(); i++) { // Проходим по смежным вершинам, ищем непосещенные
            if (A[Gr[current][i]] == 0) { 
                int y = Gr[current][i];
                A[y] = 1; // Посещаем
                s_push(st, y); 
                std::cout << y << " "; 
                found = true;
                break;
            }
        }
        if (!found) {
            s_pop(st);
        }
    }
}

void DFSTravel(std::vector<std::vector<int>> Gr, int N) {
    std::vector<int> visited(N, 0); // Массив посещенных вершин
    for (int i = 0; i < N; ++i) {
        if (visited[i] == 0) { // Если вершина не посещена
            DFS(Gr, visited, i); 
            std::cout << "\n";  // Разделяем компоненты связности
        }
    }
}

// Обход графа в широту
void BFS(std::vector<std::vector<int>> Gr, std::vector<int> &A, int x){
    queue *head = NULL, *tail = NULL; // Инициализация очереди
    A[x] = 1; // Помечаем начальную вершину
    q_push(head,tail, x);        
    std::cout << x << " "; // Выводим начальную вершину
    
    while (head) {
        int current = q_pop(head, tail); 
      
        for (int i = 0; i < Gr[current].size(); i++) { // Ищем непосещенную смежную вершину
            if (A[Gr[current][i]] == 0) {
                int y = Gr[current][i];
                A[y] = 1; // Посещаем
                q_push(head, tail, y); 
                std::cout << y << " ";
            }
        }
    }
    
}

void BFSTravel(std::vector<std::vector<int>> Gr, int N) {
    std::vector<int> visited(N, 0); // Массив посещенных вершин
    for (int i = 0; i < N; ++i) {
        if (visited[i] == 0) { // Если вершина не посещена
            BFS(Gr, visited, i); // Запускаем BFS
        }
    }
}
int main(){
    
    int N = 0;
    std::cout << "Enter the number of points of the grahp\n";
    std::cin >> N;
    std::vector<std::vector<int>> Gr(N);

    for (int i = 0; i < N; ++i) {
        std::cout << "Enter the number of the connections of the point " << i << ": ";
        int len;
        std::cin >> len;
        std::cout << "Enter the connective points for " << i << ": ";

        Gr[i].resize(len);
        for (int j = 0; j < len; j++)
            std::cin >> Gr[i][j];
    }

    for (int i = 0; i < N; i++){
        std::cout << i << ": ";
        for(auto j = Gr[i].begin(); j != Gr[i].end(); j++)
            std::cout << *j << " ";
        std::cout << "\n";
    }
    DFSTravel(Gr, N);
    BFSTravel(Gr, N);
    std::cout << "\n";
    return 0;
}