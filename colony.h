#pragma once

struct ColonyType
{
	int spiderColony = 0;
	int antColony = 1;
	int centipedeColony = 2;
};

class SpiderColony
{
private:
	int population;
	float needed_resources;
	float colonySize;
	ColonyType colony_type;
	sf::RenderWindow& window;
	
public:

	SpiderColony(int population, float needed_resources, float colonySize, sf::RenderWindow& window) :
		population(population),
		needed_resources(needed_resources),
		colonySize(colonySize),
		window(window)
	{
		
	}

	void spawn()
	{
		return;
	}

	void spawnerProcess(void)
	{
		return;
	}

	void drawColony(sf::RenderWindow& window, sf::Vector2f position)
	{
		sf::CircleShape spiderscolony(colonySize);
		spiderscolony.setFillColor(sf::Color::Blue);
		spiderscolony.setPosition(position);
		window.draw(spiderscolony);
	}
};

class AntColony
{
private:
	int population;
	float needed_resources;
	float colonySize;
	sf::RenderWindow& window;

public:

	AntColony(int population, float needed_resources, float colonySize, sf::RenderWindow& window) :
		population(population),
		needed_resources(needed_resources),
		colonySize(colonySize),
		window(window)
	{

	}

	void spawn()
	{
		return;
	}

	void spawnerProcess(void)
	{
		return;
	}

	void drawColony(sf::RenderWindow& window, sf::Vector2f position)
	{
		sf::CircleShape antscolony(colonySize);
		antscolony.setFillColor(sf::Color::Magenta);
		antscolony.setPosition(position);
		window.draw(antscolony);
	}
};