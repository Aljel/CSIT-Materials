#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "train_simulation.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

// 2 аргумента: шанс задержки и шанс проишествия в процентах
RailwayNetwork network(80, 5);
bool isRunning = false;
int lastEventCount = 0;

bool showAddTrainDialog = false;
int dialogState = 0;

std::string newRouteName = "";
int newRouteStart = 0;
int newRouteEnd = 5;
std::vector<int> selectedStops;

Font LoadRussianFontStatic(const char *fontPath, int fontSize) {
    int charsCount = 0;
    int* chars = LoadCodepoints(
        "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        "0123456789"
        ".,!?-+()[]{}<:;/\\\"'`~@#$%^&*=_| "
        "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm",
        &charsCount
    );

    Font font = LoadFontEx(fontPath, fontSize, chars, charsCount);
    UnloadCodepoints(chars);
    return font;
}

void initializeNetwork() {
    network.addStation("Москва", 0);
    network.addStation("Tula", 100);
    network.addStation("Oryol", 200);
    network.addStation("Kursk", 300);
    network.addStation("Voronezh", 400);
    network.addStation("Tambov", 500);

    network.addRoute("Первый путь", 0, 4);
    network.addRoute("Вторйо путь", 1, 4);
    network.addRoute("Третий путь", 2, 5);
    network.addRoute("Четвертый путь", 0, 3);
    network.addRoute("Пятый путь", 1, 2);

    Route* r0 = network.getRoute(0);
    if (r0) {
        r0->addStop(StationStop(0, Time(6, 0), Time(6, 5), 5));
        r0->addStop(StationStop(1, Time(7, 0), Time(7, 5), 5));
        r0->addStop(StationStop(2, Time(8, 0), Time(8, 5), 5));
        r0->addStop(StationStop(3, Time(9, 0), Time(9, 5), 5));
        r0->addStop(StationStop(4, Time(10, 0), Time(10, 5), 5));
        r0->addStop(StationStop(5, Time(11, 0), Time(11, 3), 3));
    }

    Route* r1 = network.getRoute(1);
    if (r1) {
        r1->addStop(StationStop(1, Time(6, 30), Time(6, 35), 5));
        r1->addStop(StationStop(2, Time(7, 30), Time(7, 35), 5));
        r1->addStop(StationStop(3, Time(8, 30), Time(8, 35), 5));
        r1->addStop(StationStop(4, Time(9, 30), Time(9, 30), 0));
    }

    Route* r2 = network.getRoute(2);
    if (r2) {
        r2->addStop(StationStop(2, Time(7, 0), Time(7, 5), 5));
        r2->addStop(StationStop(3, Time(8, 0), Time(8, 5), 5));
        r2->addStop(StationStop(4, Time(9, 0), Time(9, 5), 5));
        r2->addStop(StationStop(5, Time(10, 0), Time(10, 0), 0));
    }

    Route* r3 = network.getRoute(3);
    if (r3) {
        r3->addStop(StationStop(0, Time(8, 0), Time(8, 5), 5));
        r3->addStop(StationStop(1, Time(8, 50), Time(8, 55), 5));
        r3->addStop(StationStop(2, Time(9, 40), Time(9, 45), 5));
        r3->addStop(StationStop(3, Time(10, 30), Time(10, 30), 0));
    }

    Route* r4 = network.getRoute(4);
    if (r4) {
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
    network.setDelayParameters(1, 5, 0.5);
}

void printNewEvents() {
    std::vector<SimulationEvent> events = network.getEventLog();
    if (events.size() > lastEventCount) {
        for (size_t i = lastEventCount; i < events.size(); ++i) {
            std::cout << "[" << events[i].occurredAt.toString() << "] "
                      << events[i].description << "\n";
        }
    }
    lastEventCount = events.size();
}

void drawRailwayNetwork(Font customFont, Texture2D stationTexture) {
    std::vector<Station> stations = network.getStations();
    std::vector<Train> trains = network.getTrains();

    DrawTextEx(customFont, "Симуляция движеняи поездов!", Vector2{280, 20}, 20, 0, DARKGRAY);
    DrawTextEx(customFont, ("Время: " + network.getSimulationTime().toString()).c_str(), Vector2{280, 50}, 16, 0, GRAY);

    DrawRectangle(300, 100, 1100, 400, Color{240, 240, 240, 255});

    float y = 300;
    DrawLine(300, y, 1350, y, Color{100, 100, 100, 255});

    for (std::vector<Station>::iterator it = stations.begin(); it != stations.end(); ++it) {
        float x = 300 + it->positionKm * 2;
        DrawTextureEx(stationTexture, {x - 50, y - 150}, 0, 0.4f, WHITE);
        DrawTextEx(customFont, it->name.c_str(), Vector2{x, y + 25}, 20, 0, DARKGRAY);
    }

    for (std::vector<Train>::iterator it = trains.begin(); it != trains.end(); ++it) {
        float x = 300 + it->getCurrentPosition() * 2;
        float ty = y + (it->id) * 30 + 80;
        Color col;

        switch (it->getStatus()) {
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

void drawScheduleWindow(Font customFont, float& RouteForSchedule) {
    DrawRectangle(800, 600, 400, 350, Color{245, 245, 245, 255});
    DrawRectangleLines(800, 600, 400, 350, DARKGRAY);
    DrawTextEx(customFont, "Расписание", Vector2{810, 610}, 30, 0, DARKGRAY);

    Route* r = network.getRoute(RouteForSchedule);
    if (r) {
        DrawTextEx(customFont, ("Route: " + r->name).c_str(), Vector2{810, 650}, 20, 0, DARKGRAY);
        float yPos = 680;

        std::vector<StationStop> stops = r->stops;
        for (std::vector<StationStop>::iterator it = stops.begin(); it != stops.end(); ++it) {
            Station* s = network.getStation(it->stationId);
            if (s) {
                std::string text = s->name + " " + it->arrivalTime.toString() + "-" + it->departureTime.toString();
                DrawTextEx(customFont, text.c_str(), Vector2{810, yPos}, 20, 0, GRAY);
                yPos += 30;
            }
        }
    }

    Rectangle routeSlider = {870, 900, 230, 20};
    int routeCount = network.getRoutes().size();
    if (routeCount > 0) {
        GuiSlider(routeSlider, "0", std::to_string(routeCount - 1).c_str(), &RouteForSchedule, 0, (routeCount - 1));
    }
}

void drawStatisticsWindow(Font customFont) {
    DrawRectangle(300, 600, 400, 350, Color{245, 245, 245, 255});
    DrawRectangleLines(300, 600, 400, 350, DARKGRAY);
    DrawTextEx(customFont, "Statistics", Vector2{310, 610}, 30, 0, DARKGRAY);

    SimulationStatistics stats = network.getStatistics();
    float yPos = 660;

    DrawTextEx(customFont, ("Time: " + network.getSimulationTime().toString()).c_str(), Vector2{310, yPos}, 20, 0, GRAY);
    yPos += 30;

    DrawTextEx(customFont, ("Events: " + std::to_string(stats.totalEvents)).c_str(), Vector2{310, yPos}, 20, 0, GRAY);
    yPos += 30;

    DrawTextEx(customFont, ("Delays: " + std::to_string(stats.totalDelays)).c_str(), Vector2{310, yPos}, 20, 0, ORANGE);
    yPos += 30;

    DrawTextEx(customFont, ("Accidents: " + std::to_string(stats.totalAccidents)).c_str(), Vector2{310, yPos}, 20, 0, RED);
    yPos += 30;

    DrawTextEx(customFont, ("Delay time: " + std::to_string(stats.totalDelayMinutes) + " min").c_str(),
             Vector2{310, yPos}, 20, 0, GRAY);
}

void drawControlPanel(Font customFont) {
    DrawRectangle(0, 0, 250, 1000, Color{200, 200, 200, 255});
    DrawRectangleLines(0, 0, 250, 1000, DARKGRAY);
    DrawTextEx(customFont, "Управление", Vector2{10, 10}, 16, 0, DARKGRAY);

    std::vector<Train> trains = network.getTrains();
    int delayed = 0, accidents = 0, completed = 0;

    for (std::vector<Train>::iterator it = trains.begin(); it != trains.end(); ++it) {
        if (it->getStatus() == TrainStatus::DELAYED) delayed++;
        else if (it->getStatus() == TrainStatus::ACCIDENT) accidents++;
        else if (it->getStatus() == TrainStatus::COMPLETED) completed++;
    }

    DrawTextEx(customFont, ("Time: " + network.getSimulationTime().toString()).c_str(), Vector2{10, 50}, 11, 0, GRAY);
    DrawTextEx(customFont, ("Trains: " + std::to_string(trains.size())).c_str(), Vector2{10, 75}, 11, 0, GRAY);
    DrawTextEx(customFont, ("Completed: " + std::to_string(completed)).c_str(), Vector2{10, 100}, 11, 0, GREEN);
    DrawTextEx(customFont, ("Delayed: " + std::to_string(delayed)).c_str(), Vector2{10, 125}, 11, 0, ORANGE);
    DrawTextEx(customFont, ("Accidents: " + std::to_string(accidents)).c_str(), Vector2{10, 150}, 11, 0, RED);
}

void drawAddTrainDialog(Font customFont) {
    if (!showAddTrainDialog) return;

    DrawRectangle(0, 0, 1400, 800, Color{0, 0, 0, 100});
    DrawRectangle(350, 100, 700, 600, Color{245, 245, 245, 255});
    DrawRectangleLines(350, 100, 700, 600, DARKGRAY);

    if (dialogState == 0) {
        DrawTextEx(customFont, "Create New Train Route", Vector2{370, 120}, 20, 0, DARKGRAY);
        DrawTextEx(customFont, "Route Name:", Vector2{370, 170}, 14, 0, GRAY);
        DrawRectangle(370, 190, 300, 30, Color{255, 255, 255, 255});
        DrawRectangleLines(370, 190, 300, 30, DARKGRAY);
        DrawTextEx(customFont, newRouteName.c_str(), Vector2{375, 195}, 14, 0, DARKGRAY);
        DrawTextEx(customFont, "(Press BACKSPACE to clear)", Vector2{370, 225}, 10, 0, Color{128, 128, 128, 255});

        DrawTextEx(customFont, "Start Station: ", Vector2{370, 260}, 12, 0, GRAY);
        if (GuiButton({370, 280, 100, 30}, "-")) newRouteStart = (newRouteStart > 0) ? newRouteStart - 1 : 0;
        if (GuiButton({480, 280, 100, 30}, "+")) newRouteStart = (newRouteStart < 5) ? newRouteStart + 1 : 5;

        DrawTextEx(customFont, "End Station: ", Vector2{370, 330}, 12, 0, GRAY);
        if (GuiButton({370, 350, 100, 30}, "-")) newRouteEnd = (newRouteEnd > 0) ? newRouteEnd - 1 : 0;
        if (GuiButton({480, 350, 100, 30}, "+")) newRouteEnd = (newRouteEnd < 5) ? newRouteEnd + 1 : 5;

        DrawTextEx(customFont, "Note: Speed will be calculated", Vector2{370, 400}, 12, 0, GRAY);
        DrawTextEx(customFont, "automatically from route", Vector2{370, 420}, 12, 0, GRAY);
        DrawTextEx(customFont, "distance and schedule time", Vector2{370, 440}, 12, 0, GRAY);

        if (GuiButton({370, 480, 140, 40}, "Next")) {
            if (!newRouteName.empty() && newRouteStart != newRouteEnd) {
                dialogState = 1;
                selectedStops.clear();
                selectedStops.push_back(newRouteStart);
                selectedStops.push_back(newRouteEnd);
            }
        }

        if (GuiButton({530, 480, 140, 40}, "Cancel")) {
            showAddTrainDialog = false;
            newRouteName = "";
            dialogState = 0;
        }

    } else if (dialogState == 1) {
        DrawTextEx(customFont, "Select Intermediate Stops", Vector2{370, 120}, 18, 0, DARKGRAY);
        DrawTextEx(customFont, "(Start and End are already selected)", Vector2{370, 145}, 12, 0, Color{128, 128, 128, 255});

        std::vector<Station> stations = network.getStations();
        float yPos = 175;

        for (std::vector<Station>::iterator it = stations.begin(); it != stations.end(); ++it) {
            if (it->id == newRouteStart || it->id == newRouteEnd) {
                continue;
            }

            bool isSelected = std::find(selectedStops.begin(), selectedStops.end(),
                                       it->id) != selectedStops.end();
            std::string label = it->name + (isSelected ? " ✓" : "");
            Color bgColor = isSelected ? Color{100, 200, 100, 255} : Color{220, 220, 220, 255};

            DrawRectangle(370, yPos, 250, 30, bgColor);
            DrawRectangleLines(370, yPos, 250, 30, DARKGRAY);
            DrawTextEx(customFont, label.c_str(), Vector2{375, yPos + 8}, 12, 0, DARKGRAY);

            if (GuiButton({630, yPos, 30, 30}, isSelected ? "-" : "+")) {
                std::vector<int>::iterator findIt = std::find(selectedStops.begin(), selectedStops.end(), it->id);
                if (findIt != selectedStops.end()) {
                    selectedStops.erase(findIt);
                } else {
                    selectedStops.push_back(it->id);
                }
            }

            yPos += 40;
        }

        if (GuiButton({370, 560, 140, 40}, "Create")) {
            if (selectedStops.size() >= 2) {
                network.addRoute(newRouteName, newRouteStart, newRouteEnd);
                Route* newRoute = network.getRoute(network.getRoutes().size() - 1);
                if (newRoute) {
                    std::sort(selectedStops.begin(), selectedStops.end());
                    Time currentTime(6, 0);
                    for (size_t i = 0; i < selectedStops.size(); ++i) {
                        int dwellTime = (i == selectedStops.size() - 1) ? 0 : 5;
                        Time departureTime = currentTime + Time(0, dwellTime);
                        newRoute->addStop(StationStop(
                            selectedStops[i],
                            currentTime,
                            departureTime,
                            dwellTime
                        ));
                        currentTime = departureTime + Time(0, 10);
                    }
                }
                // Добавляем поезд со скоростью -1 для автоматического расчёта
                network.addTrain(network.getRoutes().size() - 1, -1);
                showAddTrainDialog = false;
                newRouteName = "";
                dialogState = 0;
            }
        }

        if (GuiButton({530, 560, 140, 40}, "Back")) {
            dialogState = 0;
        }

        if (GuiButton({690, 560, 140, 40}, "Cancel")) {
            showAddTrainDialog = false;
            newRouteName = "";
            dialogState = 0;
        }
    }
}

int main() {
    InitWindow(1800, 1000, "Симулятор движения поездов");
    SetTargetFPS(60);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);

    Font customFont = LoadRussianFontStatic("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 30);
    Texture2D stationTexture = LoadTexture("static/station.png");

    float RouteForSchedule = 0;

    initializeNetwork();

    while (!WindowShouldClose()) {
        if (showAddTrainDialog && dialogState == 0) {
            int key = GetCharPressed();
            if (key > 0) {
                newRouteName += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !newRouteName.empty()) {
                newRouteName.pop_back();
            }
        }

        if (isRunning) {
            network.simulateStep();
            printNewEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (network.getSimulationTime() == Time(0, 0))
                isRunning = false;
        }

        BeginDrawing();
        ClearBackground(Color{240, 240, 245, 255});

        drawControlPanel(customFont);
        drawRailwayNetwork(customFont, stationTexture);
        drawScheduleWindow(customFont, RouteForSchedule);
        drawStatisticsWindow(customFont);

        Rectangle startBtn = {10, 240, 230, 40};
        if (GuiButton(startBtn, isRunning ? "STOP" : "START")) {
            isRunning = !isRunning;
        }

        Rectangle addTrainBtn = {10, 600, 230, 40};
        if (GuiButton(addTrainBtn, "Add Custom Train")) {
            showAddTrainDialog = true;
            newRouteName = "";
            dialogState = 0;
            selectedStops.clear();
            newRouteStart = 0;
            newRouteEnd = 5;
        }

        Rectangle resetBtn = {10, 650, 230, 40};
        if (GuiButton(resetBtn, "Reset Experiment")) {
            isRunning = false;
            showAddTrainDialog = false;
            lastEventCount = 0;
            network = RailwayNetwork();
            initializeNetwork();
        }

        drawAddTrainDialog(customFont);

        EndDrawing();
    }

    network.printStatistics();
    CloseWindow();
    return 0;
}