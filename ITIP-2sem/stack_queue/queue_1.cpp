#include <iostream>
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
    

    //находим максимальный и последний четный элемент 
    int max = head->inf;
    int last = -1;
    queue *save = head;
    while (save){
        if (save->inf > max)
            max = save->inf;
        if (save->inf % 2 == 0)
            last = save->inf;
        save = save->next;
    }

    cout << "max = " << max << " last even = " << last << "\n";

    //получаем новую очередь с искомым порядком 
    while (head) {
        int value = pop(head, tail); //извлекаем элемент из исходной очереди
        push(head2, tail2, value);   //добавляем его в новую очередь
        if (value == max)            //если элемент равен максимальному
            push(head2, tail2, last); //вставляем последний четный элемент
    }
    print (head2);
    
    //освобождаем память
    while (head2) {
        pop(head2, tail2);
    }

    return 0;
}