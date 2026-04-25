#include "clip.hpp"
#include "figure.hpp"
#include "matrix.hpp"
#include "nfd.h"
#include "raygui.h"
#include "screen.hpp"
#include "transform.hpp"
#include <raylib.h>

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 400, "Lab4");
    SetWindowMinSize(170, 70);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    if (NFD_Init() != NFD_OKAY) {
        std::cerr << "Ошибка инициализации NFD: " << NFD_GetError() << "\n";
        return -1;
    }

    std::vector<ssu::ModelFig> models;
    Screen s;

    while (!WindowShouldClose()) {
        float screenWidth = static_cast<float>(GetScreenWidth());
        float screenHeigth = static_cast<float>(GetScreenHeight());
        s.rectCalc(screenWidth, screenHeigth);

        if (IsKeyPressed(KEY_ESCAPE)) {
            s.T = Mat3(1);
        }
        // Одиночный сдвиг на 1 пиксель
        if (IsKeyDown(KEY_W))
            s.T = translate(0, -1) * s.T; // Вверх
        if (IsKeyDown(KEY_S))
            s.T = translate(0, 1) * s.T; // Вниз
        if (IsKeyDown(KEY_A))
            s.T = translate(-1, 0) * s.T; // Влево
        if (IsKeyDown(KEY_D))
            s.T = translate(1, 0) * s.T; // Вправо

        // Усиленный сдвиг на 10 пикселей
        if (IsKeyDown(KEY_T))
            s.T = translate(0, -10) * s.T; // Вверх
        if (IsKeyDown(KEY_G))
            s.T = translate(0, 10) * s.T; // Вниз
        if (IsKeyDown(KEY_F))
            s.T = translate(-10, 0) * s.T; // Влево
        if (IsKeyDown(KEY_H))
            s.T = translate(10, 0) * s.T; // Вправо

        // Повороты относительно центра окна
        if (IsKeyDown(KEY_Q)) { // Против часовой на 0.01
            s.T = translate(s.CenterX, s.CenterY) * rotate(-0.01f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_E)) { // По часовой на 0.01
            s.T = translate(s.CenterX, s.CenterY) * rotate(0.01f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_Y)) { // Против часовой на 0.05
            s.T = translate(s.CenterX, s.CenterY) * rotate(-0.05f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_R)) { // По часовой на 0.05
            s.T = translate(s.CenterX, s.CenterY) * rotate(0.05f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }

        // Равномерное масштабирование относительно центра (Z, X)
        if (IsKeyDown(KEY_Z)) { // Увеличение в 1.1 раза
            s.T = translate(s.CenterX, s.CenterY) * scale(1.1f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_X)) { // Уменьшение в 1.1 раза
            s.T = translate(s.CenterX, s.CenterY) * scale(1.0f / 1.1f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }

        // Растяжение/сжатие по осям относительно центра
        if (IsKeyDown(KEY_I)) { // Растяжение по X
            s.T = translate(s.CenterX, s.CenterY) * scale(1.1f, 1.0f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_K)) { // Сжатие по X
            s.T = translate(s.CenterX, s.CenterY) * scale(1.0f / 1.1f, 1.0f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_O)) { // Растяжение по Y
            s.T = translate(s.CenterX, s.CenterY) * scale(1.0f, 1.1f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyDown(KEY_L)) { // Сжатие по Y
            s.T = translate(s.CenterX, s.CenterY) * scale(1.0f, 1.0f / 1.1f) *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }

        // Зеркальное отражение относительно центра
        if (IsKeyPressed(KEY_U)) { // Отражение по горизонтали (инвертируем Y)
            s.T = translate(s.CenterX, s.CenterY) * mirrorY() *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }
        if (IsKeyPressed(KEY_J)) { // Отражение по вертикали (инвертируем X)
            s.T = translate(s.CenterX, s.CenterY) * mirrorX() *
                  translate(-s.CenterX, -s.CenterY) * s.T;
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawRectangleLinesEx({s.minX, s.minY, screenWidth - s.left - s.right,
                              screenHeigth - s.top - s.bottom},
                             2, BLACK);

        if (GuiButton({screenWidth - 120, 20, 100, 30}, "OPEN FILE")) {
            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[2] = {{"Text files", "txt"},
                                             {"All files", "*"}};
            nfdresult_t result =
                NFD_OpenDialog(&outPath, filterItem, 2, nullptr);

            if (result == NFD_OKAY) {
                models = s.getModels(outPath);
                NFD_FreePath(outPath);
            } else if (result == NFD_CANCEL) {
                std::cerr << "INFO: NFD: user pressed cancel" << std::endl;
            } else {
                std::cerr << "ERROR: " << NFD_GetError() << std::endl;
            }
        }

        for (const auto &model : models) {
            Mat3 TM = s.T * model.modelM;
            for (const auto &lines : model.figure) {
                Vec2 start = normalize(TM * Vec3(lines.vertices[0], 1));
                for (const auto &line : lines.vertices) {
                    Vec2 end = normalize(TM * Vec3(line, 1));
                    Vec2 tmpEnd = end;
                    if (clip(start, end, s.minX, s.minY, s.maxX, s.maxY)) {
                        DrawLineEx({start.x, start.y}, {end.x, end.y},
                                   lines.thickness, lines.color);
                    }
                    start = tmpEnd;
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
    NFD_Quit();
    return 0;
}
