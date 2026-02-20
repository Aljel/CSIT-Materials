#include "figure.hpp"
#include "hare.hpp"
#include <raylib.h>
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
    InitWindow(800, 600, "First Lab");
    SetTargetFPS(60);

    Font f = LoadRussianFontStatic("Assets/Roboto-Regular.ttf", 100);

    ssu::Figure &figure = ssu::figure::SNAIL;

    bool keepAspect = true;

    while (!WindowShouldClose()) {
        const float Wx = static_cast<float>(GetScreenWidth());
        const float Wy = static_cast<float>(GetScreenHeight());
        float figureAspect = figure.Vx / figure.Vy;
        const float windowAspect = Wx / Wy;
        float S = figureAspect < windowAspect ? Wy / figure.Vy : Wx / figure.Vx;

        if (IsKeyPressed(KEY_M)) {
            keepAspect = !keepAspect;
        }

        BeginDrawing();

        ClearBackground(SKYBLUE);

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
        for (size_t i = 0; i < figure.vertices.size(); i += 4) {
            DrawLineEx(
                {Sx * figure.vertices[i], Ty - Sy * figure.vertices[i + 1]},
                {Sx * figure.vertices[i + 2], Ty - Sy * figure.vertices[i + 3]},
                2, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
