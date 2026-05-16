#pragma once

#include "matrix.hpp"
#include "transform.hpp"
#include <raylib.h>

struct Screen {
        float left = 30.0f, right = 100.0f, top = 20.0f, bottom = 50.0f;
        float minX, maxX;
        float minY, maxY;
        float Wx, Wy, Wcx, Wcy;
        float Rx, Ry, Rcx, Rcy;

        Mat4 T = Mat4(1.f);
        Mat4 initT = Mat4(1.f);

        Vec3 S, P, u; // Camera

        float dist;         // Distance between S and P
        float fovy, aspect; // FOV and viewport aspect ratio
        float fovy_work, aspect_work;
        float near, far;
        float n, f;
        float l, r, t, b;
        enum projType { ORTHO, FRUSTUM, PERSPECTIVE } pType = ORTHO;

        void update() {
            Wx = static_cast<float>(GetScreenWidth());
            Wy = static_cast<float>(GetScreenHeight());
            Wcx = Wx / 2.0f;
            Wcy = Wy / 2.0f;
            minX = left;
            maxX = Wx - right;
            minY = top;
            maxY = Wy - bottom;
            Rx = maxX - minX;
            Ry = maxY - minY;
            Rcx = minX;
            Rcy = maxY;
            if (Ry != 0) {
                aspect_work = Rx / Ry;
            }
        }

        void initWorkPars() {
            n = near;
            f = far;
            fovy_work = fovy;

            float Vy = 2 * n * tan(fovy / 2.f);
            float Vx = aspect_work * Vy;
            l = -Vx / 2.f;
            r = Vx / 2.f;
            b = -Vy / 2.f;
            t = Vy / 2.f;

            dist = length(P - S);
            T = lookAt(S, P, u);
        }
};
