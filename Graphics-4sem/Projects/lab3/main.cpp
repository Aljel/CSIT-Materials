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

bool isIgnorableLine(const std::string &line) {
    return line.find_first_not_of(" \t\r\n") == std::string::npos ||
           line.front() == '#';
}

ssu::Figure readFromFile(const char *fileName) {
    std::ifstream in(fileName);
    ssu::Figure figure;
    int r, g, b;
    float thickness;
    std::string line; // временная переменная, в которую считываются строки
    while (in) {
        // считываем очередную строку
        getline(in, line);
        if (isIgnorableLine(line)) {
            continue;
        }
        std::stringstream s(line);
        std::string cmd;      // переменная для имени команды
        s >> cmd;             // считываем имя команды
        if (cmd == "frame") { // размеры изображения
            s >> figure.Vx >> figure.Vy;
            std::cout << figure.Vx << ' ' << figure.Vy << std::endl;
        } else if (cmd == "color") {     // цвет линии
            s >> r >> g >> b;            // считываем три компоненты цвета
        } else if (cmd == "thickness") { // толщина линии
            s >> thickness;              // считываем значение толщины
        } else if (cmd == "path") {      // набор точек
            std::vector<Vec2> vertices;  // список точек ломаной
            int n;                       // количество точек
            s >> n;
            std::string str1; // дополнительная строка для чтения из файла
            while (n > 0) {   // пока не все точки считаны
                getline(in, str1);
                if (isIgnorableLine(str1)) {
                    continue;
                }
                float x, y;
                std::stringstream s1(str1);
                s1 >> x >> y;
                vertices.push_back(Vec2(x, y)); // добавляем точку в список
                --n;
            }
            // все точки считаны, генерируем ломаную (path) и кладем ее в список
            figure.paths.push_back(
                ssu::Path(vertices,
                          Color{static_cast<unsigned char>(r),
                                static_cast<unsigned char>(g),
                                static_cast<unsigned char>(b), 255},
                          thickness));
        }
    }
    return figure;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Third Lab");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    if (NFD_Init() != NFD_OKAY) {
        std::cerr << "Ошибка инициализации NFD: " << NFD_GetError() << "\n";
        return -1;
    }

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
        if (IsKeyPressed(KEY_ESCAPE)) {
            T = Mat3(1);
        }
        // Одиночный сдвиг на 1 пиксель
        if (IsKeyDown(KEY_W))
            T = translate(0, -1) * T; // Вверх
        if (IsKeyDown(KEY_S))
            T = translate(0, 1) * T; // Вниз
        if (IsKeyDown(KEY_A))
            T = translate(-1, 0) * T; // Влево
        if (IsKeyDown(KEY_D))
            T = translate(1, 0) * T; // Вправо

        // Усиленный сдвиг на 10 пикселей
        if (IsKeyDown(KEY_T))
            T = translate(0, -10) * T; // Вверх
        if (IsKeyDown(KEY_G))
            T = translate(0, 10) * T; // Вниз
        if (IsKeyDown(KEY_F))
            T = translate(-10, 0) * T; // Влево
        if (IsKeyDown(KEY_H))
            T = translate(10, 0) * T; // Вправо

        // Повороты относительно центра окна
        if (IsKeyDown(KEY_Q)) { // Против часовой на 0.01
            T = translate(Wcx, Wcy) * rotate(-0.01f) * translate(-Wcx, -Wcy) *
                T;
        }
        if (IsKeyDown(KEY_E)) { // По часовой на 0.01
            T = translate(Wcx, Wcy) * rotate(0.01f) * translate(-Wcx, -Wcy) * T;
        }
        if (IsKeyDown(KEY_Y)) { // Против часовой на 0.05
            T = translate(Wcx, Wcy) * rotate(-0.05f) * translate(-Wcx, -Wcy) *
                T;
        }
        if (IsKeyDown(KEY_R)) { // По часовой на 0.05
            T = translate(Wcx, Wcy) * rotate(0.05f) * translate(-Wcx, -Wcy) * T;
        }

        // Равномерное масштабирование относительно центра (Z, X)
        if (IsKeyDown(KEY_Z)) { // Увеличение в 1.1 раза
            T = translate(Wcx, Wcy) * scale(1.1f) * translate(-Wcx, -Wcy) * T;
        }
        if (IsKeyDown(KEY_X)) { // Уменьшение в 1.1 раза
            T = translate(Wcx, Wcy) * scale(1.0f / 1.1f) *
                translate(-Wcx, -Wcy) * T;
        }

        // Растяжение/сжатие по осям относительно центра
        if (IsKeyDown(KEY_I)) { // Растяжение по X
            T = translate(Wcx, Wcy) * scale(1.1f, 1.0f) *
                translate(-Wcx, -Wcy) * T;
        }
        if (IsKeyDown(KEY_K)) { // Сжатие по X
            T = translate(Wcx, Wcy) * scale(1.0f / 1.1f, 1.0f) *
                translate(-Wcx, -Wcy) * T;
        }
        if (IsKeyDown(KEY_O)) { // Растяжение по Y
            T = translate(Wcx, Wcy) * scale(1.0f, 1.1f) *
                translate(-Wcx, -Wcy) * T;
        }
        if (IsKeyDown(KEY_L)) { // Сжатие по Y
            T = translate(Wcx, Wcy) * scale(1.0f, 1.0f / 1.1f) *
                translate(-Wcx, -Wcy) * T;
        }

        // Зеркальное отражение относительно центра
        if (IsKeyPressed(KEY_U)) { // Отражение по горизонтали (инвертируем Y)
            T = translate(Wcx, Wcy) * mirrorY() * translate(-Wcx, -Wcy) * T;
        }
        if (IsKeyPressed(KEY_J)) { // Отражение по вертикали (инвертируем X)
            T = translate(Wcx, Wcy) * mirrorX() * translate(-Wcx, -Wcy) * T;
        }

        Mat3 M = T * initT;
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        if (GuiButton({Wx - 140, 20, 120, 30}, "OPEN FILE")) {
            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[2] = {{"Text files", "txt"},
                                             {"All files", "*"}};
            nfdresult_t result =
                NFD_OpenDialog(&outPath, filterItem, 2, nullptr);
            if (result == NFD_OKAY) {
                figure = readFromFile(outPath);
                const float figureAspect = figure.Vx / figure.Vy;
                const float S = figureAspect < windowAspect ? Wy / figure.Vy
                                                            : Wx / figure.Vx;
                const float Ty = S * figure.Vy;
                initT = translate(0, Ty) * scale(S, -S);
                NFD_FreePath(outPath);
            }
        }

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
    NFD_Quit();
    return 0;
}
