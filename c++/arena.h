#pragma once
#include "constants.h"
#include "food_source.h"
#include "system_headers.h"

class Arena {
public:
  explicit Arena(int arena_size);

  auto get_pheromone(int x, int y) -> double;
  void update_pheromones();
  void deposit_pheromone(int x, int y, double strength);
  auto get_arena_size() const -> int;
  auto get_total_pheromones() const -> double;

private:
  int arena_size;
  std::vector<std::vector<double>> pheromone_grid;
};
