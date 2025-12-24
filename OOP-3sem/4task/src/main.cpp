// Обновленный файл main.cpp с русским текстом на всех кнопках

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "train_simulation.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <thread>

// 2 аргумента: шанс задержки и шанс происшествия в процентах
RailwayNetwork network(80, 20);

bool isRunning = false;
int lastEventCount = 0;
bool showAddTrainDialog = false;
int dialogState = 0;
std::string newRouteName = "";
int newRouteStart = 0;
int newRouteEnd = 5;
std::vector<int> selectedStops;

Font LoadRussianFontStatic(const char *fontPath, int fontSize)
{
    std::vector<int> codepoints;
    for (int i = 0; i <= 0x04FF; i++)
    {
        codepoints.push_back(i);
    }
    Font font = LoadFontEx(fontPath, fontSize, codepoints.data(), codepoints.size());
    return font;
}

void initializeNetwork()
{
    network.addStation("Москва", 0);
    network.addStation("Тула", 100);
    network.addStation("Орел", 200);
    network.addStation("Курск", 300);
    network.addStation("Воронеж", 400);
    network.addStation("Тамбов", 500);

    network.addRoute("Первый путь", 0, 4);
    network.addRoute("Второй путь", 1, 4);
    network.addRoute("Третий путь", 2, 5);
    network.addRoute("Четвертый путь", 0, 3);
    network.addRoute("Пятый путь", 1, 2);

    Route *r0 = network.getRoute(0);
    if (r0)
    {
        r0->addStop(StationStop(0, Time(6, 0), Time(6, 5), 5));
        r0->addStop(StationStop(1, Time(7, 0), Time(7, 5), 5));
        r0->addStop(StationStop(2, Time(8, 0), Time(8, 5), 5));
        r0->addStop(StationStop(3, Time(9, 0), Time(9, 5), 5));
        r0->addStop(StationStop(4, Time(10, 0), Time(10, 5), 5));
        r0->addStop(StationStop(5, Time(11, 0), Time(11, 3), 3));
    }

    Route *r1 = network.getRoute(1);
    if (r1)
    {
        r1->addStop(StationStop(1, Time(6, 30), Time(6, 35), 5));
        r1->addStop(StationStop(2, Time(7, 30), Time(7, 35), 5));
        r1->addStop(StationStop(3, Time(8, 30), Time(8, 35), 5));
        r1->addStop(StationStop(4, Time(9, 30), Time(9, 30), 0));
    }

    Route *r2 = network.getRoute(2);
    if (r2)
    {
        r2->addStop(StationStop(2, Time(7, 0), Time(7, 5), 5));
        r2->addStop(StationStop(3, Time(8, 0), Time(8, 5), 5));
        r2->addStop(StationStop(4, Time(9, 0), Time(9, 5), 5));
        r2->addStop(StationStop(5, Time(10, 0), Time(10, 0), 0));
    }

    Route *r3 = network.getRoute(3);
    if (r3)
    {
        r3->addStop(StationStop(0, Time(8, 0), Time(8, 5), 5));
        r3->addStop(StationStop(1, Time(8, 50), Time(8, 55), 5));
        r3->addStop(StationStop(2, Time(9, 40), Time(9, 45), 5));
        r3->addStop(StationStop(3, Time(10, 30), Time(10, 30), 0));
    }

    Route *r4 = network.getRoute(4);
    if (r4)
    {
        r4->addStop(StationStop(1, Time(14, 0), Time(14, 5), 5));
        r4->addStop(StationStop(2, Time(14, 50), Time(14, 50), 0));
    }

    network.addTrain(0, -1);
    network.addTrain(1, -1);
    network.addTrain(2, -1);
    network.addTrain(3, -1);
    network.addTrain(4, -1);

    network.setSimulationStartTime(Time(6, 0));
    network.setSimulationStep(15);
}

void printNewEvents()
{
    std::vector<SimulationEvent> events = network.getEventLog();
    if (events.size() > lastEventCount)
    {
        for (size_t i = lastEventCount; i < events.size(); ++i)
        {
            std::cout << "[" << events[i].occurredAt.toString() << "] "
                      << events[i].description << "\n";
        }
    }
    lastEventCount = events.size();
}

void drawRailwayNetwork(Font customFont, Texture2D stationTexture)
{
    std::vector<Station> stations = network.getStations();
    std::vector<Train> trains = network.getTrains();

    DrawTextEx(customFont, "Симуляция движения поездов!", Vector2{280, 20}, 40, 0, DARKGRAY);
    DrawTextEx(customFont, ("Время: " + network.getSimulationTime().toString()).c_str(), Vector2{280, 60}, 30, 0, GRAY);

    DrawRectangle(300, 100, 1100, 400, Color{240, 240, 240, 255});

    float y = 300;
    DrawLine(300, y, 1350, y, Color{100, 100, 100, 255});

    for (std::vector<Station>::iterator it = stations.begin(); it != stations.end(); ++it)
    {
        float x = 300 + it->positionKm * 2;
        DrawTextureEx(stationTexture, {x - 50, y - 150}, 0, 0.4f, WHITE);
        DrawTextEx(customFont, it->name.c_str(), Vector2{x, y + 25}, 20, 0, DARKGRAY);
    }

    for (std::vector<Train>::iterator it = trains.begin(); it != trains.end(); ++it)
    {
        float x = 300 + it->getCurrentPosition() * 2;
        float ty = y + (it->id) * 30 + 80;

        Color col;
        switch (it->getStatus())
        {
        case TrainStatus::DELAYED:
            col = Color{255, 170, 0, 255};
            break;
        case TrainStatus::ACCIDENT:
            col = Color{255, 0, 0, 255};
            break;
        case TrainStatus::COMPLETED:
            col = Color{0, 221, 0, 255};
            break;
        case TrainStatus::AT_STATION:
            col = Color{100, 200, 255, 255};
            break;
        default:
            col = Color{255, 221, 0, 255};
        }

        DrawRectangle(x - 12, ty - 7, 35, 25, col);
        DrawTextEx(customFont, ("T" + std::to_string(it->id)).c_str(), Vector2{x - 8, ty - 8}, 15, 0, DARKGRAY);
    }
}

void drawScheduleWindow(Font customFont, float &RouteForSchedule)
{
    DrawRectangle(800, 600, 400, 350, Color{245, 245, 245, 255});
    DrawRectangleLines(800, 600, 400, 350, DARKGRAY);

    DrawTextEx(customFont, "Расписание", Vector2{810, 610}, 30, 0, DARKGRAY);

    Route *r = network.getRoute(RouteForSchedule);
    if (r)
    {
        DrawTextEx(customFont, ("Маршрут: " + r->name).c_str(), Vector2{810, 650}, 20, 0, DARKGRAY);

        float yPos = 680;
        std::vector<StationStop> stops = r->stops;

        for (std::vector<StationStop>::iterator it = stops.begin(); it != stops.end(); ++it)
        {
            Station *s = network.getStation(it->stationId);
            if (s)
            {
                std::string text = s->name + " " + it->arrivalTime.toString() + "-" + it->departureTime.toString();
                DrawTextEx(customFont, text.c_str(), Vector2{810, yPos}, 20, 0, GRAY);
                yPos += 30;
            }
        }
    }

    Rectangle routeSlider = {870, 900, 230, 20};
    int routeCount = network.getRoutes().size();
    if (routeCount > 0)
    {
        GuiSlider(routeSlider, "0", std::to_string(routeCount - 1).c_str(), &RouteForSchedule, 0, (routeCount - 1));
    }
}

void drawStatisticsWindow(Font customFont)
{
    DrawRectangle(300, 600, 400, 350, Color{245, 245, 245, 255});
    DrawRectangleLines(300, 600, 400, 350, DARKGRAY);

    DrawTextEx(customFont, "Статистика", Vector2{310, 610}, 30, 0, DARKGRAY);

    SimulationStatistics stats = network.getStatistics();

    float yPos = 660;

    DrawTextEx(customFont, ("Время: " + network.getSimulationTime().toString()).c_str(), Vector2{310, yPos}, 20, 0, GRAY);
    yPos += 30;

    DrawTextEx(customFont, ("События: " + std::to_string(stats.totalEvents)).c_str(), Vector2{310, yPos}, 20, 0, GRAY);
    yPos += 30;

    DrawTextEx(customFont, ("Задержки: " + std::to_string(stats.totalDelays)).c_str(), Vector2{310, yPos}, 20, 0, ORANGE);
    yPos += 30;

    DrawTextEx(customFont, ("Происшествия: " + std::to_string(stats.totalAccidents)).c_str(), Vector2{310, yPos}, 20, 0, RED);
    yPos += 30;

    DrawTextEx(customFont, ("Время задержки: " + std::to_string(stats.totalDelayMinutes) + " min").c_str(),
               Vector2{310, yPos}, 20, 0, GRAY);
}

void drawControlPanel(Font customFont)
{
    DrawRectangle(0, 0, 250, 1000, Color{200, 200, 200, 255});
    DrawRectangleLines(0, 0, 250, 1000, DARKGRAY);

    DrawTextEx(customFont, "Управление", Vector2{10, 10}, 30, 0, DARKGRAY);

    std::vector<Train> trains = network.getTrains();
    int completed = 0;

    for (std::vector<Train>::iterator it = trains.begin(); it != trains.end(); ++it)
    {
        if (it->getStatus() == TrainStatus::COMPLETED)
            completed++;
    }

    DrawTextEx(customFont, ("Время: " + network.getSimulationTime().toString()).c_str(), Vector2{10, 50}, 20, 0, GRAY);
    DrawTextEx(customFont, ("Поезда: " + std::to_string(trains.size())).c_str(), Vector2{10, 75}, 20, 0, GRAY);
    DrawTextEx(customFont, ("Завершено: " + std::to_string(completed)).c_str(), Vector2{10, 100}, 20, 0, GREEN);

    DrawTextEx(customFont, "Для выхода нажмите Esc. \nПоезда автоматически \nпроходят по заданному маршруту, \nпри этом задержки (оранжевые) \nи происшествия (красные) \nпроисходят с заданной в коде \nвероятностью.", Vector2{10, 150}, 18, 0, DARKGRAY);
}

void drawAddTrainDialog(Font customFont)
{
    if (!showAddTrainDialog)
        return;

    DrawRectangle(0, 0, 1800, 1000, Color{0, 0, 0, 100});
    DrawRectangle(500, 100, 800, 600, Color{245, 245, 245, 255});
    DrawRectangleLines(500, 100, 800, 600, DARKGRAY);

    if (dialogState == 0)
    {
        DrawTextEx(customFont, "Создание нового маршрута!", Vector2{530, 120}, 20, 0, DARKGRAY);
        DrawTextEx(customFont, "Имя маршрута:", Vector2{520, 170}, 14, 0, GRAY);

        DrawRectangle(520, 190, 300, 30, Color{255, 255, 255, 255});
        DrawRectangleLines(520, 190, 300, 30, DARKGRAY);
        DrawTextEx(customFont, newRouteName.c_str(), Vector2{525, 195}, 20, 0, DARKGRAY);

        DrawTextEx(customFont, "Начальная станция: ", Vector2{520, 260}, 20, 0, GRAY);

        // Кнопка "-" для начальной станции
        if (GuiButton({520, 280, 100, 30}, ""))
        {
            newRouteStart = (newRouteStart > 0) ? newRouteStart - 1 : 0;
        }
        DrawTextEx(customFont, "-", Vector2{555, 290}, 20, 0, DARKGRAY);

        // Кнопка "+" для начальной станции
        if (GuiButton({630, 280, 100, 30}, ""))
        {
            newRouteStart = (newRouteStart < 5) ? newRouteStart + 1 : 5;
        }
        DrawTextEx(customFont, "+", Vector2{663, 290}, 20, 0, DARKGRAY);

        DrawTextEx(customFont, "Конечная станция: ", Vector2{520, 330}, 20, 0, GRAY);

        // Кнопка "-" для конечной станции
        if (GuiButton({520, 350, 100, 30}, ""))
        {
            newRouteEnd = (newRouteEnd > 0) ? newRouteEnd - 1 : 0;
        }
        DrawTextEx(customFont, "-", Vector2{555, 360}, 20, 0, DARKGRAY);

        // Кнопка "+" для конечной станции
        if (GuiButton({630, 350, 100, 30}, ""))
        {
            newRouteEnd = (newRouteEnd < 5) ? newRouteEnd + 1 : 5;
        }
        DrawTextEx(customFont, "+", Vector2{663, 360}, 20, 0, DARKGRAY);

        DrawTextEx(customFont, "Скорость будет вычислена автоматически", Vector2{520, 400}, 20, 0, GRAY);

        // Кнопка "Далее"
        if (GuiButton({520, 480, 140, 40}, ""))
        {
            if (!newRouteName.empty() && newRouteStart != newRouteEnd)
            {
                dialogState = 1;
                selectedStops.clear();
                selectedStops.push_back(newRouteStart);
                selectedStops.push_back(newRouteEnd);
            }
        }
        DrawTextEx(customFont, "Далее", Vector2{550, 495}, 20, 0, DARKGRAY);

        // Кнопка "Закрыть"
        if (GuiButton({680, 480, 140, 40}, ""))
        {
            showAddTrainDialog = false;
            newRouteName = "";
            dialogState = 0;
        }
        DrawTextEx(customFont, "Закрыть", Vector2{705, 495}, 20, 0, DARKGRAY);
    }
    else if (dialogState == 1)
    {
        DrawTextEx(customFont, "Выберите промежуточные станции", Vector2{520, 120}, 18, 0, DARKGRAY);
        DrawTextEx(customFont, "(Начальная и конечная уже выбраны)", Vector2{520, 145}, 12, 0, Color{128, 128, 128, 255});

        std::vector<Station> stations = network.getStations();
        float yPos = 175;

        for (std::vector<Station>::iterator it = stations.begin(); it != stations.end(); ++it)
        {
            if (it->id == newRouteStart || it->id == newRouteEnd)
            {
                continue;
            }

            bool isSelected = std::find(selectedStops.begin(), selectedStops.end(),
                                        it->id) != selectedStops.end();

            std::string label = it->name + (isSelected ? " ✓" : "");
            Color bgColor = isSelected ? Color{100, 200, 100, 255} : Color{220, 220, 220, 255};

            DrawRectangle(520, yPos, 250, 30, bgColor);
            DrawRectangleLines(520, yPos, 250, 30, DARKGRAY);
            DrawTextEx(customFont, label.c_str(), Vector2{525, yPos + 8}, 12, 0, DARKGRAY);

            if (GuiButton({780, yPos, 30, 30}, ""))
            {
                std::vector<int>::iterator findIt = std::find(selectedStops.begin(), selectedStops.end(), it->id);
                if (findIt != selectedStops.end())
                {
                    selectedStops.erase(findIt);
                }
                else
                {
                    selectedStops.push_back(it->id);
                }
            }
            DrawTextEx(customFont, isSelected ? "-" : "+", Vector2{790, yPos + 8}, 16, 0, DARKGRAY);

            yPos += 40;
        }

        // Кнопка "Создать"
        if (GuiButton({520, 560, 140, 40}, ""))
        {
            if (selectedStops.size() >= 2)
            {
                network.addRoute(newRouteName, newRouteStart, newRouteEnd);
                Route *newRoute = network.getRoute(network.getRoutes().size() - 1);

                if (newRoute)
                {
                    std::sort(selectedStops.begin(), selectedStops.end());

                    Time currentTime(6, 0);
                    for (size_t i = 0; i < selectedStops.size(); ++i)
                    {
                        int dwellTime = (i == selectedStops.size() - 1) ? 0 : 5;
                        Time departureTime = currentTime + Time(0, dwellTime);

                        newRoute->addStop(StationStop(
                            selectedStops[i],
                            currentTime,
                            departureTime,
                            dwellTime));

                        currentTime = departureTime + Time(0, 90);
                    }

                    // Добавляем поезд со скоростью -1 для автоматического расчёта
                    network.addTrain(network.getRoutes().size() - 1, -1);

                    showAddTrainDialog = false;
                    newRouteName = "";
                    dialogState = 0;
                }
            }
        }
        DrawTextEx(customFont, "Создать", Vector2{540, 575}, 20, 0, DARKGRAY);

        // Кнопка "Назад"
        if (GuiButton({680, 560, 140, 40}, ""))
        {
            dialogState = 0;
        }
        DrawTextEx(customFont, "Назад", Vector2{700, 575}, 20, 0, DARKGRAY);

        // Кнопка "Закрыть"
        if (GuiButton({840, 560, 140, 40}, ""))
        {
            showAddTrainDialog = false;
            newRouteName = "";
            dialogState = 0;
        }
        DrawTextEx(customFont, "Закрыть", Vector2{860, 575}, 20, 0, DARKGRAY);
    }
}

int main()
{
    InitWindow(1800, 1000, "Симулятор движения поездов");
    SetTargetFPS(60);

    Font customFont = LoadRussianFontStatic("..\\static\\AdwaitaSans-Regular.ttf", 30);
    Texture2D stationTexture = LoadTexture("..\\static\\station.png");

    float RouteForSchedule = 0;

    initializeNetwork();

    while (!WindowShouldClose())
    {
        if (showAddTrainDialog && dialogState == 0)
        {
            int key = GetCharPressed();
            if (key > 0)
            {
                newRouteName += (char)key;
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !newRouteName.empty())
            {
                newRouteName.pop_back();
            }
        }

        if (isRunning)
        {
            network.simulateStep();
            printNewEvents();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (network.getSimulationTime() == Time(19, 0))
                isRunning = false;
        }

        BeginDrawing();
        ClearBackground(Color{240, 240, 245, 255});

        drawControlPanel(customFont);
        drawRailwayNetwork(customFont, stationTexture);
        drawScheduleWindow(customFont, RouteForSchedule);
        drawStatisticsWindow(customFont);

        // Кнопка СТАРТ/СТОП
        Rectangle startBtn = {10, 300, 230, 40};
        if (GuiButton(startBtn, ""))
        {
            isRunning = !isRunning;
        }
        DrawTextEx(customFont, isRunning ? "СТОП" : "СТАРТ", Vector2{65, 310}, 20, 0, DARKGRAY);

        // Кнопка "Добавить свой маршрут"
        Rectangle addTrainBtn = {10, 350, 230, 40};
        if (GuiButton(addTrainBtn, ""))
        {
            showAddTrainDialog = true;
            newRouteName = "";
            dialogState = 0;
            selectedStops.clear();
            newRouteStart = 0;
            newRouteEnd = 5;
        }
        DrawTextEx(customFont, "Добавить маршрут", Vector2{28, 360}, 20, 0, DARKGRAY);

        // Кнопка "Перезапустить эксперимент"
        Rectangle resetBtn = {10, 400, 230, 40};
        if (GuiButton(resetBtn, ""))
        {
            isRunning = false;
            showAddTrainDialog = false;
            lastEventCount = 0;
            network = RailwayNetwork();
            initializeNetwork();
        }
        DrawTextEx(customFont, "Перезапустить", Vector2{33, 410}, 20, 0, DARKGRAY);

        drawAddTrainDialog(customFont);

        EndDrawing();
    }

    UnloadFont(customFont);
    network.printStatistics();
    CloseWindow();

    return 0;
}