#include <fstream> 
#include <iomanip>
using namespace std; 
 
ifstream in("input.txt"); 
ofstream out("output.txt"); 
 
// Сортировка расческой. По сути работает за N^2, но стремится к NlogN 
void sort(int **matrix, int col, int n) {
    int gap = n/1.247; // сравниваем не соседние элементы, а элементы, находящиеся на каком-то расстоянии. И потом это расстояние сокращаем, до тех пор, пока оно не станет равно единице
    while (gap >= 1){
        if (gap < 1) 
            gap = 1;
        for(int i = 0; i < n - gap; i++)
            if ((col%3!=2 && matrix[i][col] > matrix[i+gap][col]) || (col%3==2 && matrix[i][col] < matrix[i+gap][col])){
                int temp = matrix[i][col];
                matrix[i][col] = matrix[i+gap][col];
                matrix[i+gap][col] = temp;
            }
        gap /= 1.247;
    }
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
    sort(matrix, i, n); 

for (int i = 0; i < n; i++){ 
    for (int j = 0; j < n; j++) 
        out << setw(4) << matrix[i][j];
    out << "\n"; 
} 


return 0; 
}