#include "clip.hpp"
#include "figure.hpp"
#include "matrix.hpp"
#include "nfd.h"
#include "raygui.h"
#include "screen.hpp"
#include "transform.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <string>

float max(float a, float b, float c) {
    if (a >= b && a >= c)
        return a;
    else if (b >= c)
        return b;
    else
        return c;
}

bool isIgnorableLine(const std::string &line) {
    return line.find_first_not_of(" \t\r\n") == std::string::npos ||
           line.front() == '#';
}

std::vector<ssu::Model> readFromFile(const char *fileName, Screen &screen) {
    std::ifstream in(fileName);
    std::vector<ssu::Model> models;
    Mat4 M = Mat4(1.f);
    Mat4 initM;
    std::vector<ssu::Path> figure;
    std::vector<Mat4> transforms;
    float mVcx, mVcy, mVcz, mVx, mVy, mVz;
    int r = 0, g = 0, b = 0;
    float thickness = 1.0f;
    std::string line; // Временная переменная, в которую считываются строки

    while (getline(in, line)) {
        if (isIgnorableLine(line)) {
            continue;
        }
        std::stringstream s(line);
        std::string cmd; // Переменная для имени команды
        s >> cmd;        // Считываем имя команды
        if (cmd == "camera") {
            // Координаты точки наблюдения
            s >> screen.S.x >> screen.S.y >> screen.S.z;
            // Точка, в которую направлен вектор наблюдения
            s >> screen.P.x >> screen.P.y >> screen.P.z;
            // Вектор направления вверх
            s >> screen.u.x >> screen.u.y >> screen.u.z;
        } else if (cmd == "screen") {
            s >> screen.fovy_work >> screen.aspect >> screen.near >> screen.far;
            screen.fovy = screen.fovy_work / 180.f * PI;
        } else if (cmd == "color") {     // Цвет линии
            s >> r >> g >> b;            // Считываем три компоненты цвета
        } else if (cmd == "thickness") { // Толщина линии
            s >> thickness;              // Считываем значение толщины
        } else if (cmd == "path") {      // Набор точек
            std::vector<Vec3> vertices;  // Список точек ломаной
            int n;                       // Количество точек
            s >> n;
            std::string str1; // Дополнительная строка для чтения из файла
            while (n > 0) {   // Пока не все точки считаны
                getline(in, str1);
                if (isIgnorableLine(str1)) {
                    continue;
                }
                float x, y, z;
                std::stringstream s1(str1);
                s1 >> x >> y >> z;
                vertices.push_back(Vec3(x, y, z)); // Добавляем точку в список
                --n;
            }
            // Все точки считаны, генерируем ломаную (path) и кладем ее в список
            figure.push_back(ssu::Path(vertices,
                                       Color{static_cast<uint8_t>(r),
                                             static_cast<uint8_t>(g),
                                             static_cast<uint8_t>(b), 255},
                                       thickness));
        } else if (cmd == "model") {
            // Считываем значения переменных
            s >> mVcx >> mVcy >> mVcz >> mVx >> mVy >> mVz;
            float S = 2.f / max(mVx, mVy, mVz);
            // Сдвиг точки привязки из начала координат в нужную позицию
            // После которого проводим масштабирование
            initM = scale(S, S, S) * translate(-mVcx, -mVcy, -mVcz);
            figure.clear();
        } else if (cmd == "figure") {
            models.push_back(
                ssu::Model(figure, M * initM)); // Добавляем рисунок в список
        } else if (cmd == "translate") {
            float Tx, Ty, Tz;    // Параметры преобразования переноса
            s >> Tx >> Ty >> Tz; // Считываем параметры
            // Добавляем перенос к общему преобразованию
            M = translate(Tx, Ty, Tz) * M;
        } else if (cmd == "scale") {
            float S; // Параметр масштабирования
            s >> S;  // Считываем параметр
            // Добавляем масштабирование к общему преобразованию
            M = scale(S, S, S) * M;
        } else if (cmd == "rotate") {
            float theta;                  // Угол поворота в градусах
            float nx, ny, nz;             // Координаты вектора поворота
            s >> theta >> nx >> ny >> nz; // Считываем параметр
            // Добавляем поворот к общему преобразованию
            M = rotate(theta / 180.f * PI, Vec3(nx, ny, nz)) * M;
        } else if (cmd == "pushTransform") {
            transforms.push_back(M); // сохраняем матрицу в стек
        } else if (cmd == "popTransform") {
            M = transforms.back(); // получаем верхний элемент стека
            transforms.pop_back(); // выкидываем матрицу из стека
        }
    }
    screen.update();
    screen.initWorkPars();
    return models;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 800, "Lab6");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    if (NFD_Init() != NFD_OKAY) {
        std::cerr << "Ошибка инициализации NFD: " << NFD_GetError() << "\n";
        return -1;
    }

    std::vector<ssu::Model> models;
    Screen s;

    while (!WindowShouldClose()) {
        float screenWidth = static_cast<float>(GetScreenWidth());
        float screenHeigth = static_cast<float>(GetScreenHeight());

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        s.update();
        DrawRectangleLinesEx({s.minX, s.minY, screenWidth - s.left - s.right,
                              screenHeigth - s.top - s.bottom},
                             2, BLACK);
        if (IsKeyPressed(KEY_ESCAPE)) {
            s.initWorkPars();
        }
        if (IsKeyPressed(KEY_ONE)) {
            s.pType = s.ORTHO;
        }
        if (IsKeyPressed(KEY_TWO)) {
            s.pType = s.FRUSTUM;
        }
        if (IsKeyPressed(KEY_THREE)) {
            s.pType = s.PERSPECTIVE;
        }
        if (IsKeyDown(KEY_W)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = lookAt(Vec3(0, 0, -0.1f), Vec3(0, 0, -0.2f),
                             Vec3(0, 1.f, 0)) *
                      s.T;
            } else {
                s.T = lookAt(Vec3(0, 0, -1.f), Vec3(0, 0, -2.f),
                             Vec3(0, 1.f, 0)) *
                      s.T;
            }
        }
        if (IsKeyDown(KEY_S)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = lookAt(Vec3(0, 0, 0.1f), Vec3(0, 0, 0), Vec3(0, 1.f, 0)) *
                      s.T;
            } else {
                s.T = lookAt(Vec3(0, 0, 1.f), Vec3(0, 0, 0), Vec3(0, 1.f, 0)) *
                      s.T;
            }
        }
        if (IsKeyDown(KEY_A)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = lookAt(Vec3(-0.1f, 0, 0), Vec3(-0.1f, 0, -1.f),
                             Vec3(0, 1.f, 0)) *
                      s.T;
            } else {
                s.T = lookAt(Vec3(-1.f, 0, 0), Vec3(-1.f, 0, -1.f),
                             Vec3(0, 1.f, 0)) *
                      s.T;
            }
        }
        if (IsKeyDown(KEY_D)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = lookAt(Vec3(0.1f, 0, 0), Vec3(0.1f, 0, -1.f),
                             Vec3(0, 1.f, 0)) *
                      s.T;
            } else {
                s.T = lookAt(Vec3(1.f, 0, 0), Vec3(1.f, 0, -1.f),
                             Vec3(0, 1.f, 0)) *
                      s.T;
            }
        }
        if (IsKeyDown(KEY_R)) {
            Vec3 u_new = Mat3(rotate(0.1f, Vec3(0, 0, 1.f))) * Vec3(0, 1.f, 0);
            s.T = lookAt(Vec3(0, 0, 0), Vec3(0, 0, -1.f), u_new) * s.T;
        }
        if (IsKeyDown(KEY_T)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                Mat4 M = rotateP(0.1f, Vec3(1.f, 0, 0), Vec3(0, 0, -s.dist));
                Vec3 u_new = Mat3(M) * Vec3(0, 1.f, 0);
                Vec3 S_new = normalize(M * Vec4(0, 0, 0, 1.f));
                s.T = lookAt(S_new, Vec3(0, 0, -s.dist), u_new) * s.T;

            } else {
                Mat4 M = rotate(0.1f, Vec3(1.f, 0, 0));
                Vec3 u_new = Mat3(M) * Vec3(0, 1.f, 0);
                Vec3 P_new = normalize(M * Vec4(0, 0, -1.f, 1.f));
                s.T = lookAt({0, 0, 0}, P_new, u_new) * s.T;
            }
        }
        if (IsKeyDown(KEY_I)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.t -= 1;
            } else {
                s.t += 1;
            }
        }
        if (IsKeyDown(KEY_J)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.l -= 1;
            } else {
                s.l += 1;
            }
        }
        if (IsKeyDown(KEY_Y)) {
            s.T = rotate(0.1f, Vec3(0, 0, 1)) * s.T;
        }

        if (IsKeyDown(KEY_G)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = rotateP(0.1f, Vec3(1, 0, 0), s.P) * s.T;
            } else {
                s.T = rotate(0.1f, Vec3(1, 0, 0)) * s.T;
            }
        }
        if (IsKeyDown(KEY_F)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = rotateP(-0.1f, Vec3(0, 1, 0), s.P) * s.T;
            } else {
                s.T = rotate(-0.1f, Vec3(0, 1, 0)) * s.T;
            }
        }
        if (IsKeyDown(KEY_H)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.T = rotateP(0.1f, Vec3(0, 1, 0), s.P) * s.T;
            } else {
                s.T = rotate(0.1f, Vec3(0, 1, 0)) * s.T;
            }
        }

        if (IsKeyDown(KEY_K)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.b -= 1.0f;
            } else {
                s.b += 1.0f;
            }
        }
        if (IsKeyDown(KEY_L)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.r -= 1.0f;
            } else {
                s.r += 1.0f;
            }
        }
        if (IsKeyDown(KEY_U)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.n -= 0.2f;
            } else {
                s.n += 0.2f;
            }
            s.n = std::max(0.1f, s.n);
            s.n = std::min(s.n, s.f - 0.1f);
        }
        if (IsKeyDown(KEY_O)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.f -= 0.2f;
            } else {
                s.f += 0.2f;
            }
            s.f = std::max(s.n + 0.1f, s.f);
        }
        if (IsKeyDown(KEY_B)) {
            Vec3 view_dir = norm(s.P - s.S);
            float step = 0.2f;
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                if (length(s.P - s.S) - step >= 0.1f) {
                    s.S += view_dir * step;
                    s.initWorkPars();
                }
            } else {
                s.S -= view_dir * step;
                s.initWorkPars();
            }
        }
        if (IsKeyDown(KEY_Z)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.fovy_work -= 0.1f;
            } else {
                s.fovy_work += 0.1f;
            }
            s.fovy_work = std::max(0.3f, s.fovy_work);
            s.fovy_work = std::min(3.0f, s.fovy_work);
        }
        if (IsKeyDown(KEY_X)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                s.aspect_work -= 0.05f;
            } else {
                s.aspect_work += 0.05f;
            }
            s.aspect_work = std::max(0.01f, s.aspect_work);
        }

        if (GuiButton({s.Wx - 140, 20, 120, 30}, "OPEN FILE")) {
            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[2] = {{"Text files", "txt"},
                                             {"All files", "*"}};
            nfdresult_t result =
                NFD_OpenDialog(&outPath, filterItem, 2, nullptr);

            if (result == NFD_OKAY) {
                models = readFromFile(outPath, s);
                NFD_FreePath(outPath);
            } else if (result == NFD_CANCEL) {
                std::cerr << "INFO: NFD: user pressed cancel" << "\n";
            } else {
                std::cerr << "ERROR: " << NFD_GetError() << "\n";
            }
        }

        Mat4 proj; // матрица перехода в пространство отсечения
        switch (s.pType) {
        case s.ORTHO:
            proj = ortho(s.l, s.r, s.b, s.t, -s.n, -s.f);
            break;
        case s.FRUSTUM:
            proj = frustum(s.l, s.r, s.b, s.t, s.n, s.f);
            break;
        case s.PERSPECTIVE:
            proj = perspective(s.fovy_work, s.aspect_work, s.n, s.f);
            break;
        }

        Mat3 cdr = cadrRL(Vec2(-1.f, 1.f), Vec2(2.f, 2.f), Vec2(s.minX, s.minY),
                          Vec2(s.Rx, s.Ry));
        Mat4 C = proj * s.T;
        for (const auto &model : models) {
            Mat4 TM = C * model.modelM;
            for (const auto &lines : model.paths) {
                Vec3 start_3D = normalize(TM * Vec4(lines.vertices[0], 1.f));
                Vec2 start = normalize(cdr * Vec3(Vec2(start_3D), 1.f));
                for (const auto &line : lines.vertices) {
                    Vec3 end_3D = normalize(TM * Vec4(line, 1.f));
                    Vec2 end = normalize(cdr * Vec3(Vec2(end_3D), 1.f));
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
