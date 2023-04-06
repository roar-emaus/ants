#pragma once

const int WINDOW_SIZE = 100;
const int ARENA_SIZE = 500;
const int NEST_X = ARENA_SIZE / 2;
const int NEST_Y = ARENA_SIZE / 2;
const int NUM_ANTS = 100;
const int FOOD_AMOUNT = 100;
const int ANT_RADIUS = 3;
const int FOOD_RADIUS = 20;
const int NEST_RADIUS = 8;
const int FRAME_RATE = 120;
const int MAX_STEPS_BEFORE_ANGLE_CHANGE = 50;
const double MAX_ANGLE_CHANGE = M_PI / 4;
const double MOVE_DISTANCE = 2.0;
// Logistic distribution parameters for the follow threshold
/*
Location parameter (FOLLOW_THRESHOLD_LOCATION): This parameter defines the center of the distribution. In our case, it represents the mean follow threshold value. A higher value would mean that, on average, ants would need a higher pheromone level to follow the trail. Conversely, a lower value would mean that ants would be more likely to follow the trail even at lower pheromone levels
*/
const double FOLLOW_THRESHOLD_LOCATION = 0.3;
/*
 * Scale parameter (FOLLOW_THRESHOLD_SCALE): This parameter defines the spread or shape of the distribution. A larger scale value would result in a flatter distribution, meaning that the follow threshold would vary more widely among the ants. A smaller scale value would result in a more peaked distribution, causing the follow threshold to be more consistent among the ants.
 */
const double FOLLOW_THRESHOLD_SCALE = 0.1;
