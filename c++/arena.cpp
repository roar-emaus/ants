#include "arena.h"

Arena::Arena(int grid_size)
    : grid_size(grid_size), pheromone_grid(grid_size, std::vector<double>(grid_size, 0.0)) {}

double Arena::get_pheromone(int x, int y) {
    return pheromone_grid[x][y];
}

void Arena::update_pheromones() {
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            // Dissipate pheromone over time
            pheromone_grid[i][j] *= 0.99;
        }
    }
}

void Arena::deposit_pheromone(int x, int y, double strength) {
    pheromone_grid[x][y] += strength;
}

int Arena::get_grid_size() const {
    return grid_size;
}
