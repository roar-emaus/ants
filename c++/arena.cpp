#include "arena.h"

Arena::Arena(int grid_size)
    : grid_size(grid_size), pheromone_grid(grid_size, std::vector<double>(grid_size, 0.0)) {}

double Arena::get_pheromone(int x, int y) {
    if (x < 0 || x >= grid_size || y < 0 || y >= grid_size) {
        throw std::out_of_range("Coordinates are outside the grid");
    }
    return pheromone_grid[x][y];
}

void Arena::update_pheromones() {
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            // Dissipate pheromone over time
            pheromone_grid[i][j] *= Constants::PHEROMONE_DISSIPATION_FACTOR;
        }
    }
}

void Arena::deposit_pheromone(int x, int y, double strength) {
    if (x < 0 || x >= grid_size || y < 0 || y >= grid_size) {
        throw std::out_of_range("Coordinates are outside the grid");
    }
    pheromone_grid[x][y] += strength;
}

int Arena::get_grid_size() const {
    return grid_size;
}
