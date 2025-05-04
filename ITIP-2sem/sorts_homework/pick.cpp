#include <iostream>
using namespace std;

void print(int *mas){
    for(int i=0; i < 9; i++)
        cout << mas[i] << " ";
}

int main(){
    int n = 9;
    int *mas = new int [9];
    for(int i=0; i < n; i++)
        cin >> mas[i];
    for(int i=0; i < n; i++)

    cout << mas[i] << " ";
    cout << endl;
    for (int i = 0; i < n; i++){
        int min = mas[i];
        int min_index = i;
        for (int j = i+1; j <= n-1; j++){
            if (min > mas[j]){
                min_index = j; 
                min = mas[j];
            }
            print(mas);
            cout << " i = " << i << " j = " << j << "\n";
        }
        if(i!=min_index) 
            swap(mas[i],mas[min_index]); 
    }
    for(int i=0; i < n; i++)
    cout << mas[i] << " ";
    return 0;
}