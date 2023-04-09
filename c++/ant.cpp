#include "ant.h"
#include "constants.h"
#include <iostream>

std::default_random_engine Ant::generator(std::random_device{}());
std::uniform_real_distribution<double> Ant::angle_distribution(0.0, 2 * M_PI);
std::uniform_real_distribution<double> Ant::random_angle_distribution(-0.1, 0.1);
std::uniform_real_distribution<double> Ant::normal_distribution(0.0, 1.0);
std::exponential_distribution<double> Ant::exponential_distribution(1.0 / Constants::MAX_STEPS_BEFORE_ANGLE_CHANGE);


Ant::Ant(
        double start_x, 
        double start_y, 
        int arena_size, 
        double nest_x, 
        double nest_y, 
        std::shared_ptr<FoodSource> food_source
        ): 
    x(start_x), 
    y(start_y), 
    arena_size(arena_size), 
    nest_x(nest_x), 
    nest_y(nest_y), 
    food_source(food_source), 
    job(Status::IN_NEST),
    direction_angle(angle_distribution(generator)){};


void Ant::move(Arena& arena) {
    if (job == Status::IN_NEST) {
        if (exit_nest()) {
            job = Status::FORAGING;
            update_position();
        }
    } else if (job == Status::FORAGING) {
        if (has_found_food()) {
            int food_taken = food_source->take_food(food_amount);
            if (food_taken > 0) {
                carrying_food = true;
                job = Status::RETURN;
            }
        } else if (should_follow_pheromone(arena)){
            follow_pheromone(arena);

        } 
        update_position();
    } else if (job == Status::RETURN) {
        if (carrying_food) {
            set_angle_towards_nest();
            double strength = 0;
            if (food_source){
                strength = 10000 / distance(x, y, food_source->get_x(), food_source->get_y());
            }
            update_position();
            arena.deposit_pheromone(static_cast<int>(x), static_cast<int>(y), strength);
        }

        if (reached_nest()) {
            carrying_food = false;
            job = Status::FORAGING;
        }
    } 
}

void Ant::update_position(){
    steps_since_angle_change++;
    if (steps_since_angle_change >= exponential_distribution(generator)){
        steps_since_angle_change = 0;
        direction_angle = angle_distribution(generator);
        update_angle();
    }
    
    double new_x = x + Constants::MOVE_DISTANCE * x_cos;
    double new_y = y + Constants::MOVE_DISTANCE * y_sin;
    x = std::clamp(new_x, 0.0, Constants::ARENA_SIZE - 1.0);
    y = std::clamp(new_y, 0.0, Constants::ARENA_SIZE - 1.0);
    
}

void Ant::update_angle(){
    x_cos = cos(direction_angle);
    y_sin = sin(direction_angle);
}

bool Ant::should_follow_pheromone(Arena& arena){
    double pheromone_level = arena.get_pheromone(static_cast<int>(x), static_cast<int>(y));
    if (pheromone_level < 0.1){
        return false;
    }
    double follow_probability = 1.0 / (1.0 + std::exp(-Constants::SIGMOID_SCALE * (pheromone_level - Constants::SIGMOID_MIDPOINT)));
    double random_value = normal_distribution(generator);
    return random_value < follow_probability;
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
double Ant::get_x_cos() const {
    return x_cos;
}

double Ant::get_y_sin() const {
    return y_sin;
}

double Ant::get_angle() const {
    return direction_angle;
}

bool Ant::exit_nest() {
    return normal_distribution(generator) < 0.0001;
}

void Ant::set_angle_towards_nest() {
    direction_angle = std::atan2(nest_y - y, nest_x - x);
    update_angle();
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
    double angle_to_next_cell = std::atan2(new_y - y, new_x - x);
    double random_angle = random_angle_distribution(generator);
    direction_angle = angle_to_next_cell + random_angle;
    update_angle();
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
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

std::string Ant::ant_job_to_string() {
    switch (job) {
        case Status::FORAGING:
            return "Foraging";
        case Status::RETURN:
            return "Returning to nest";
        case Status::IN_NEST:
            return "Idle";
        default:
            return "Unknown";
    }
}

