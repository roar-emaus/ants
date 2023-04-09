#include "arena.h"

Arena::Arena(int arena_size)
    : arena_size(arena_size), pheromone_grid(arena_size, std::vector<double>(arena_size, 0.0)) {}

auto Arena::get_pheromone(int x, int y) -> double {
    if (x < 0 || x >= arena_size || y < 0 || y >= arena_size) {
        return 0.0;
        //throw std::out_of_range("Coordinates are outside the grid");
    }
    return pheromone_grid[x][y];
}

void Arena::update_pheromones(const FoodSource& food_source) {
        double const evaporation_rate = food_source.is_food_source_empty() ? Constants::INCREASED_PHEROMONE_DISSIPATION_RATE: Constants::PHEROMONE_DISSIPATION_RATE;
    for (int i = 0; i < arena_size; ++i) {
        for (int j = 0; j < arena_size; ++j) {
            // Dissipate pheromone over time
            pheromone_grid[i][j] *= evaporation_rate;
        }
    }
    //diffuse_pheromones();
}

void Arena::deposit_pheromone(int x, int y, double strength) {
    if (x < 0 || x >= arena_size || y < 0 || y >= arena_size) {
        throw std::out_of_range("Coordinates are outside the grid");
    }
    pheromone_grid[x][y] += strength;
}

auto Arena::get_arena_size() const -> int {
    return arena_size;
}

auto Arena::get_total_pheromones() const -> double {
    double total_pheromones = 0;
    for (int i = 0; i < arena_size; ++i) {
        for (int j = 0; j < arena_size; ++j) {
            total_pheromones += pheromone_grid[i][j];
        }
    }
    return total_pheromones;
}

void Arena::diffuse_pheromones() {
    std::vector<std::vector<double>> new_pheromone_grid(arena_size, std::vector<double>(arena_size, 0.0));

    for (int i = 0; i < arena_size; ++i) {
        for (int j = 0; j < arena_size; ++j) {
            double neighbor_sum = 0;
            int neighbor_count = 0;

            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) { continue; // Skip the current cell
}

                    int const x = i + dx;
                    int const y = j + dy;

                    if (x >= 0 && x < arena_size && y >= 0 && y < arena_size) {
                        neighbor_sum += pheromone_grid[x][y];
                        neighbor_count++;
                    }
                }
            }

            new_pheromone_grid[i][j] = neighbor_sum / neighbor_count;
        }
    }

    pheromone_grid.swap(new_pheromone_grid);
}
