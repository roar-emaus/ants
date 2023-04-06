#include "food_source.h"

FoodSource::FoodSource(double x, double y, int amount, double radius)
    : x(x), y(y), remaining_food(amount), radius(radius) {}

bool FoodSource::has_food() const {
    return remaining_food > 0;
}

int FoodSource::take_food(int amount) {
    if (remaining_food >= amount) {
        remaining_food -= amount;
        return amount;
    } else {
        int taken_food = remaining_food;
        remaining_food = 0;
        return taken_food;
    }
}

double FoodSource::get_x() const {
    return x;
}

double FoodSource::get_y() const {
    return y;
}

double FoodSource::get_radius() const {
    return radius;
}
