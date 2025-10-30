#include "Rational.h"
#include <iostream>
#include <cmath>

using namespace std;

int Rational::gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int Rational::abs(int a) {
    return std::abs(a);
}

void Rational::normalize() {
    if (denominator == 0) {
        throw 0;
    }
    
    // Определяем знак
    isNegative = (numerator < 0) || (denominator < 0);
    numerator = abs(numerator);
    denominator = abs(denominator);
    
    // Сокращаем дробь
    int g = gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;
    
    // Определяем тип дроби
    isProper = (numerator < denominator) && (numerator != 0);
    
    // Вычисляем целую часть
    if (!isProper) {
        wholePart = numerator / denominator;
        numerator = numerator % denominator;
    } else {
        wholePart = 0;
    }
}

Rational::Rational() {
    numerator = 0;
    denominator = 1;
    isProper = true;
    wholePart = 0;
    isNegative = false;
}

Rational::Rational(int num, int den) {
    numerator = num;
    denominator = den;
    normalize();
}

int Rational::getNumerator() { return numerator; }
int Rational::getDenominator() { return denominator; }
bool Rational::getIsProper() { return isProper; }
int Rational::getWholePart() { return wholePart; }
bool Rational::getIsNegative() { return isNegative; }

void Rational::setNumerator(int num) {
    numerator = num;
    normalize();
}

void Rational::setDenominator(int den) {
    denominator = den;
    normalize();
}

void Rational::setRational(int num, int den) {
    numerator = num;
    denominator = den;
    normalize();
}

double Rational::getValue() const {
    double result = wholePart + (double)numerator / denominator;
    return isNegative ? -result : result;
}

string Rational::toMixed() const {
    string result = "";
    if (isNegative) {
        result += "-";
    }
    if (wholePart != 0) {
        result += to_string(wholePart) + " ";
    }
    result += to_string(numerator) + "/" + to_string(denominator);
    return result;
}

bool Rational::operator==(const Rational other) const {
    return (numerator * other.denominator == other.numerator * denominator) &&
           (isNegative == other.isNegative) && (wholePart == other.wholePart);
}

bool Rational::operator!=(const Rational other) const {
    return !(*this == other);
}

bool Rational::operator<(const Rational other) const {
    double thisValue = getValue();
    double otherValue = other.getValue();
    return thisValue < otherValue;
}

bool Rational::operator>(const Rational other) const {
    return other < *this;
}

bool Rational::operator<=(const Rational other) const {
    return *this < other || *this == other;
}

bool Rational::operator>=(const Rational other) const {
    return *this > other || *this == other;
}

Rational Rational::operator+(const Rational other) const {
    int newNum = (isNegative ? -1 : 1) * (wholePart * denominator + numerator) * other.denominator +
                 (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator) * denominator;
    int newDen = denominator * other.denominator;
    return Rational(newNum, newDen);
}

Rational Rational::operator-(const Rational other) const {
    int newNum = (isNegative ? -1 : 1) * (wholePart * denominator + numerator) * other.denominator -
                 (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator) * denominator;
    int newDen = denominator * other.denominator;
    return Rational(newNum, newDen);
}

Rational Rational::operator*(const Rational other) const {
    int thisTotal = (isNegative ? -1 : 1) * (wholePart * denominator + numerator);
    int otherTotal = (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator);
    return Rational(thisTotal * otherTotal, denominator * other.denominator);
}

Rational Rational::operator/(const Rational other) const {
    int thisTotal = (isNegative ? -1 : 1) * (wholePart * denominator + numerator);
    int otherTotal = (other.isNegative ? -1 : 1) * (other.wholePart * other.denominator + other.numerator);
    return Rational(thisTotal * other.denominator, denominator * otherTotal);
}

void Rational::Print() {
    cout << "Рациональное число: ";
    if (isNegative) cout << "-";
    if (wholePart != 0) {
        cout << wholePart << " ";
    } else if (numerator != 0) {
        cout << numerator << "/" << denominator;
    } else {
        cout << 0;
    }
    cout << " (десятичное: " << (isNegative ? -1 : 1) *
            (wholePart + (double)numerator / denominator) << ")";
    cout << " [" << (isProper ? "правильная" : "неправильная") << " дробь]\n";
}
