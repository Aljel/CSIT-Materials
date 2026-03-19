#pragma once
#include <iostream>

struct Vec2 {
        float x = 0;
        float y = 0;

        Vec2() {}
        Vec2(float a, float b)
            : x(a),
              y(b) {}
};

struct Vec3 {
        float x = 0;
        float y = 0;
        float z = 0;

        Vec3() {}
        Vec3(float a, float b, float c)
            : x(a),
              y(b),
              z(c) {}
        Vec3(Vec2 ab, float c)
            : Vec3(ab.x, ab.y, c) {}

        Vec3 &operator*=(const Vec3 &v) {
            this->x *= v.x;
            this->y *= v.y;
            this->z *= v.z;
            return *this;
        }

        const Vec3 operator*(const Vec3 &v) const { return Vec3(*this) *= v; }

        float &operator[](size_t i) {
            return (reinterpret_cast<float *>(this))[i];
        }

        const float &operator[](size_t i) const {
            return (reinterpret_cast<const float *>(this))[i];
        }
};

inline float dot(const Vec3 &a, const Vec3 &b) {
    Vec3 tmp = a * b;             // вычисляем произведения соответствующих
    return tmp.x + tmp.y + tmp.z; // и возвращаем их сумму
}

struct Mat3 {
        Vec3 row1{};
        Vec3 row2{};
        Vec3 row3{};
        Mat3() {}

        Mat3(Vec3 r1, Vec3 r2, Vec3 r3)
            : row1(r1),
              row2(r2),
              row3(r3) {}

        Mat3(float a) {
            row1 = Vec3(a, 0.f, 0.f);
            row2 = Vec3(0.f, a, 0.f);
            row3 = Vec3(0.f, 0.f, a);
        }

        Vec3 &operator[](size_t i) {
            return (reinterpret_cast<Vec3 *>(this))[i];
        }

        Mat3 &transpose() {
            Mat3 tmp(*this); // делаем временную копию матрицы
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    (*this)[i][j] =
                        tmp[j][i]; // заменяем элементы текущего объекта
                    // из временной копии
                }
            }
            return *this;
        }

        const Vec3 operator*(const Vec3 &v) {
            Vec3 res{}; // создаем новый вектор (для результата)
            for (int i = 0; i < 3; ++i) {
                res[i] = dot((*this)[i],
                             v); // i-й элемент вектора - скалярное произведение
            }
            return res;
        }

        Mat3 &operator*=(const Mat3 &m) {
            Mat3 A(*this); // создаем копии исходных матриц
            Mat3 B(m);
            B.transpose(); // транспонируем вторую матрицу
            for (int i = 0; i < 3; i++) {
                (*this)[i] =
                    A * B[i]; // в i-ю строку текщего объекта записываем
                              // результат перемножения первой матрицы с i-й
                              // строкой транспонированной матрицы,
            }
            return this->transpose(); // транспонируем текущий объект, получаем
                                      // результат
        }
        const Mat3 operator*(const Mat3 &m) const { return Mat3(*this) *= m; }
};

inline Vec2 normalize(const Vec3 &v) { return Vec2(v.x / v.z, v.y / v.z); }
