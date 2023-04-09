#pragma once
#include "system_headers.h"

namespace Constants {
const int WINDOW_SIZE = 100;
const int ARENA_SIZE = 500;
const int NEST_X = ARENA_SIZE / 2;
const int NEST_Y = ARENA_SIZE / 2;
const int NUM_ANTS = 1000;
const int FOOD_AMOUNT = 100;
const int ANT_RADIUS = 3;
const int FOOD_RADIUS = 20;
const int NEST_RADIUS = 15;
const int FRAME_RATE = 240;
const int MAX_STEPS_BEFORE_ANGLE_CHANGE = 100;
const double MAX_ANGLE_CHANGE = M_PI / 2;
const double MOVE_DISTANCE = 1.0;
const double PHEROMONE_DISSIPATION_RATE = 0.001;
constexpr double INCREASED_PHEROMONE_DISSIPATION_RATE = 0.5;

/*
SIGMOID_SCALE: Controls the steepness of the sigmoid function. A higher value
will make the function steeper, meaning the ant's decision to follow the
pheromone will be more strongly influenced by the pheromone level. You can
adjust this value to control the ants' sensitivity to pheromone levels.
SIGMOID_MIDPOINT: Determines the value of the pheromone level at which the ant
has a 50% chance of following the trail. You can adjust this value to control
the ants' threshold for following pheromones.
*/
constexpr double SIGMOID_SCALE = 0.5;
constexpr double SIGMOID_MIDPOINT = 5.0;
} // namespace Constants
