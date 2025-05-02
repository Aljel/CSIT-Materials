#include <fstream> 
#include <iomanip>
#include <vector> 
#include <iostream> 
using namespace std; 
 
ifstream in("input.txt"); 
ofstream out("output.txt"); 
 
// Пузырьковая сортировка 
void sort(vector<int>& mas) {
    int n = mas.size();
    for(int i = 0; i < n; i++)
        for(int j = n - 1; j > i; j--)
            if (mas[j-1] > mas[j]){
                int temp = mas[j];
                mas[j] = mas[j - 1];
                mas[j - 1] = temp;
            }
}
    
int main(){ 
int n;
in >> n;
vector<vector<int>> matrix(n, vector<int>(n));
 
for (int i = 0; i < n; i++) 
    for (int j = 0; j < n; j++) 
        in >> matrix[i][j]; 

for (int k = 1; k < 2 * n - 2; k++){
    vector<int> diagonal;
  
    for (int i = 0; i < n ; i++){ // Сбор элементов диагонали
        if (k - i < n && k - i >= 0)
            diagonal.push_back(matrix[i][k - i]);
    }

    sort(diagonal);

    int L = 0;
    for (int i = 0; i < n; i++){ // Запись отсортированных элементов обратно в матрицу
        if (k - i < n && k - i >= 0){
            matrix[i][k - i] = diagonal[L];
            L += 1;
        }
    }

    diagonal.clear();
}

for (int i = 0; i < n; i++){ 
    for (int j = 0; j < n; j++) 
        out << setw(4) << matrix[i][j] << " "; 
    out << "\n"; 
} 
cout << "END";

return 0; 
}