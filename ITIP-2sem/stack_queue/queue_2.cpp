#include <iostream>
#include <math.h>
using namespace std;


struct queue{
    int inf;
    queue *next;
};

void push(queue *&h, queue *&t, int x){ //вставка элемента в очередь
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

int pop(queue *&h, queue *&t){ //удаление элемента из очереди
    queue *r = h; //создаем указатель на голову
    int i = h->inf; //сохраняем значение головы
    h = h->next; //сдвигаем указатель на следующий элемент
    if (!h) //если удаляем последний элемент из очереди
        t = NULL;
    delete r; //удаляем первый элемент
    return i;
}

void print(queue *&h){
    queue *tmph = h;
    while (tmph){
        cout << tmph->inf << " ";
        tmph = tmph->next;
    }
    cout << "\n";
    delete tmph;
}

bool complex(int n){
    for (int i = 2; i < sqrt(n) + 1; i++)
        if (n % i == 0 && n != i)
            return true;
    return false;
}
int main(){
    queue *head = NULL, *tail = NULL;
    queue *head2 = NULL, *tail2 = NULL;

    
    //ввод
    cout << "Enter n - length of queue\n";
    int n;
    cin >> n;
    cout << "Enter elements of queue\n";
    int a;
    for (int i = 0; i < n; i++){
        cin >> a;
        push(head, tail, a);
    }
    print(head);
    

    int first = 0;
    queue *save = head;
    while (save){
        if (complex(save->inf)){
            first = save->inf;
            break;
        }
        save = save->next;
    }
    if (first == 0){
        cout << "there is no complex numbers\n";
        return 0;
    }
    cout << "first complex = " << first << "\n";
    

    while (head) {
        if (complex(head->inf))
            break;
        push(head, tail, pop(head, tail));
    }
    print (head);


    //освобождаем память
    while (head) {
        pop(head, tail);
    }

    return 0;
}