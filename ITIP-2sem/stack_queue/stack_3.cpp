#include <iostream>
using namespace std;

struct stack{
    int inf;
    stack *next;
};

void push(stack *&h, int x){
    stack *r = new stack; //создаем новый элемент
    r->inf = x; //он равен х
    r->next = h; //следующим элементом будет прошлый первый
    h = r; //r ставим первым
}

int pop (stack *&h){
    int i = h->inf; //значение первого элемента
    stack *r = h; //указатель на голову 
    h = h->next; //переносим указатель на новый элемент 
    delete r;
    return i;
}

void reverse(stack *&h){
    stack *h1 = NULL;
    while (h)
        push(h1, pop(h));
    h = h1;
}

void print(stack *&h){
    stack *save = h;
    while (save){
        cout << save->inf << " ";
        save = save->next;
    }
    cout << "\n";
    delete save;
}
int main(){
    stack *h = NULL, *chh = NULL, *nchh = NULL, *res = NULL;
    //ввод
    cout << "Enter n - length of stack\n";
    int n;
    cin >> n;
    cout << "Enter elements of stack\n";
    int a;
    for (int i = 0; i < n; i++){
        cin >> a;
        push(h, a);
    }

    while (h){
        if (h->inf % 2 == 0)
            push(chh, pop(h));
        else
            push(nchh, pop(h));
    }
    
    cout << "Even numbers: ";
    print(chh);
    cout << "Odd numbers: ";
    print(nchh);

    reverse(chh);
    reverse(nchh);

    while (nchh)
        push(res, pop(nchh));
    while (chh)
        push(res, pop(chh));

    print(res);

    return 0;
}