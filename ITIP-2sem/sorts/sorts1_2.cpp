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

vector<pair<int,people>> inFile(){ // ввод из файла
    vector<pair<int,people>> x;
    people temp;
    while(in.peek() != EOF){ // считываем по пробелам
        in >> temp.Surname; // фамилия
        in >> temp.Work; // место работы

        string tmp; // дата рождения
        in >> tmp;
        temp.DateOfBirth = Str_to_Date(tmp);

        in >> temp.TimeOfWork; // стаж работы
        in >> temp.Salary; // зарплата

        pair<int,people> z = make_pair(temp.DateOfBirth.yy, temp);
        x.push_back(z);
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

// сортировка подсчетом - сложность O(k + N), но зависит от k и в общем от количества одинаковых чисел
void Sort(vector<pair<int, people>> &x){
    int min = x[0].first;
    int max = x[0].first;

    for(int i=0; i < x.size(); i++){ 
        if (max < x[i].first) max = x[i].first;
        if (min > x[i].first) min = x[i].first;
    }

    int k = abs(max - min + 1);
    vector<vector<pair<int, people>>> B(k);
    for (int i = 0; i < x.size(); i++)
        B[x[i].first - min].push_back(x[i]);

    x.clear();
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < B[i].size(); j++) {
            x.push_back(B[i][j]);
        }
    }

}

int main(){
    vector<pair<int,people>> x;
    x = inFile();
    Sort(x);
    for (int i = 0; i < x.size(); i++)
        print(x[i].second);
    return 0;
    }
    