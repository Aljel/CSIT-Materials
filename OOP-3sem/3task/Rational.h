#pragma once
// Это буквально альтернатива
// #ifndef MYHEADER_H
// #define MYHEADER_H

#include <string>

class Rational {
private:
    int numerator;      // числитель
    int denominator;    // знаменатель
    bool isProper;      // флаг правильной дроби
    int wholePart;      // целая часть для смешанной дроби
    bool isNegative;    // флаг отрицательного числа

    // Вспомогательный метод для нахождения НОД
    int gcd(int a, int b);
    
    // Вспомогательный метод для взятия модуля
    int abs(int a);
    
    // Приведение дроби к каноническому виду
    void normalize();

public:
    // Конструктор по умолчанию
    Rational();
    
    // Конструктор с параметрами
    Rational(int num, int den = 1);
    
    // Геттеры для всех атрибутов
    int getNumerator();
    int getDenominator();
    bool getIsProper();
    int getWholePart();
    bool getIsNegative();
    
    // Сеттеры
    void setNumerator(int num);
    void setDenominator(int den);
    void setRational(int num, int den);
    
    // Получение десятичного значения
    double getValue() const;
    
    // Преобразование к смешанной дроби
    std::string toMixed() const;
    
    // Операции сравнения
    bool operator==(const Rational other) const;
    bool operator!=(const Rational other) const;
    bool operator<(const Rational other) const;
    bool operator>(const Rational other) const;
    bool operator<=(const Rational other) const;
    bool operator>=(const Rational other) const;
    
    // Арифметические операции
    Rational operator+(const Rational other) const;
    Rational operator-(const Rational other) const;
    Rational operator*(const Rational other) const;
    Rational operator/(const Rational other) const;
    
    // Метод для вывода
    void Print();
};
