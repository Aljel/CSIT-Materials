#include <fstream> 
#include <iomanip>
using namespace std; 
 
ifstream in("input.txt"); 
ofstream out("output.txt"); 
 
// быстрая сортировка. Среднее время работы - NlogN
void sort(int **matrix, int col, int low, int high) {
    int pivot = matrix[(low + high) / 2][col]; // берем точку опоры в середине 
    int i = low; // создаем верхний и нижни указатель (потому что столбцы, так-то левый и правый)
    int j = high;
    while (i <= j) {
        while (matrix[i][col] < pivot) // если элементы слева от середины меньше нее - то супер
            i++;
        while (matrix[j][col] > pivot) // если элемента справа от середины больше нее - то супер
            j--;
        if (i <= j){ // получается наткнулись на элементы, которые не удовлетворяют условиям выше - меняем их 
            int temp = matrix[i][col];
            matrix[i][col] = matrix[j][col];
            matrix[j][col] = temp;
            i++;
            j--;
        }
    }
    // получили массив, в котором слева от середины числа меньше, а справа - больше. Теперь отдельно рекурсивно сортируем эти половинки
    if (j - low > 0)
        sort(matrix, col, low, j);
    if (high - i > 0)
        sort(matrix, col, j + 1, high);
}


int main(){ 
int n;  
in >> n ; 
int **matrix = new int*[n];
for (int i = 0; i < n; i++) 
    matrix[i] = new int[n]; 
 
for (int i = 0; i < n; i++) 
    for (int j = 0; j < n; j++) 
        in >> matrix[i][j]; 

for (int i = 0; i < n; i++) 
    sort(matrix, i, 0, n-1); 

for (int i = 0; i < n; i++){ 
    for (int j = 0; j < n; j++) 
        out << setw(4) << matrix[i][j] << " "; 
    out << "\n"; 
} 


return 0; 
}