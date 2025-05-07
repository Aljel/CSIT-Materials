#include <iostream>
#include <math.h>
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

int dividers(int n){
    if (n == 1) return 1;
    int count = 0;
    for (int i = 1; i <= sqrt(n); i++)
        if (n % i == 0)
            if (i == n / i)
                count ++;
            else 
                count += 2;

    return count;
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
    stack *h = NULL, *res = NULL;
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
    
    //находим максимальное число делителей
    int max = -1;
    stack *save = h;
    while (save){
        if (max < dividers(save->inf))
            max = dividers(save->inf);
        save = save->next;
    }
    delete save;

    cout << "Max number of divisors = " << max << "\n";

    //удаляем элементы с максимальным количеством делителей
    while (h) {
        int value = pop(h);
        if (dividers(value) != max) {
            push(res, value); 
        }
    }

    print(res);

    return 0;
}