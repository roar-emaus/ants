#pragma once
#include "arena.h"
#include "constants.h"
#include "food_source.h"
#include "system_headers.h"

enum class Status { IN_NEST, FORAGING, RETURN };

class Ant {
public:
  Ant(double start_x, double start_y, int arena_size, double nest_x,
      double nest_y, std::shared_ptr<FoodSource> food_source);

  void move(Arena& arena);
  [[nodiscard]] auto get_x() const -> double;
  [[nodiscard]] auto get_y() const -> double;
  [[nodiscard]] auto get_angle() const -> double;
  [[nodiscard]] auto get_x_cos() const -> double;
  [[nodiscard]] auto get_y_sin() const -> double;
  void move_in_current_direction();
  auto has_found_food() -> bool;
  auto reached_nest() -> bool;
  [[nodiscard]] auto is_carrying_food() const -> bool;
  void clear_food_source();
  auto ant_job_to_string() -> std::string;

private:
  double x;
  double y;
  double x_cos = 0.0;
  double y_sin = 0.0;
  double direction_angle;
  int arena_size;
  double nest_x;
  double nest_y;
  double nest_radius = 8.0;
  std::shared_ptr<FoodSource> food_source;
  Status job;
  bool carrying_food = false;
  const int food_amount = 1;
  int steps_since_angle_change = 0;

  auto exit_nest() -> bool;
  void set_angle_towards_nest();
  void follow_pheromone(Arena& arena);
  auto should_follow_pheromone(Arena& arena) -> bool;
  auto random_normal(double mean, double stddev) -> double;
  auto distance(double x1, double y1, double x2, double y2) -> double;
  void update_position();
  void update_angle();

  static std::default_random_engine generator;
  static std::uniform_real_distribution<double> angle_distribution;
  static std::uniform_real_distribution<double> normal_distribution;
  static std::exponential_distribution<double> exponential_distribution;
  static std::uniform_real_distribution<double> random_angle_distribution;
};
