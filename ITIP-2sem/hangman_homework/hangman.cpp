#include <iostream>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <chrono> // для задержки
#include <thread> // для зажержки
using namespace std;

string topicChoose(){
    cout << "Which topic would you like to play with?\n";
    cout << "1) Fruits\n" << "2) Transport\n" << "3) Ballroom dance figure\n" << "4) animals\n" << "5) musical instruments\n" << "Choose your preferences (by typing corresponding number)\n";
    int num;
    cin >> num;
    switch (num){
    case 1:
        return "fruits";
        break;
    case 2:
        return "transport";
        break;
    case 3:
        return "ballroom dance figure";
        break;
    case 4:
        return "animals";
        break;
    case 5:
        return "musical instruments";
        break;
    default:
        cout << "Bad user\n";
        return "random letters";
        break;
    }
}

void clearScreen(){ // для красивого вывода
    for (int i = 0; i < 100; i++){
        cout << "\n";
    }
}

void masterpiece(int misses){ // рисунок повешенного
    vector<vector<string>> thisIsTheEnd = {
        {"  -----  ",
        "  |   |  ",
        "  O   |  ",
        "      |  ",
        "      |  ",
        "      |  ",
        "---------"},
        {"  -----  ",
        "  |   |  ",
        "  O   |  ",
        "  |   |  ",
        "      |  ",
        "      |  ",
        "---------"},
        {"  -----  "
        "  |   |  ",
        "  O   |  ",
        " /|   |  ",
        "      |  ",
        "      |  ",
        "---------"},
        {"  -----  ",
        "  |   |  ",
        "  O   |  ",
        " /|/  |  ",
        "      |  ",
        "      |  ",
        "---------"},
        {"  -----  ",
        "  |   |  ",
        "  O   |  ",
        " /|/  |  ",
        " //   |  ",
        "      |  ",
        "---------"}
    };
    for (const auto line : thisIsTheEnd[misses])
        cout << line << "\n";
}

int main(){
    // нужные переменные 
    map<string, string> wordsTopics;
    vector<string> words;
    set<string> topics;
    string str;
    int misses = 0;
    int maxTries = 5;
    string alphabet = "a b c d e f g h i j k l m n o p q r s t u v w x y z";

    // проверка на успешное открытие файла со словами
    ifstream in("last_words.txt");
    if (!in.is_open()){
        cout << "Cant find the input file!\n";
        return 0;}
    
    // заполнение слов, тем, и их сочетаний в словаре
    while (getline(in, str)){
        topics.insert(str.substr(str.find_first_of("-") + 1, str.length()));
        words.push_back(str.substr(0, str.find_first_of("-")));
        wordsTopics[str.substr(0, str.find_first_of("-"))] = str.substr(str.find_first_of("-") + 1, str.length());
    }
    
    // фича
    clearScreen();
    char withTopic;
    cout << "Do you want to play with specialized topic? Type Y (yes) or N (no)\n"; // самое неоптимизированное решение в моей жизни
    cin >> withTopic;

    string word;
    string topic;

    if (withTopic == 'N'){
        srand(time(NULL));
        word = words[rand() % words.size()]; // находим случайное слово
        topic = wordsTopics[word]; // находим тему по слову
    }

    else{ // лучше не смотрите 
        srand(time(NULL));
        topic = topicChoose();
        vector<string> words; // создаем список слов заново 
        for (const auto pair : wordsTopics) // находим слова, которые есть в этой теме
            if (pair.second == topic)
                words.push_back(pair.first);
        word = words[rand() % words.size()]; // находим случайное слово
    }

    string userWord(word.size(), '_'); // создаем "маску" для пользователя

    while (misses < maxTries && userWord != word){
        clearScreen();
        masterpiece(misses);
        if (topic == "random letters")
            cout << "Good luck :)\n";
        cout << "\nYour destiny: " << userWord << "\n";
        cout << "Your topic: " << topic << "\n";
        cout << "Left letters: " << alphabet << "\n";
        cout << "Guess the whole word or just one letter\n";
        
        string userTry;
        cin >> userTry;
        
        if (userTry.length() == 1){
            char userLetter = userTry[0];
            size_t pos = word.find(userLetter);

            if (alphabet.find(userLetter) != string::npos)
                alphabet.erase(alphabet.find(userLetter), 1); // чистка алфавита 
            else{
                cout << "You have already tried this letter. Smells like weakness \n";
                this_thread::sleep_for(chrono::seconds(1));
                continue;
            }

            if (pos != std::string::npos){ // проверка, есть ли указанная буква в загаданном слове 
                for (size_t i = 0; i < word.size(); i++)
                    if (word[i] == userLetter)
                        userWord[i] = userLetter; // замена _ на указанную букву, если она есть в слове
            }
            else{
                cout << "Pathetic\n";
                this_thread::sleep_for(chrono::seconds(1));
                misses++;
            }
            
            
        }
        else{
            if (userTry == word)
                cout << "You Won.. Congratulations!\n";
            else{
                cout << "Nice try. The word was " << word << "\n";
                masterpiece(4);
            }
            return 0;
        }
    }
    clearScreen();

    if (userWord == word)
        cout << "You Won.. Congratulations! The word was " << word << "\n";
    else
        cout << "Nice try. The word was " << word << "\n";

    return 0;
}