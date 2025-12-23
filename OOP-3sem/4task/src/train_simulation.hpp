#ifndef TRAIN_SIMULATION_HPP
#define TRAIN_SIMULATION_HPP

#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>

enum class EventType {
    TRAIN_DEPARTURE,
    TRAIN_ARRIVAL,
    TRAIN_DELAYED,
    TRACK_ACCIDENT,
    TRACK_REPAIR,
    NONE
};

enum class TrainStatus {
    SCHEDULED,
    IN_TRANSIT,
    AT_STATION,
    DELAYED,
    ACCIDENT,
    COMPLETED
};

class Time {
private:
    void normalize() {
        if (minutes >= 60) {
            hours += minutes / 60;
            minutes %= 60;
        }
        if (hours >= 24) {
            hours %= 24;
        }
    }

public:
    int hours;
    int minutes;

    Time(int h = 0, int m = 0) : hours(h), minutes(m) {
        normalize();
    }

    static Time fromMinutes(int totalMinutes) {
        return Time(totalMinutes / 60, totalMinutes % 60);
    }

    int toMinutes() {
        return hours * 60 + minutes;
    }

    std::string toString() {
        std::string h = (hours < 10) ? "0" + std::to_string(hours) : std::to_string(hours);
        std::string m = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
        return h + ":" + m;
    }

    Time operator+(Time other) {
        return Time(hours + other.hours, minutes + other.minutes);
    }

    Time operator-(Time other) {
        int thisMin = toMinutes();
        int otherMin = other.toMinutes();
        return fromMinutes(thisMin - otherMin);
    }

    bool operator<(Time other) {
        return toMinutes() < other.toMinutes();
    }

    bool operator<=(Time other) {
        return toMinutes() <= other.toMinutes();
    }

    bool operator>(Time other) {
        return toMinutes() > other.toMinutes();
    }

    bool operator>=(Time other) {
        return toMinutes() >= other.toMinutes();
    }

    bool operator==(Time other) {
        return toMinutes() == other.toMinutes();
    }

    bool operator!=(Time other) {
        return toMinutes() != other.toMinutes();
    }
};

class Station {
public:
    int id;
    std::string name;
    float positionKm;

    Station(int id = -1, std::string name = "", float pos = 0)
        : id(id), name(name), positionKm(pos) {}
};

class StationStop {
public:
    int stationId;
    Time arrivalTime;
    Time departureTime;
    int dwellMinutes;

    StationStop(int sid = -1, Time arr = Time(), Time dep = Time(), int dwell = 0)
        : stationId(sid), arrivalTime(arr), departureTime(dep), dwellMinutes(dwell) {}
};

class Route {
public:
    int id;
    std::string name;
    int startStationId;
    int endStationId;
    std::vector<StationStop> stops;
    float totalDistanceKm;

    Route(int id = -1, std::string name = "", int start = -1, int end = -1)
        : id(id), name(name), startStationId(start), endStationId(end), totalDistanceKm(0) {}

    void addStop(StationStop stop) {
        stops.push_back(stop);
    }

    int getStopIndex(int stationId) {
        for (size_t i = 0; i < stops.size(); ++i) {
            if (stops[i].stationId == stationId) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    float calculateTotalDistance(std::vector<Station>& stations) {
        if (stops.empty()) return 0;
        float totalDist = 0;
        for (size_t i = 1; i < stops.size(); ++i) {
            Station* prev = nullptr;
            Station* curr = nullptr;
            for (auto& s : stations) {
                if (s.id == stops[i - 1].stationId) prev = &s;
                if (s.id == stops[i].stationId) curr = &s;
            }
            if (prev && curr) {
                totalDist += std::abs(curr->positionKm - prev->positionKm);
            }
        }
        return totalDist;
    }

    int calculateTravelTimeMinutes() {
        if (stops.empty()) return 0;
        int totalTime = 0;
        for (size_t i = 1; i < stops.size(); ++i) {
            int prevDepMinutes = stops[i - 1].departureTime.toMinutes();
            int currArrMinutes = stops[i].arrivalTime.toMinutes();
            totalTime += (currArrMinutes - prevDepMinutes);
        }
        return totalTime;
    }

    float calculateAverageSpeed(std::vector<Station>& stations) {
        int travelTimeMinutes = calculateTravelTimeMinutes();
        if (travelTimeMinutes == 0) return 70.0f;

        float distance = calculateTotalDistance(stations);
        if (distance == 0) return 70.0f;

        float travelTimeHours = travelTimeMinutes / 60.0f;
        float avgSpeed = distance / travelTimeHours;

        if (avgSpeed < 30.0f) avgSpeed = 30.0f;
        if (avgSpeed > 150.0f) avgSpeed = 150.0f;

        return avgSpeed;
    }
};

class Train {
public:
    int id;
    int routeId;
    float speed;
    TrainStatus status;
    float currentPositionKm;
    int currentStationIndex;
    int delayMinutes;
    int accidentDurationMinutes;
    float distanceToNextStation;
    float distanceTraveledToNextStation;

    Train(int id = -1, int routeId = -1, float speed = 70)
        : id(id), routeId(routeId), speed(speed), status(TrainStatus::SCHEDULED),
          currentPositionKm(0), currentStationIndex(-1),
          delayMinutes(0), accidentDurationMinutes(0),
          distanceToNextStation(0), distanceTraveledToNextStation(0) {}

    TrainStatus getStatus() {
        return status;
    }

    void setStatus(TrainStatus s) {
        status = s;
    }

    int getDelayMinutes() {
        return delayMinutes;
    }

    void addDelay(int minutes) {
        delayMinutes += minutes;
    }

    void clearDelay() {
        delayMinutes = 0;
    }

    int getAccidentDuration() {
        return accidentDurationMinutes;
    }

    void setAccidentDuration(int minutes) {
        accidentDurationMinutes = minutes;
    }

    float getCurrentPosition() {
        return currentPositionKm;
    }
};

class SimulationEvent {
public:
    EventType type;
    Time occurredAt;
    int trainId;
    int stationId;
    int routeId;
    std::string description;

    SimulationEvent(EventType t = EventType::NONE, Time time = Time(),
                   int tid = -1, int sid = -1, int rid = -1,
                   std::string desc = "")
        : type(t), occurredAt(time), trainId(tid), stationId(sid),
          routeId(rid), description(desc) {}
};

class RandomGenerator {
private:
    std::mt19937 gen;

public:
    RandomGenerator() : gen(std::random_device{}()) {}

    int getRandomInt(int min, int max) {
        if (min >= max) return min;
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    float getRandomFloat(float min, float max) {
        if (min >= max) return min;
        std::uniform_real_distribution<> dis(min, max);
        return static_cast<float>(dis(gen));
    }

    bool getRandomBool(float probability) {
        return getRandomFloat(0, 1) < probability;
    }
};

class SimulationStatistics {
public:
    int totalEvents;
    int totalDelays;
    int totalAccidents;
    int totalDelayMinutes;
    int totalAccidentMinutes;
    std::vector<SimulationEvent> eventLog;

    SimulationStatistics()
        : totalEvents(0), totalDelays(0), totalAccidents(0),
          totalDelayMinutes(0), totalAccidentMinutes(0) {}

    void recordEvent(SimulationEvent event) {
        eventLog.push_back(event);
        ++totalEvents;
        if (event.type == EventType::TRAIN_DELAYED) {
            ++totalDelays;
        } else if (event.type == EventType::TRACK_ACCIDENT) {
            ++totalAccidents;
        }
    }

    void recordDelay(int minutes) {
        totalDelayMinutes += minutes;
    }

    void recordAccident(int minutes) {
        totalAccidentMinutes += minutes;
    }

    float getAverageDelay() {
        if (totalDelays == 0) return 0;
        return static_cast<float>(totalDelayMinutes) / static_cast<float>(totalDelays);
    }
};

class RailwayNetwork {
private:
    std::vector<Station> stations;
    std::vector<Route> routes;
    std::vector<Train> trains;
    SimulationStatistics statistics;
    RandomGenerator randomGenerator;
    Time simulationTime;
    int simulationStep;
    int nextTrainId;
    int nextRouteId;
    bool isInitialized;
    int accidentIntervalMinHours;
    int accidentIntervalMaxHours;
    int accidentDurationMinMinutes;
    int accidentDurationMaxMinutes;
    int delayMinMinutes;
    int delayMaxMinutes;
    float delayProbability;
    float accidentProbability;

    void initializeTrains() {
        for (Train& train : trains) {
            Route* route = getRoute(train.routeId);
            if (route && !route->stops.empty()) {
                train.currentStationIndex = 0;
                Station* firstStation = getStation(route->stops[0].stationId);
                if (firstStation) {
                    train.currentPositionKm = firstStation->positionKm;
                    train.setStatus(TrainStatus::IN_TRANSIT);

                    if (route->stops.size() > 1) {
                        Station* nextStation = getStation(route->stops[1].stationId);
                        if (nextStation) {
                            train.distanceToNextStation =
                                std::abs(nextStation->positionKm - firstStation->positionKm);
                            train.distanceTraveledToNextStation = 0;
                        }
                    }

                    statistics.recordEvent(
                        SimulationEvent(EventType::TRAIN_DEPARTURE, route->stops[0].departureTime,
                                      train.id, route->stops[0].stationId, train.routeId,
                                      "Train " + std::to_string(train.id) + " departed")
                    );
                }
            }
        }
    }

    // СИНХРОНИЗАЦИЯ ВРЕМЕНИ ПОЕЗДА С ГЛОБАЛЬНЫМ ВРЕМЕНЕМ
    void syncTrainTimeWithSimulation(Train& train, Route* route) {
        if (!route || route->stops.empty()) return;

        // Дополнительная задержка из-за аварий и задержек
        int extraDelayMinutes = train.delayMinutes + train.accidentDurationMinutes;
        Time effectiveScheduleTime = simulationTime + Time(0, extraDelayMinutes);

        // Определяем текущую остановку и позицию поезда в графике
        for (int i = 0; i < static_cast<int>(route->stops.size()); ++i) {
            Time arrivalTime = route->stops[i].arrivalTime + Time(0, extraDelayMinutes);
            Time departureTime = route->stops[i].departureTime + Time(0, extraDelayMinutes);

            if (effectiveScheduleTime >= arrivalTime && effectiveScheduleTime <= departureTime) {
                // Поезд на станции
                train.currentStationIndex = i;
                Station* station = getStation(route->stops[i].stationId);
                if (station) {
                    train.currentPositionKm = station->positionKm;
                }
                train.setStatus(TrainStatus::AT_STATION);
                return;
            }

            if (effectiveScheduleTime > departureTime &&
                i < static_cast<int>(route->stops.size()) - 1) {
                // Проверяем, не приехал ли он на следующую станцию
                Time nextArrivalTime = route->stops[i + 1].arrivalTime + Time(0, extraDelayMinutes);

                if (effectiveScheduleTime >= nextArrivalTime) {
                    continue; // Переходим к следующей итерации
                }

                // Поезд в пути между станциями i и i+1
                train.currentStationIndex = i;
                Station* currStation = getStation(route->stops[i].stationId);
                Station* nextStation = getStation(route->stops[i + 1].stationId);

                if (currStation && nextStation) {
                    Time prevDepartureTime = route->stops[i].departureTime + Time(0, extraDelayMinutes);
                    int minutesSinceDeparture = effectiveScheduleTime.toMinutes() - prevDepartureTime.toMinutes();

                    if (minutesSinceDeparture > 0) {
                        float moveDistance = (train.speed / 60.0f) * minutesSinceDeparture;
                        float segmentDistance = std::abs(nextStation->positionKm - currStation->positionKm);

                        if (moveDistance >= segmentDistance) {
                            // Прибываем на станцию
                            train.currentPositionKm = nextStation->positionKm;
                            train.currentStationIndex = i + 1;
                            train.setStatus(TrainStatus::AT_STATION);
                        } else {
                            // В пути
                            train.currentPositionKm = currStation->positionKm + moveDistance;
                            train.setStatus(TrainStatus::IN_TRANSIT);
                            train.distanceTraveledToNextStation = moveDistance;
                            train.distanceToNextStation = segmentDistance;
                        }
                    } else {
                        train.currentPositionKm = currStation->positionKm;
                        train.setStatus(TrainStatus::AT_STATION);
                    }
                }
                return;
            }
        }

        // Если поезд прошел последнюю остановку
        if (effectiveScheduleTime > route->stops.back().departureTime) {
            train.setStatus(TrainStatus::COMPLETED);
            Station* lastStation = getStation(route->stops.back().stationId);
            if (lastStation) {
                train.currentPositionKm = lastStation->positionKm;
            }
            train.currentStationIndex = static_cast<int>(route->stops.size()) - 1;
        }
    }

    void updateTrain(Train& train) {
        if (train.getStatus() == TrainStatus::ACCIDENT) {
            train.accidentDurationMinutes -= simulationStep;
            if (train.accidentDurationMinutes <= 0) {
                train.setStatus(TrainStatus::IN_TRANSIT);
                train.accidentDurationMinutes = 0;
                statistics.recordEvent(
                    SimulationEvent(EventType::TRACK_REPAIR, simulationTime, train.id, -1,
                                  train.routeId,
                                  "Track repair completed for train " + std::to_string(train.id))
                );
            }
            return;
        }

        if (train.getStatus() == TrainStatus::COMPLETED) {
            return;
        }

        Route* route = getRoute(train.routeId);
        if (!route || route->stops.empty()) return;

        // СИНХРОНИЗИРУЕМ ВРЕМЯ И ПОЗИЦИЮ ПОЕЗДА С ГЛОБАЛЬНЫМ ВРЕМЕНЕМ
        syncTrainTimeWithSimulation(train, route);
    }

    void checkForAccidents() {
        for (auto& train : trains) {
            if (train.getStatus() == TrainStatus::IN_TRANSIT) {
                int probability = randomGenerator.getRandomInt(1, 100);
                if (probability <= accidentProbability) { 
                    int duration = randomGenerator.getRandomInt(accidentDurationMinMinutes,
                                                               accidentDurationMaxMinutes);
                    train.setStatus(TrainStatus::ACCIDENT);
                    train.setAccidentDuration(duration);
                    statistics.recordEvent(
                        SimulationEvent(EventType::TRACK_ACCIDENT, simulationTime, train.id, -1,
                                      train.routeId,
                                      "Accident on train " + std::to_string(train.id) +
                                      " (duration: " + std::to_string(duration) + " min)")
                    );
                    statistics.recordAccident(duration);
                }
            }
        }
    }

    void checkForDelays() {
        for (auto& train : trains) {
            if (train.getStatus() == TrainStatus::AT_STATION) {
                if (randomGenerator.getRandomBool(delayProbability)) {
                    int delay = randomGenerator.getRandomInt(delayMinMinutes, delayMaxMinutes);
                    train.addDelay(delay);
                    train.setStatus(TrainStatus::DELAYED);
                    statistics.recordEvent(
                        SimulationEvent(EventType::TRAIN_DELAYED, simulationTime, train.id, -1,
                                      train.routeId,
                                      "Train " + std::to_string(train.id) +
                                      " delayed for " + std::to_string(delay) + " minutes")
                    );
                    statistics.recordDelay(delay);
                }
            }
        }
    }

    void updateSchedule() {
        for (auto& train : trains) {
            if (train.getDelayMinutes() > 0) {
                Route* route = getRoute(train.routeId);
                if (route) {
                    for (size_t i = train.currentStationIndex + 1; i < route->stops.size(); ++i) {
                        route->stops[i].arrivalTime =
                            route->stops[i].arrivalTime + Time(0, train.getDelayMinutes());
                        route->stops[i].departureTime =
                            route->stops[i].departureTime + Time(0, train.getDelayMinutes());
                    }
                }
                train.clearDelay();
            }
        }
    }

public:
    RailwayNetwork(float delayProb = 30, float accidentProb = 20)
        : simulationTime(6, 0), simulationStep(15), nextTrainId(0), nextRouteId(0),
          isInitialized(false),
          accidentIntervalMinHours(2), accidentIntervalMaxHours(15),
          accidentDurationMinMinutes(30), accidentDurationMaxMinutes(120),
          delayMinMinutes(1), delayMaxMinutes(5), delayProbability(delayProb / 100), 
          accidentProbability(accidentProb) {}

    void addStation(std::string name, float positionKm) {
        Station station(static_cast<int>(stations.size()), name, positionKm);
        stations.push_back(station);
    }

    Station* getStation(int id) {
        for (auto& station : stations) {
            if (station.id == id) {
                return &station;
            }
        }
        return nullptr;
    }

    std::vector<Station>& getStations() {
        return stations;
    }

    void addRoute(std::string name, int startId, int endId) {
        Route route(nextRouteId++, name, startId, endId);
        routes.push_back(route);
    }

    Route* getRoute(int id) {
        for (auto& route : routes) {
            if (route.id == id) {
                return &route;
            }
        }
        return nullptr;
    }

    std::vector<Route>& getRoutes() {
        return routes;
    }

    void addTrain(int routeId, float speed = -1.0f) {
        Route* route = getRoute(routeId);
        if (route && speed < 0) {
            // Вычисляем скорость автоматически
            speed = route->calculateAverageSpeed(stations);
        }
        Train train(nextTrainId++, routeId, speed);
        trains.push_back(train);
    }

    Train* getTrain(int id) {
        for (auto& train : trains) {
            if (train.id == id) {
                return &train;
            }
        }
        return nullptr;
    }

    std::vector<Train>& getTrains() {
        return trains;
    }

    void setSimulationStartTime(Time time) {
        simulationTime = time;
    }

    void setSimulationStep(int minutes) {
        if (minutes == 15 || minutes == 30) {
            simulationStep = minutes;
        }
    }

    void setAccidentInterval(int minHours, int maxHours) {
        accidentIntervalMinHours = minHours;
        accidentIntervalMaxHours = maxHours;
    }

    void setDelayParameters(int minMin, int maxMin, float probability) {
        delayMinMinutes = minMin;
        delayMaxMinutes = maxMin;
        delayProbability = probability;
    }

    Time getSimulationTime() {
        return simulationTime;
    }

    void simulateStep() {
        if (!isInitialized) {
            initializeTrains();
            isInitialized = true;
        }

        simulationTime = simulationTime + Time(0, simulationStep);

        for (auto& train : trains) {
            updateTrain(train);
        }

        checkForAccidents();
        checkForDelays();
        updateSchedule();
    }

public:
    SimulationStatistics& getStatistics() {
        return statistics;
    }

    std::vector<SimulationEvent> getEventLog() {
        return statistics.eventLog;
    }

    void printStatistics() {
        std::cout << "\n=== SIMULATION STATISTICS ===" << std::endl;
        std::cout << "Current Time: " << simulationTime.toString() << std::endl;
        std::cout << "Total Events: " << statistics.totalEvents << std::endl;
        std::cout << "Total Delays: " << statistics.totalDelays << std::endl;
        std::cout << "Total Accidents: " << statistics.totalAccidents << std::endl;
        std::cout << "Total Delay Time: " << statistics.totalDelayMinutes << " minutes" << std::endl;
        std::cout << "Total Accident Time: " << statistics.totalAccidentMinutes << " minutes" << std::endl;
        std::cout << "Average Delay: " << statistics.getAverageDelay() << " minutes" << std::endl;
        std::cout << "Trains Count: " << trains.size() << std::endl;
        std::cout << "Routes Count: " << routes.size() << std::endl;
        std::cout << "Stations Count: " << stations.size() << std::endl;

        std::cout << "\n=== TRAIN SPEEDS ===" << std::endl;
        for (const auto& train : trains) {
            Route* route = getRoute(train.routeId);
            if (route) {
                std::cout << "Train " << train.id << " (Route: " << route->name
                         << ") - Speed: " << train.speed << " km/h" << std::endl;
            }
        }
    }
};

#endif // TRAIN_SIMULATION_HPP