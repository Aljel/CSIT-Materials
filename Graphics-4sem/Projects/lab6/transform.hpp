#pragma once

#include "matrix.hpp"
#include <math.h>

inline Mat3 translate(float Tx, float Ty) {
    Mat3 res = Mat3(1.f);
    res[0][2] = Tx;
    res[1][2] = Ty;
    return res;
}

inline Mat3 scale(float Sx, float Sy) {
    Mat3 res = Mat3(1.f);
    res[0][0] = Sx;
    res[1][1] = Sy;
    return res;
}

inline Mat3 scale(float S) { return scale(S, S); }

inline Mat3 rotate(float theta) {
    Mat3 res = Mat3(1.f);
    res[0][0] = static_cast<float>(cos(theta));
    res[1][1] = res[0][0];
    res[0][1] = static_cast<float>(-sin(theta));
    res[1][0] = -res[0][1];
    return res;
}

inline Mat3 mirrorX() {
    Mat3 res = Mat3(1.f);
    res[1][1] = -1;
    return res;
}

inline Mat3 mirrorY() {
    Mat3 res = Mat3(1.f);
    res[0][0] = -1;
    return res;
}

inline Mat4 translate(float Tx, float Ty, float Tz) {
    Mat4 res = Mat4(1.f);
    res[0][3] = Tx;
    res[1][3] = Ty;
    res[2][3] = Tz;
    return res;
}

inline Mat4 scale(float Sx, float Sy, float Sz) {
    Mat4 res = Mat4(1.f);
    res[0][0] = Sx;
    res[1][1] = Sy;
    res[2][2] = Sz;
    return res;
}

inline Mat4 rotate(float theta, const Vec3 &n) {
    Vec3 n_norm = norm(n);
    Mat3 n_cross = crossM(n_norm);
    Mat3 E(1.0);

    Mat3 R = E + n_cross * sin(theta) + (n_cross * n_cross) * (1 - cos(theta));

    Mat4 res(1.0);
    res[0][0] = R[0][0];
    res[0][1] = R[0][1];
    res[0][2] = R[0][2];
    res[1][0] = R[1][0];
    res[1][1] = R[1][1];
    res[1][2] = R[1][2];
    res[2][0] = R[2][0];
    res[2][1] = R[2][1];
    res[2][2] = R[2][2];

    return res;
}

inline Mat4 rotateP(float theta, const Vec3 &n, const Vec3 &P) {
    return translate(P.x, P.y, P.z) *
           (rotate(theta, n) * translate(-P.x, -P.y, -P.z));
}

inline Mat4 lookAt(const Vec3 &S, const Vec3 &P, const Vec3 &u) {
    Vec3 e3 = norm(S - P);
    Vec3 e1 = norm(cross(u, e3));
    Vec3 e2 = cross(e3, e1);

    Mat4 R(1.f);
    R[0][0] = e1.x;
    R[0][1] = e1.y;
    R[0][2] = e1.z;
    R[1][0] = e2.x;
    R[1][1] = e2.y;
    R[1][2] = e2.z;
    R[2][0] = e3.x;
    R[2][1] = e3.y;
    R[2][2] = e3.z;

    Mat4 T = translate(-S.x, -S.y, -S.z);

    return R * T;
}

inline Mat4 ortho(float l, float r, float b, float t, float zn, float zf) {
    return Mat4(Vec4(2.f / (r - l), 0.f, 0.f, -(r + l) / (r - l)),
                Vec4(0.f, 2.f / (t - b), 0.f, -(t + b) / (t - b)),
                Vec4(0.f, 0.f, -2.f / (zf - zn), -(zf + zn) / (zf - zn)),
                Vec4(0.f, 0.f, 0.f, 1.f));
}

inline Mat4 frustum(float l, float r, float b, float t, float n, float f) {
    return Mat4(Vec4(2 * n / (r - l), 0, (r + l) / (r - l), 0),
                Vec4(0, 2 * n / (t - b), (t + b) / (t - b), 0),
                Vec4(0, 0, -(f + n) / (f - n), -2 * f * n / (f - n)),
                Vec4(0, 0, -1, 0));
}

inline Mat4 perspective(float fovy, float aspect, float n, float f) {
    float ctg_fovy_2 = 1.0f / tan(fovy / 2.0f);
    return Mat4(Vec4(ctg_fovy_2 / aspect, 0, 0, 0), Vec4(0, ctg_fovy_2, 0, 0),
                Vec4(0, 0, -(f + n) / (f - n), -2 * f * n / (f - n)),
                Vec4(0, 0, -1, 0));
}

inline Mat3 cadrRL(Vec2 Vc, Vec2 V, Vec2 Wc, Vec2 W) {
    return translate(Wc.x, Wc.y) *
           (scale(W.x / V.x, -W.y / V.y) * translate(-Vc.x, -Vc.y));
}
