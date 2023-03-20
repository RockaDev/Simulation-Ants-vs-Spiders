#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include "SFML/Graphics.hpp"
#include "ability.h"
#include "food.h"
#include "colony.h"
#include "ant.h"
#include "spider.h"


int main() {
    Mine mineObj;
    std::vector<sf::CircleShape> mineShapes;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Ants");

    sf::CircleShape mine(20.f);
    std::vector<Spider> spiders;
    std::vector<Ant> ants;
    std::vector<Food>foods;

    
    float spiderColonySize = 25.f;
    float spiderColonyNeededResources = 10;
    int spiderColonyPopulation = 5;
    SpiderColony spidercolony(spiderColonyPopulation, spiderColonyNeededResources, spiderColonySize,window);

    float antColonySize = 25.f;
    float antColonyNeededResources = 10;
    int antColonyPopulation = 5;
    AntColony antcolony(antColonyPopulation, antColonyNeededResources, antColonySize, window);

    for (int food = 0; food < 1; food++)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f position(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        float size = 15.f;
        float proteine = 1.f;
        foods.emplace_back(position, size, proteine);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(90.f, 100.f);
    std::uniform_real_distribution<float> velDist(3.f, 6.f);
    std::uniform_real_distribution<float> speedDist(0.03f, 0.04f);
    std::uniform_real_distribution<float> persDist(10.f, 30.f);
    std::uniform_real_distribution<float> venomDist(1.f, 2.f);
    std::uniform_real_distribution<float> aggrDist(1.f, 2.f);
    std::uniform_real_distribution<float> rotDist(0.f, 360.f);

    sf::Clock spawnTimer;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text spiderCounter;
    spiderCounter.setFont(font);
    spiderCounter.setString("Spiders: 0");
    spiderCounter.setCharacterSize(24);
    spiderCounter.setPosition(50,50);
    spiderCounter.setFillColor(sf::Color::Blue);
    int spidersCreated = 0;

    sf::Text antCounter;
    antCounter.setFont(font);
    antCounter.setString("Ants: 0");
    antCounter.setCharacterSize(24);
    antCounter.setPosition(50, 80);
    antCounter.setFillColor(sf::Color::Magenta);
    int antsCreated = 0;

    int nextSpiderID = 0;

    for (int spiderCount = 0; spiderCount < 40; spiderCount++)
    {
        sf::Vector2f position(50.f,500.f);
        sf::Vector2f velocity(velDist(gen), velDist(gen));
        float health = 70.f;
        float damage = 10.f;
        float speed = speedDist(gen);
        float persistence = persDist(gen);
        float venom_level = venomDist(gen);
        float agressivity_level = aggrDist(gen);
        bool hungry = true;
        float maxSpeed = 0.5f;
        float rotation = rotDist(gen);

        spiders.emplace_back(nextSpiderID, health, damage, speed, velocity, agressivity_level, persistence, venom_level, position, maxSpeed, hungry, rotation);
        spidersCreated++;
        nextSpiderID++;
        spiderCounter.setString("Spiders: " + std::to_string(spidersCreated));
    }

    for (int antsCount = 0; antsCount < 60; antsCount++)
    {
        sf::Vector2f position(700.f, 50.f);
        sf::Vector2f velocity(velDist(gen), velDist(gen));
        float health = 100.f;
        float damage = 10.f;
        float speed = speedDist(gen);
        float persistence = persDist(gen);
        float venom_level = venomDist(gen);
        float agressivity_level = aggrDist(gen);
        bool hungry = true;
        const float maxSpeed = 0.5f;
        float rotation = rotDist(gen);

        ants.emplace_back(health, damage, speed, velocity, agressivity_level, persistence, venom_level, position, maxSpeed, hungry, rotation);
        antsCreated++;
        antCounter.setString("Ants: " + std::to_string(antsCreated));
    }


    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                foodVisible = true;
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                foodVisible = false;
            }

        }

        // Clear the window
        window.clear(sf::Color::Black);

        for (auto& spider : spiders)
        {
            spider.update(spiders,window,foods[0],mineObj);
            spider.draw(window);
            spider.MaxVision(window,spider);
            spider.drawtrail(window);
        }

        for (auto& ant : ants)
        {
            ant.update(ants, window,spiders, foods[0],mineObj);
            ant.draw(window);
            ant.MaxVision(window, ant);
            ant.drawtrail(window);
        }

        for (auto& food : foods)
        {
            if (foodVisible)
            {
                sf::Vector2f foodPosition = food.getPositionMouse(window);
                food.draw(window, foodPosition);
            }
            
        }
        spidercolony.drawColony(window, sf::Vector2f(50.f,500.f));
        antcolony.drawColony(window, sf::Vector2f(700.f, 50.f));

        window.draw(spiderCounter);
        window.draw(antCounter);
        // Display the window
        window.display();
    }

    return 0;
}
