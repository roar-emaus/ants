#include "arena.h"

Arena::Arena(int arena_size)
    : arena_size(arena_size),
      pheromone_grid(arena_size, std::vector<double>(arena_size, 0.0)) {}

auto Arena::get_pheromone(int x, int y) -> double {
  if (x < 0 || x >= arena_size || y < 0 || y >= arena_size) {
    return 0.0;
    // throw std::out_of_range("Coordinates are outside the grid");
  }
  return pheromone_grid[x][y];
}

void Arena::update_pheromones() {
  for (int i = 0; i < arena_size; ++i) {
    for (int j = 0; j < arena_size; ++j) {
      // Dissipate pheromone over time
      if (pheromone_grid[i][j] < 0.1){
        pheromone_grid[i][j] = 0.0;
      } else {
        pheromone_grid[i][j] *= Constants::PHEROMONE_DISSIPATION_RATE;
      }
    }
  }
}

void Arena::deposit_pheromone(int x, int y, double strength) {
  if (x < 0 || x >= arena_size || y < 0 || y >= arena_size) {
    throw std::out_of_range("Coordinates are outside the grid");
  }

  pheromone_grid[x][y] += strength;

  // Add strength/2 to neighboring cells
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) {
        continue; // Skip the current cell
      }

      int const nx = x + dx;
      int const ny = y + dy;

      if (nx >= 0 && nx < arena_size && ny >= 0 && ny < arena_size) {
        pheromone_grid[nx][ny] += strength / 2;
      }
    }
  }

  // Add strength/4 to the cells further out
  for (int dx = -2; dx <= 2; dx += 4) {
    for (int dy = -2; dy <= 2; dy += 4) {
      int const nx = x + dx;
      int const ny = y + dy;

      if (nx >= 0 && nx < arena_size && ny >= 0 && ny < arena_size) {
        pheromone_grid[nx][ny] += strength / 4;
      }
    }
  }
}

auto Arena::get_arena_size() const -> int { return arena_size; }

auto Arena::get_total_pheromones() const -> double {
  double total_pheromones = 0;
  for (int i = 0; i < arena_size; ++i) {
    for (int j = 0; j < arena_size; ++j) {
      total_pheromones += pheromone_grid[i][j];
    }
  }
  return total_pheromones;
}
