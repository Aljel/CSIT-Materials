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
