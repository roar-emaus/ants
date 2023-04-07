#pragma once
#include <cmath>
#include <algorithm>

class FoodSource {
public:
    FoodSource(double x, double y, int amount, double radius);

    bool has_food() const;
    int take_food(int amount);
    double get_x() const;
    double get_y() const;
    double get_radius() const;
    bool is_food_source_empty() const;
    int get_food_remaining() const;

private:
    double x;
    double y;
    double radius;
    int food_amount;
    bool is_empty = false;
};
