#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void print(vector<int> vec){
    for(vector<int>::iterator iter = vec.begin(); iter != vec.end(); iter ++)
        cout << *iter << " ";
    cout << "\n";
}
// Проверка на нечетность
bool odd(int x){ 
    return x % 2 == 0;
    }
    
// Удаление нечетных чисел 
void removeOddNumbers(vector<int> vec) {
    vector<int>::iterator it = remove_if(vec.begin(), vec.end(), odd); //remove только передвигает элементы и смещает итератор
    vec.erase(it , vec.end()); // само удаление
}

// Замена чисел, кратных X, на максимальное число 
void replaceMultiplesOfX(vector<int> vec, int X) {
    if (vec.empty()) 
        return;

    int max = *max_element(vec.begin(), vec.end());
    for(vector<int>::iterator iter = vec.begin(); iter != vec.end(); iter ++){
        if (*iter % X ==0)
            replace(vec.begin(), vec.end(), *iter, max);
    }
}

// Слияние двух отсортированных векторов в один
vector<int> mergeVectors(const vector<int> vec1, const vector<int> vec2) {
    vector<int> result;
    result.reserve(vec1.size() + vec2.size());

    auto it1 = vec1.begin(); // использую тип auto, так как работаю с указателями (итераторами), компилятор сам понимает, что за тип при инициализации 
    auto it2 = vec2.begin();

    // Слияние двух отсортированных последовательностей 
    while (it1 != vec1.end() && it2 != vec2.end()) { 
        if (*it1 < *it2) { // сравнение самих чисел из двух векторов, для сортированного результата
            result.push_back(*it1);
            it1++;
        } else {
            result.push_back(*it2);
            it2++;
        }
    }
    // После работы цикла один из векторов еще имеет числа, причем большие, чем предыдущие, поэтому:

    // Добавление оставшихся элементов из первого вектора
    while (it1 != vec1.end()) {
        result.push_back(*it1);
        it1++;
    }

    // Добавление оставшихся элементов из второго вектора
    while (it2 != vec2.end()) {
        result.push_back(*it2);
        it2++;
    }

    return result;
}

// Вставка нового элемента в отсортированный список
void insertIntoVector(vector<int> vec, int num) {
    auto it = lower_bound(vec.begin(), vec.end(), num); // опять auto. Больно он хорош. Lower_bound возвращает итератор первого не меньшего (от данного в функции) числа в векторе
    vec.insert(it, num);
}

int main() {
    setlocale(LC_ALL,"russian");
    int a,b;
    cout << "Введите длинну первого и второго вектора соответственно\n";
    cin >> a >> b;

    vector<int> vec1;
    int x;
    for(int i = 0; i < a; i++){ //создание вектора
        cout << "a[" << i << "] = ";
        cin >> x;
        vec1.push_back(x);
    }
    vector<int> vec2;
    for(int i = 0; i < b; i++){ 
        cout << "b[" << i << "] = ";
        cin >> x;
        vec2.push_back(x);
    }

    int X; 
    cout << "Введите число, кратное которому нужно заменить\n";
    cin >> X;

    removeOddNumbers(vec1);
    
    // замена элементов, кратных X на максимальный во втором векторе
    int max = *max_element(vec2.begin(), vec2.end());
    for(vector<int>::iterator iter = vec2.begin(); iter != vec2.end(); iter ++){
        if (*iter % X ==0)
            replace(iter, iter + 1, *iter, max);
    }

    sort(vec1.begin(), vec1.end());
    sort(vec2.begin(), vec2.end());

    vector<int> vec3 = mergeVectors(vec1, vec2);

    int Y;
    cout << "Введите число, которое нужно вставить\n";
    cin >> Y;
    insertIntoVector(vec3, Y);

    cout << "Результирующий вектор: ";
    for(vector<int>::iterator iter = vec3.begin(); iter != vec3.end(); iter ++)
        cout << *iter << " ";
    cout << "\n";

    return 0;
}