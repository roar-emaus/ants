#include "ant.h"
#include "constants.h"

std::default_random_engine Ant::generator(std::random_device{}());
std::uniform_real_distribution<double> Ant::angle_distribution(0.0, 2 * M_PI);
std::uniform_real_distribution<double> Ant::exit_distribution(0.0, 1.0);
std::uniform_real_distribution<double> Ant::logistic_distribution(0.0, 1.0);

Ant::Ant(double start_x, double start_y, int arena_size, double nest_x, double nest_y, std::shared_ptr<FoodSource> food_source)
    : x(start_x), y(start_y), arena_size(arena_size), nest_x(nest_x), nest_y(nest_y), food_source(food_source), job(Status::IN_NEST) {
    direction_angle = angle_distribution(generator);
}
void Ant::move(Arena& arena) {
    if (job == Status::IN_NEST) {
        if (exit_nest()) {
            random_move();
            job = Status::FORAGING;
        }
    } else if (job == Status::FORAGING) {
        double pheromone_level = arena.get_pheromone(static_cast<int>(x), static_cast<int>(y));
        double follow_probability = pheromone_level / (1 + pheromone_level);
        double random_follow_threshold = logistic_sample(Constants::FOLLOW_THRESHOLD_LOCATION, Constants::FOLLOW_THRESHOLD_SCALE);
        if (has_found_food()) {
            int food_taken = food_source->take_food(food_amount);
            if (food_taken > 0) {
                carrying_food = true;
                job = Status::RETURN;
            }
        } else if (follow_probability > random_follow_threshold){
            int x_int = static_cast<int>(x);
            int y_int = static_cast<int>(y);
            if (x_int >= 0 && x_int < arena_size && y_int >= 0 && y_int < arena_size) {
                follow_pheromone(arena);
            }

        } else {
            steps_since_angle_change++;
            if (steps_since_angle_change >= (rand() % Constants::MAX_STEPS_BEFORE_ANGLE_CHANGE) + 1) {
                steps_since_angle_change = 0;
                random_move();
            } else {
                move_in_current_direction();
            }
        }
    } else if (job == Status::RETURN) {
        if (carrying_food) {
            go_towards_nest();
            double strength = 0;
            if (food_source){
                strength = 5 / distance(x, y, food_source->get_x(), food_source->get_y());
            }
            arena.deposit_pheromone(static_cast<int>(x), static_cast<int>(y), strength);
        }

        if (reached_nest()) {
            carrying_food = false;
            job = Status::FORAGING;
        }
    } 
}

void Ant::move_in_current_direction() {
    double new_x = x + Constants::MOVE_DISTANCE * cos(direction_angle);
    double new_y = y + Constants::MOVE_DISTANCE * sin(direction_angle);
    x = std::clamp(new_x, 0.0, Constants::ARENA_SIZE - 1.0);
    y = std::clamp(new_y, 0.0, Constants::ARENA_SIZE - 1.0);
}

bool Ant::has_found_food() {
    if (food_source){
        double distance_to_food = distance(x, y, food_source->get_x(), food_source->get_y());
        // Check if the ant is within the food radius.
        return distance_to_food <= food_source->get_radius();
    }
    return false;
}

bool Ant::reached_nest() {
    return distance(x, y, nest_x, nest_y) <= nest_radius;
}

double Ant::get_x() const {
    return x;
}

double Ant::get_y() const {
    return y;
}

bool Ant::exit_nest() {
    return exit_distribution(generator) < 0.01;
}

void Ant::random_move() {
    direction_angle = angle_distribution(generator);
}

void Ant::go_towards_nest() {
    double angle_to_nest = std::atan2(nest_y - y, nest_x - x);
    double new_x = x + cos(angle_to_nest);
    double new_y = y + sin(angle_to_nest);

    if (new_x >= 0 && new_x < arena_size && new_y >= 0 && new_y < arena_size) {
        x = new_x;
        y = new_y;
    }
}


void Ant::follow_pheromone(Arena& arena) {
    int x_int = static_cast<int>(x);
    int y_int = static_cast<int>(y);
    double max_pheromone = -1;
    int new_x = x_int;
    int new_y = y_int;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {
                int nx = x_int + i;
                int ny = y_int + j;

                if (nx >= 0 && nx < arena_size && ny >= 0 && ny < arena_size) {
                    double pheromone = arena.get_pheromone(nx, ny);
                    if (pheromone > max_pheromone) {
                        max_pheromone = pheromone;
                        new_x = nx;
                        new_y = ny;
                    }
                }
            }
        }
    }

    // Move towards the neighboring cell with the highest pheromone level
    if (new_x >= 0 && new_x < arena_size && new_y >= 0 && new_y < arena_size) {
        double angle_to_next_cell = std::atan2(new_y - y, new_x - x);
        double random_angle = random_normal(0, 0.1); // Adjust the second parameter for more or less randomness
        double combined_angle = angle_to_next_cell + random_angle;

        double proposed_x = x + Constants::MOVE_DISTANCE * cos(combined_angle);
        double proposed_y = y + Constants::MOVE_DISTANCE * sin(combined_angle);

        int clamped_x = std::clamp(static_cast<int>(proposed_x), 0, arena_size - 1);
        int clamped_y = std::clamp(static_cast<int>(proposed_y), 0, arena_size - 1);

        x = clamped_x;
        y = clamped_y;
    }
}

double Ant::logistic_sample(double location, double scale) {
    double u = logistic_distribution(generator);
    return location + scale * std::log(u / (1 - u));
}

double Ant::random_normal(double mean, double stddev) {
    double u1 = (rand() % 1000) / 1000.0;
    double u2 = (rand() % 1000) / 1000.0;
    double z = std::sqrt(-2.0 * std::log(u1)) * std::cos(2 * M_PI * u2);
    return mean + stddev * z;
}

bool Ant::is_carrying_food() const {
    return carrying_food;
}

void Ant::clear_food_source() {
    food_source.reset();
}

double Ant::distance(double x1, double y1, double x2, double y2) {
    return std::hypot(x1 - x2, y1 - y2);
}
