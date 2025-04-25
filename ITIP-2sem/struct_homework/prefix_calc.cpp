#include <iostream>
#include <string>
using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

double applyOperation(int a, int b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

struct stack{
    string inf;
    stack *next;
};

void push(stack *&h, string x){
    stack *r = new stack; //создаем новый элемент
    r->inf = x; //он равен х
    r->next = h; //следующим элементом будет прошлый первый
    h = r; //r ставим первым
}

string pop (stack *&h){
    string i = h->inf; //значение первого элемента
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

int evaluatePrefix(string str) {
    stack *s = NULL;

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') continue;

        if (isOperator(str[i])) {
            push(s, string(1, str[i]));
        }
        else {
            if (s != NULL && isOperator(s->inf[0])) {
                push(s,string(1, str[i]));
            }
            else {
                int b = str[i] - '0';
                while (s != NULL && !isOperator(s->inf[0])) {
                    int a = stoi(pop(s));
                    char op = pop(s)[0];
                    b = applyOperation(a, b, op);
                }
                push(s, to_string(b));
            }
        }
    }

    return stoi(pop(s));
}

int main() {
    setlocale(LC_ALL, "RUS");
    string prefixExpr;
    cout << "Введите префиксное выражение: ";
    getline(cin, prefixExpr);
    
    int result = evaluatePrefix(prefixExpr);
    cout << "Результат: " << result << "\n";
    
    return 0;
}