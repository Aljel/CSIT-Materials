#include "train_simulation.hpp"
#include <iostream>

void printNetworkInfo(RailwayNetwork network) {
    std::cout << "\n=== NETWORK ===" << std::endl;
    auto stations = network.getStations();
    std::cout << "Stations: " << stations.size() << std::endl;
    auto routes = network.getRoutes();
    std::cout << "Routes: " << routes.size() << std::endl;
    auto trains = network.getTrains();
    std::cout << "Trains: " << trains.size() << std::endl;
}

void printEventLog(RailwayNetwork network) {
    auto events = network.getEventLog();
    std::cout << "\n=== EVENTS (" << events.size() << ") ===" << std::endl;
    for (auto e : events) {
        std::cout << "[" << e.occurredAt.toString() << "] " << e.description << std::endl;
    }
}
