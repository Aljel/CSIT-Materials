#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");

struct date{ // дата
    int dd, mm, yy;
};

struct people{ // данные о человеке
    string Surname; // фамилия
    string Work;
    date DateOfBirth; // дата рождения
    int TimeOfWork;
    int Salary; // зарплата
};

date Str_to_Date(string str ){ // из строки в дату
    date x;
    string temp = str.substr(0, 2); // день
    x.dd = atoi(temp.c_str() );
    temp = str.substr(3, 2);// месяц
    x.mm = atoi(temp.c_str());
    temp = str.substr(6, 4); // год
    x.yy = atoi(temp.c_str() );
    return x;
}

vector<people> inFile(){ // ввод из файла
    vector<people> x;
    people temp;
    while(in.peek() != EOF){ // считываем по пробелам
        in >> temp.Surname; // фамилия
        in >> temp.Work; // место работы

        string tmp; // дата рождения
        in >> tmp;
        temp.DateOfBirth = Str_to_Date(tmp);

        in >> temp.TimeOfWork; // стаж работы
        in >> temp.Salary; // зарплата

        x.push_back(temp);
    }
    return x;
}

void print(people x){
    out << setw(11) << left << x.Surname; // фамилия
    out << setw(10) << left << x.Work; // работа

    if (x.DateOfBirth.dd < 10) out << '0' << x.DateOfBirth.dd << '.'; // добавляем 0
    else out << x.DateOfBirth.dd << '.';
    if (x.DateOfBirth.mm < 10) out << '0' << x.DateOfBirth.mm << '.';
    else out << x.DateOfBirth.mm << '.';
    out << x.DateOfBirth.yy << "  "; 

    out << setw(4) << left << x.TimeOfWork; // стаж работы
    out << setw(10) << left << x.Salary << "\n"; // запрлата
}

bool operator < (people a, people b){ // переопределяем оператор < в соотвествии с условием
    if (a.Work < b.Work) 
        return true;
    else if (a.Work == b.Work && a.TimeOfWork <= b.TimeOfWork) 
        return true;
    else if (a.Work == b.Work && a.TimeOfWork == b.TimeOfWork && a.Salary <= b.Salary)
        return true;
    return false;
}

// сортировка пузырьком - O(N^2)
void Sort(vector<people> &x){
    for(int i = 0; i < x.size(); i++)
        for(int j = x.size()-1; j > i; j--)
            if (x[j] < x[j-1]) 
                swap(x[j-1],x[j]); 
}

int main(){
    vector<people> x;
    x = inFile();
    Sort(x);
    for (int i = 0; i < x.size(); i++)
        print(x[i]);
    return 0;
}