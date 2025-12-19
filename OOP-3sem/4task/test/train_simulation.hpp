#ifndef TRAIN_SIMULATION_HPP
#define TRAIN_SIMULATION_HPP

#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <cmath>
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

    int toMinutes() const {
        return hours * 60 + minutes;
    }

    std::string toString() const {
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

    bool operator<(Time other) const {
        return toMinutes() < other.toMinutes();
    }

    bool operator<=(Time other) const {
        return toMinutes() <= other.toMinutes();
    }

    bool operator>(Time other) const {
        return toMinutes() > other.toMinutes();
    }

    bool operator>=(Time other) const {
        return toMinutes() >= other.toMinutes();
    }

    bool operator==(Time other) const {
        return toMinutes() == other.toMinutes();
    }

    bool operator!=(Time other) const {
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
};

class Train {
public:
    int id;
    int routeId;
    float speed;
    TrainStatus status;
    Time currentTime;
    float currentPositionKm;
    int currentStationIndex;
    int delayMinutes;
    int accidentDurationMinutes;
    float distanceToNextStation;
    float distanceTraveledToNextStation;

    Train(int id = -1, int routeId = -1, float speed = 70)
        : id(id), routeId(routeId), speed(speed), status(TrainStatus::SCHEDULED),
          currentTime(6, 0), currentPositionKm(0), currentStationIndex(-1),
          delayMinutes(0), accidentDurationMinutes(0),
          distanceToNextStation(0), distanceTraveledToNextStation(0) {}

    TrainStatus getStatus() const {
        return status;
    }

    void setStatus(TrainStatus s) {
        status = s;
    }

    int getDelayMinutes() const {
        return delayMinutes;
    }

    void addDelay(int minutes) {
        delayMinutes += minutes;
    }

    void clearDelay() {
        delayMinutes = 0;
    }

    int getAccidentDuration() const {
        return accidentDurationMinutes;
    }

    void setAccidentDuration(int minutes) {
        accidentDurationMinutes = minutes;
    }

    float getCurrentPosition() const {
        return currentPositionKm;
    }

    Time getCurrentTime() const {
        return currentTime;
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

public:
    RailwayNetwork()
        : simulationTime(6, 0), simulationStep(15), nextTrainId(0), nextRouteId(0),
          isInitialized(false),
          accidentIntervalMinHours(2), accidentIntervalMaxHours(15),
          accidentDurationMinMinutes(30), accidentDurationMaxMinutes(120),
          delayMinMinutes(1), delayMaxMinutes(5), delayProbability(0.3f) {}

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

    const std::vector<Station>& getStations() const {
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

    void addTrain(int routeId, float speed = 70) {
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

    Time getSimulationTime() const {
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

private:
    void initializeTrains() {
        for (auto& train : trains) {
            Route* route = getRoute(train.routeId);
            if (route && !route->stops.empty()) {
                train.currentStationIndex = 0;
                Station* firstStation = getStation(route->stops[0].stationId);
                if (firstStation) {
                    train.currentPositionKm = firstStation->positionKm;
                    train.setStatus(TrainStatus::IN_TRANSIT);
                    train.currentTime = route->stops[0].departureTime;
                    
                    if (route->stops.size() > 1) {
                        Station* nextStation = getStation(route->stops[1].stationId);
                        if (nextStation) {
                            train.distanceToNextStation = 
                                std::abs(nextStation->positionKm - firstStation->positionKm);
                            train.distanceTraveledToNextStation = 0;
                        }
                    }

                    statistics.recordEvent(
                        SimulationEvent(EventType::TRAIN_DEPARTURE, train.currentTime, 
                                      train.id, route->stops[0].stationId, train.routeId,
                                      "Train " + std::to_string(train.id) + " departed")
                    );
                }
            }
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

        if (train.getStatus() == TrainStatus::IN_TRANSIT && train.currentStationIndex >= 0 &&
            train.currentStationIndex < static_cast<int>(route->stops.size()) - 1) {
            
            Station* currentStation = getStation(route->stops[train.currentStationIndex].stationId);
            Station* nextStation = getStation(route->stops[train.currentStationIndex + 1].stationId);

            if (currentStation && nextStation) {
                float moveDistance = (train.speed / 60) * simulationStep;
                train.distanceTraveledToNextStation += moveDistance;
                train.currentPositionKm += moveDistance;

                if (train.distanceTraveledToNextStation >= train.distanceToNextStation) {
                    train.currentPositionKm = nextStation->positionKm;
                    train.currentStationIndex++;
                    train.setStatus(TrainStatus::AT_STATION);
                    train.currentTime = route->stops[train.currentStationIndex].arrivalTime;
                    train.distanceTraveledToNextStation = 0;

                    statistics.recordEvent(
                        SimulationEvent(EventType::TRAIN_ARRIVAL, train.currentTime, train.id,
                                      route->stops[train.currentStationIndex].stationId, 
                                      train.routeId,
                                      "Train " + std::to_string(train.id) + " arrived at station " +
                                      std::to_string(route->stops[train.currentStationIndex].stationId))
                    );

                    if (train.currentStationIndex < static_cast<int>(route->stops.size()) - 1) {
                        Station* nextNextStation = 
                            getStation(route->stops[train.currentStationIndex + 1].stationId);
                        if (nextNextStation) {
                            train.distanceToNextStation = 
                                std::abs(nextNextStation->positionKm - nextStation->positionKm);
                        }
                    }

                    if (train.currentStationIndex >= static_cast<int>(route->stops.size()) - 1) {
                        train.setStatus(TrainStatus::COMPLETED);
                        statistics.recordEvent(
                            SimulationEvent(EventType::NONE, simulationTime, train.id, -1, 
                                          train.routeId,
                                          "Train " + std::to_string(train.id) + 
                                          " completed route")
                        );
                    }
                }
            }
        }

        if ((train.getStatus() == TrainStatus::AT_STATION || train.getStatus() == TrainStatus::DELAYED) && train.currentStationIndex >= 0 &&
            train.currentStationIndex < static_cast<int>(route->stops.size()) - 1) {

                train.currentTime = route->stops[train.currentStationIndex].departureTime;

                if (simulationTime >= train.currentTime) {
                    train.setStatus(TrainStatus::IN_TRANSIT);

                    statistics.recordEvent(
                            SimulationEvent(EventType::TRAIN_DEPARTURE, train.currentTime, 
                                        train.id, route->stops[train.currentStationIndex].stationId, train.routeId,
                                        "Train " + std::to_string(train.id) + " departed")
                        );
                }
            }
    }

    void checkForAccidents() {
        for (auto& train : trains) {
            if (train.getStatus() == TrainStatus::IN_TRANSIT) {
                int probability = randomGenerator.getRandomInt(1, 10000);
                if (probability < 1000) {  // 0.05% chance
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
    const SimulationStatistics& getStatistics() const {
        return statistics;
    }

    std::vector<SimulationEvent> getEventLog() const {
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
    }
};

#endif // TRAIN_SIMULATION_HPP
