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

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(SKYBLUE);

        const float Wx = static_cast<float>(GetScreenWidth());
        const float Wy = static_cast<float>(GetScreenHeight());

        DrawLineEx({0, 0}, {Wx, Wy}, 6, RED);
        DrawLineEx({90, 50}, {Wx, 80}, 10, BLUE);

        Vector2 p1{Wx * 2 / 3, 0};  // верх
        Vector2 p2{Wx, Wy * 2 / 3}; // право
        Vector2 p3{Wx * 1 / 3, Wy}; // низ
        Vector2 p4{0, Wy * 1 / 3};  // лево

        DrawLineEx(p1, p2, 5, GREEN);
        DrawLineEx(p2, p3, 5, GREEN);
        DrawLineEx(p3, p4, 5, GREEN);
        DrawLineEx(p4, p1, 5, GREEN);

        DrawTextEx(f, "Надпись на форме", {40, 380}, 26, 0, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
