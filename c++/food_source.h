#pragma once

class FoodSource {
public:
    FoodSource(double x, double y, int amount, double radius);

    bool has_food() const;
    int take_food(int amount);
    double get_x() const;
    double get_y() const;
    double get_radius() const;

private:
    double x;
    double y;
    double radius;
    int remaining_food;
};
