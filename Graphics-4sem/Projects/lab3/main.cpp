#include "figure.hpp"
#include "hare.hpp"
#include "matrix.hpp"
#include "nfd.h"
#include "raygui.h"
#include "transform.hpp"
#include <fstream>
#include <raylib.h>
#include <sstream>
#include <vector>

Font LoadRussianFontStatic(const char *fontPath, int fontSize) {
    std::vector<int> codepoints;
    for (int i = 0; i <= 0x04FF; i++) {
        codepoints.push_back(i);
    }
    Font font =
        LoadFontEx(fontPath, fontSize, codepoints.data(), codepoints.size());
    return font;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Second Lab");
    SetTargetFPS(60);

    Font f = LoadRussianFontStatic("Assets/Roboto-Regular.ttf", 100);

    bool keepAspect = true;
    bool imageAspect = true;

    ssu::Figure figure = ssu::figure::HARE;

    Mat3 T = Mat3(1.f); // матрица, в которой накапливаются все преобразования
                        // первоначально - единичная матрица
    Mat3 initT;         // матрица начального преобразования

    while (!WindowShouldClose()) {

        const float Wx = static_cast<float>(GetScreenWidth());
        const float Wy = static_cast<float>(GetScreenHeight());
        const float Wcx = Wx / 2.0f;
        const float Wcy = Wy / 2.0f;
        float figureAspect = figure.Vx / figure.Vy;
        const float windowAspect = Wx / Wy;
        float S = figureAspect < windowAspect ? Wy / figure.Vy : Wx / figure.Vx;

        if (IsKeyPressed(KEY_M)) {
            keepAspect = !keepAspect;
        }

        if (IsKeyPressed(KEY_N)) {
            imageAspect = !imageAspect;
        }

        if (IsKeyDown(KEY_Q)) {
            T = translate(-Wcx, -Wcy) *
                T;                  // перенос начала координат в (Wcx, Wcy)
            T = rotate(-0.01f) * T; // поворот на -0.01 радиан относительно
            // нового центра
            T = translate(Wcx, Wcy) * T; // перенос начала координат обратно
        }

        if (IsKeyDown(KEY_W)) {
            T = translate(0, -1) * T;
        }

        if (IsKeyPressed(KEY_C)) {
            T = Mat3(1);
        }

        if (imageAspect) {
            figure = ssu::figure::HARE;
        }

        float Sx, Sy;
        if (keepAspect) {
            float figureAspect = figure.Vx / figure.Vy;
            Sx = Sy =
                figureAspect < windowAspect ? Wy / figure.Vy : Wx / figure.Vx;
        } else {
            Sx = Wx / figure.Vx;
            Sy = Wy / figure.Vy;
        }

        const float Ty = Sy * figure.Vy; // смещение в положительную сторону по
                                         // оси Oy после смены знака
        // Преобразования применяются справа налево. Сначала масштабирование, а
        // потом перенос. В initT совмещаем эти два преобразования.
        initT = translate(0, Ty) * scale(Sx, -Sy);

        // совмещение начального преобразования и накопленных преобразований
        Mat3 M = T * initT;
        BeginDrawing();
        ClearBackground(SKYBLUE);

        for (const auto &lines : figure.paths) {
            Vec2 start = normalize(M * Vec3(lines.vertices[0], 1));
            for (const auto &line : lines.vertices) {
                const Vec2 end = normalize(M * Vec3(line, 1));
                DrawLineEx({start.x, start.y}, {end.x, end.y}, lines.thickness,
                           lines.color);
                start = end;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
