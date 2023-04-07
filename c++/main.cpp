#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics.hpp>
#include "arena.h"
#include "food_source.h"
#include "ant.h"
#include "constants.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(Constants::WINDOW_SIZE, Constants::WINDOW_SIZE), "Ant Simulation");

    window.setFramerateLimit(Constants::FRAME_RATE);

    Arena arena(Constants::ARENA_SIZE);
    FoodSource food_source(20, 20, Constants::FOOD_AMOUNT, Constants::FOOD_RADIUS);
    std::shared_ptr<FoodSource> food_source_ptr = std::make_shared<FoodSource>(food_source);
    std::vector<Ant> ants;

    for (int i = 0; i < Constants::NUM_ANTS; i++) {
        ants.emplace_back(Ant(Constants::NEST_X, Constants::NEST_Y, Constants::ARENA_SIZE, Constants::NEST_X, Constants::NEST_Y, food_source_ptr));
    }

    sf::CircleShape nest(Constants::NEST_RADIUS);
    nest.setFillColor(sf::Color(139, 69, 19));
    nest.setPosition(Constants::NEST_X - Constants::NEST_RADIUS, Constants::NEST_Y - Constants::NEST_RADIUS);

    sf::CircleShape food(Constants::FOOD_RADIUS);
    food.setFillColor(sf::Color(0, 255, 0));
    food.setPosition(food_source.get_x() - Constants::FOOD_RADIUS, food_source.get_y() - Constants::FOOD_RADIUS);
    sf::Font font;
    if (!font.loadFromFile("DejaVuSansMono.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }
    
    sf::Text food_remaining_text;
    food_remaining_text.setFont(font);
    food_remaining_text.setCharacterSize(24);
    food_remaining_text.setFillColor(sf::Color::White);
    food_remaining_text.setPosition(Constants::WINDOW_SIZE, 10);

    std::vector<sf::CircleShape> ant_shapes(Constants::NUM_ANTS, sf::CircleShape(Constants::ANT_RADIUS));
    for (auto& ant_shape : ant_shapes) {
        ant_shape.setFillColor(sf::Color::Red);
    }
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
            if (food_source_empty){
                ant.clear_food_source();
            }
            ant.move(arena);
            double ant_x = ant.get_x() - Constants::ANT_RADIUS;
            double ant_y = ant.get_y() - Constants::ANT_RADIUS;
            ant_shape.setPosition(ant_x, ant_y);
            window.draw(ant_shape);
        }

        window.draw(nest);
        if (!food_source_ptr->is_food_source_empty()) {
            window.draw(food);
            std::stringstream ss;
            ss << "Food remaining: " << food_source_ptr->get_food_remaining();
            food_remaining_text.setString(ss.str());
            window.draw(food_remaining_text);
        }    
        arena.update_pheromones();

        window.display();
    }

    return 0;
}
