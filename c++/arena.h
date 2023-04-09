#pragma once
#include <vector>
#include <stdexcept>
#include <cmath>
#include "constants.h"
#include "food_source.h"

class Arena {
public:
    Arena(int arena_size);

    double get_pheromone(int x, int y);
    void update_pheromones(const FoodSource& food_source);
    void deposit_pheromone(int x, int y, double strength);
    int get_arena_size() const;
    double get_total_pheromones() const;
    void diffuse_pheromones();

private:
    int arena_size;
    std::vector<std::vector<double>> pheromone_grid;
};
