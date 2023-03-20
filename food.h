#pragma once

bool foodVisible = false;

class Food
{
private:
	sf::Vector2f position;
	float size;
	float proteine;

public:
	Food(sf::Vector2f position, float size, float proteine) :
		position(position),
		size(size),
		proteine(proteine)
	{

	}

	void draw(sf::RenderWindow& window, sf::Vector2f position)
	{
		sf::CircleShape Food(size);
		Food.setFillColor(sf::Color::Green);
		Food.setPosition(position);
		window.draw(Food);
	}

	sf::Vector2f getPositionMouse(const sf::RenderWindow& window) const
	{
		return window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	sf::Vector2f getPosition() const
	{
		return position;
	}
};
