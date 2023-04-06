#pragma once
#include <random>
#include <memory>
#include <cmath>
#include <algorithm>
#include "constants.h"
#include "arena.h"
#include "food_source.h"

enum class Status { IN_NEST, FORAGING, RETURN };


class Ant {
public:
    Ant(double start_x, double start_y, int arena_size, double nest_x, double nest_y, std::shared_ptr<FoodSource> food_source);

    void move(Arena& arena);
    double get_x() const;
    double get_y() const;
    void move_in_current_direction();
    bool has_found_food();
    bool reached_nest();
    double distance_to_food();

private:
    double x;
    double y;
    double direction_angle;
    int arena_size;
    double nest_x;
    double nest_y;
    double nest_radius = 8.0;
    std::shared_ptr<FoodSource> food_source;
    Status job;
    bool carrying_food = false;
    const int food_amount = 1;
    double angle;
    int steps_since_angle_change = 0;

    bool exit_nest();
    void random_move();
    void go_towards_nest();
    void follow_pheromone(Arena& arena);
    double distance_to_nest();
    double logistic_sample(double location, double scale);
    double random_normal(double mean, double stddev);

    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> angle_distribution;
    static std::uniform_real_distribution<double> exit_distribution;
    static std::uniform_real_distribution<double> logistic_distribution;
};

