#include "food_source.h"

FoodSource::FoodSource(double x, double y, int amount, double radius)
    : x(x), y(y), food_amount(amount), radius(radius), is_empty(amount == 0) {}

bool FoodSource::has_food() const { return food_amount > 0; }

int FoodSource::take_food(int amount) {
  int taken = std::min(food_amount, amount);
  food_amount -= taken;

  if (food_amount == 0) {
    is_empty = true;
  }

  return taken;
}

double FoodSource::get_x() const { return x; }

double FoodSource::get_y() const { return y; }

double FoodSource::get_radius() const { return radius; }

bool FoodSource::is_food_source_empty() const { return is_empty; }

int FoodSource::get_food_remaining() const { return food_amount; }
