#pragma once
#include <vector>

class Arena {
public:
    Arena(int grid_size);

    double get_pheromone(int x, int y);
    void update_pheromones();
    void deposit_pheromone(int x, int y, double strength);
    int get_grid_size() const;

private:
    int grid_size;
    std::vector<std::vector<double>> pheromone_grid;
};
