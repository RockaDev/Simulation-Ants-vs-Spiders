#pragma once


struct Mine
{
	std::vector<sf::CircleShape> mineShapes;
	void plantMine(
		sf::RenderWindow& window,
		sf::CircleShape& mine, // pass by reference
		bool beingAttacked,
		bool isAlive,
		sf::Vector2f position
	)
	{
		if (beingAttacked)
		{
			mine.setFillColor(sf::Color(255, 0, 0, 50));
			mine.setPosition(position.x - 12.5, position.y - 12.5);
			mine.setOrigin(3.f, 3.f);
			mineShapes.push_back(mine);
			window.draw(mine);
		}
	}

	std::vector<sf::CircleShape>& getMineShapes()
	{
		return mineShapes;
	}


	bool intersects(const sf::CircleShape& shape)
	{
		sf::Vector2f spiderCenter = shape.getPosition();
		return shape.getGlobalBounds().contains(spiderCenter);
	}


};

struct RegenerationBuoy
{
	std::vector<sf::CircleShape> regenBuoyShapes;
	void plantRegenBuoy(
		sf::RenderWindow& window,
		sf::CircleShape& regenbuoy, // pass by reference
		bool isHpLow,
		sf::Vector2f position
	)
	{
		if (isHpLow)
		{
			regenbuoy.setFillColor(sf::Color(124, 252, 0, 50));
			regenbuoy.setPosition(position.x - 12.5, position.y - 12.5);
			regenbuoy.setOrigin(3.f, 3.f);
			regenBuoyShapes.push_back(regenbuoy);
			window.draw(regenbuoy);
		}
	}

	const std::vector<sf::CircleShape>& getRegenBuoyShapes() const
	{
		return regenBuoyShapes;
	}
};