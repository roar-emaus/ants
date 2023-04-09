#include "ant.h"
#include "arena.h"
#include "constants.h"
#include "food_source.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;

int main() {
  srand(static_cast<unsigned>(time(0)));

  sf::RenderWindow window(
      sf::VideoMode(Constants::WINDOW_SIZE, Constants::WINDOW_SIZE),
      "Ant Simulation");

  window.setFramerateLimit(Constants::FRAME_RATE);

  Arena arena(Constants::ARENA_SIZE);
  std::shared_ptr<Arena> arena_ptr = std::make_shared<Arena>(arena);
  FoodSource food_source(20, 20, Constants::FOOD_AMOUNT,
                         Constants::FOOD_RADIUS);
  std::shared_ptr<FoodSource> food_source_ptr =
      std::make_shared<FoodSource>(food_source);
  std::vector<Ant> ants;

  for (int i = 0; i < Constants::NUM_ANTS; i++) {
    ants.emplace_back(Ant(Constants::NEST_X, Constants::NEST_Y,
                          Constants::NEST_X, Constants::NEST_Y, food_source_ptr,
                          arena_ptr));
  }

  sf::CircleShape nest(Constants::NEST_RADIUS);
  nest.setFillColor(sf::Color(139, 69, 19));
  nest.setPosition(Constants::NEST_X - Constants::NEST_RADIUS,
                   Constants::NEST_Y - Constants::NEST_RADIUS);

  sf::CircleShape food(Constants::FOOD_RADIUS);
  food.setFillColor(sf::Color(0, 255, 0));
  food.setPosition(food_source.get_x() - Constants::FOOD_RADIUS,
                   food_source.get_y() - Constants::FOOD_RADIUS);

  std::vector<sf::CircleShape> ant_shapes(
      Constants::NUM_ANTS, sf::CircleShape(Constants::ANT_RADIUS));
  for (auto& ant_shape : ant_shapes) {
    ant_shape.setFillColor(sf::Color::Red);
  }

  std::vector<double> avg_distances;
  std::ofstream position_file("ant_positions.csv");

  int timestep = 0;
  position_file << "Timestep, AntIndex, X, Y, Cos, Sin, DirectionAngle, Job, "
                   "PheromoneLevel"
                << std::endl;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();

    bool food_source_empty = food_source_ptr->is_food_source_empty();
    for (std::size_t i = 0; i < ants.size(); ++i) {
      auto& ant = ants[i];
      auto& ant_shape = ant_shapes[i];
      if (food_source_empty) {
        ant.clear_food_source();
      }
      ant.move();
      double ant_x = ant.get_x() - Constants::ANT_RADIUS;
      double ant_y = ant.get_y() - Constants::ANT_RADIUS;
      ant_shape.setPosition(ant_x, ant_y);
      position_file << timestep << ", " << i << ", " << ant_x << ", " << ant_y
                    << ", " << ant.get_x_cos() << ", " << ant.get_y_sin()
                    << ", " << ant.get_angle() << ", "
                    << ant.ant_job_to_string() << ", "
                    << arena_ptr->get_pheromone(static_cast<int>(ant_x),
                                           static_cast<int>(ant_y))
                    << std::endl;
      window.draw(ant_shape);
    }
    position_file << std::endl;
    timestep++;
    
    std::cout << arena_ptr->get_total_pheromones() << std::endl;
    window.draw(nest);
    if (!food_source_ptr->is_food_source_empty()) {
      window.draw(food);
    }
    arena_ptr->update_pheromones();
    window.display();
  }

  return 0;
}
