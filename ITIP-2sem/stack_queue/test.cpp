#include <iostream>
#include <math.h>
using namespace std;

int dividers(int n){
    int count = 0;
    for (int i = 2; i <= sqrt(n) ; i++)
        if (n % i == 0 && n != i)
            if(i == n / i)
                count ++;
            else 
                count += 2;

    return count;
}

int main(){
    cout << dividers(8) << " " << sqrt(6);
}
