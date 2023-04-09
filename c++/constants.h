#pragma once
#include "system_headers.h"

namespace Constants {
const int WINDOW_SIZE = 50;
const int ARENA_SIZE = 200;
const int NEST_X = ARENA_SIZE / 2;
const int NEST_Y = ARENA_SIZE / 2;
const int NUM_ANTS = 100;
const int FOOD_AMOUNT = 100;
const int ANT_RADIUS = 3;
const int FOOD_RADIUS = 10;
const int NEST_RADIUS = 15;
const int FRAME_RATE = 60;
const int MAX_STEPS_BEFORE_ANGLE_CHANGE = 100;
const double MAX_ANGLE_CHANGE = M_PI / 2;
const double MOVE_DISTANCE = 2.0;
const double PHEROMONE_DISSIPATION_RATE = 0.99;

/*
SIGMOID_SCALE: Controls the steepness of the sigmoid function. A higher value
will make the function steeper, meaning the ant's decision to follow the
pheromone will be more strongly influenced by the pheromone level. You can
adjust this value to control the ants' sensitivity to pheromone levels.
SIGMOID_MIDPOINT: Determines the value of the pheromone level at which the ant
has a 50% chance of following the trail. You can adjust this value to control
the ants' threshold for following pheromones.
*/
constexpr double SIGMOID_SCALE = 100.9;
constexpr double SIGMOID_MIDPOINT = 0.1;
} // namespace Constants
