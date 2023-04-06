#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "arena.h"
#include "food_source.h"
#include "ant.h"
#include "constants.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Ant Simulation");
    window.setFramerateLimit(FRAME_RATE);

    Arena arena(ARENA_SIZE);
    FoodSource food_source(20, 20, FOOD_AMOUNT, FOOD_RADIUS);
    std::shared_ptr<FoodSource> food_source_ptr = std::make_shared<FoodSource>(food_source);
    std::vector<Ant> ants;

    for (int i = 0; i < NUM_ANTS; i++) {
        ants.emplace_back(Ant(NEST_X, NEST_Y, ARENA_SIZE, NEST_X, NEST_Y, food_source_ptr));
    }

    sf::CircleShape nest(NEST_RADIUS);
    nest.setFillColor(sf::Color(139, 69, 19));
    nest.setPosition(NEST_X - NEST_RADIUS, NEST_Y - NEST_RADIUS);

    sf::CircleShape food(FOOD_RADIUS);
    food.setFillColor(sf::Color(0, 255, 0));
    food.setPosition(food_source.get_x() - FOOD_RADIUS, food_source.get_y() - FOOD_RADIUS);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        for (auto& ant : ants) {
            ant.move(arena);
            sf::CircleShape ant_shape(ANT_RADIUS);
            ant_shape.setFillColor(sf::Color::Red);
            ant_shape.setPosition(ant.get_x() - ANT_RADIUS, ant.get_y() - ANT_RADIUS);
            window.draw(ant_shape);
        }

        window.draw(nest);
        window.draw(food);

        window.display();
    }

    return 0;
}
