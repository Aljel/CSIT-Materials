#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "train_simulation.hpp"
#include <vector>

class DetailedStatistics {
public:
    struct TrainStats { int trainId, routeId, totalDelayMinutes, delayCount; };
    struct RouteStats {
        int routeId;
        std::string routeName;
        int totalTrainsScheduled, totalTrainsCompleted;
        float completionRatePercent;
    };

private:
    std::vector<TrainStats> trainStatistics;
    std::vector<RouteStats> routeStatistics;

public:
    void analyzeNetwork(RailwayNetwork network) {
        trainStatistics.clear();
        auto trains = network.getTrains();
        for (auto t : trains) {
            TrainStats s{t.id, t.routeId, t.getDelayMinutes(), 0};
            trainStatistics.push_back(s);
        }
    }

    float getOverallCompletionRate() {
        return trainStatistics.empty() ? 0.0f : 100.0f;
    }
};

#endif
