#pragma once

#include "spider.h"

struct Ant
{
private:
	float health;
	float damage;
	float speed;
	sf::Vector2f velocity;
	float agressivity_level;
	float persistence;
	float venom_level;
	sf::Vector2f position;
	bool hungry;
	sf::Clock clock;
	float maxSpeed;
	std::vector<sf::Vector2f> trailPositions;
	float rotation;
	sf::Vector2f position_;
	float maximumspeed = 0.5f;
	sf::Clock attackClock;
	sf::Clock radiationClock;
	Mine* pMine;

public:
	Ant(float health, float damage, float speed, sf::Vector2f velocity, float agressivity_level, float persistence, float venom_level, sf::Vector2f position, float maxSpeed, bool hungry, float rotation) :
		health(health),
		damage(damage),
		speed(speed),
		velocity(velocity),
		agressivity_level(agressivity_level),
		persistence(persistence),
		venom_level(venom_level),
		position(position),
		maxSpeed(maxSpeed),
		hungry(hungry),
		rotation(rotation),
		isAlive(true)
	{
		srand(time(NULL));
		clock.restart();
	}

	bool isAlive = true;

	sf::Vector2f getPosition() const
	{
		return position;
	}


	void randomizeVelocity(float maxdx, float maxdy, std::vector<Ant>& ants)
	{

		static std::mt19937 generator(1234); // Seed the generator with a fixed value
		std::uniform_real_distribution<float> distribution(-10.f, 10.f);
		float dx = distribution(generator) / maxdx;
		float dy = distribution(generator) / maxdy;

		// Add the random values to the velocity vector
		velocity.x += dx;
		velocity.y += dy;

		// Clamp the velocity vector to a maximum length
		float velLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		if (velLength > maxSpeed)
		{
			velocity /= velLength;
			velocity *= maxSpeed;
		}
	}

	void update(std::vector<Ant>& ants, const sf::RenderWindow& window, std::vector<Spider>& spiders, const Food& food, Mine& mineObj)
	{
		const float rotationSpeed = 0.5f;

		float elapsedTime = clock.getElapsedTime().asMilliseconds();
		if (elapsedTime > 10)
		{

			if (std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) > 0) {
				rotation = std::atan2(velocity.y, velocity.x) * 180 / 3.14;
				rotation /= 1;
			}

			mineRadiation(mineObj, ants);
			attack(spiders);
			rules(ants);

			if (foodVisible)
			{
				// Move spiders towards the food
				behavior(ants, window,food);

				position += velocity * 0.1f;
				randomizeVelocity(130.f, 130.f, ants);
			}
			else
			{
				// Randomize spider velocities when food is not visible on screen

				randomizeVelocity(130.f, 130.f, ants);
			}

			for (auto& trail : ants)
			{
				trail.trailPositions.push_back(trail.position);

				if (trail.trailPositions.size() > 5000) {
					trail.trailPositions.erase(trail.trailPositions.begin());
				}
			}

			position += velocity * 2.f;

			clock.restart();
		}

	}

	void attack(std::vector<Spider>& spiders) {
		float minDistance = std::numeric_limits<float>::max();
		sf::Vector2f closestSpiderPos;
		Spider* closestSpider = nullptr;
		int spiderID;

		// Find the closest spider
		for (auto& spider : spiders) {
			auto spiderPosition = spider.getPosition();
			spiderID = spider.getID();
			auto distance = std::sqrt(std::pow(position.x - spiderPosition.x, 2) + std::pow(position.y - spiderPosition.y, 2));
			if (distance < minDistance) {
				minDistance = distance;
				closestSpiderPos = spiderPosition;
				closestSpider = &spider;
			}
		}

		// If the closest spider is within range, move towards it
		if (minDistance < 150) {
			sf::Vector2f direction = closestSpiderPos - position;
			float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

			direction /= distance;
			velocity += direction * speed * persistence;
			velocity *= maxSpeed;

			// Check if the ant is on the closest spider
			if (distance <= 10) {
				// Check if enough time has passed since the last attack
				if (attackClock.getElapsedTime().asSeconds() >= 1.f) {
					// Deal 5 damage to the spider
					//std::cout << closestSpider->getHealth() << std::endl;
					closestSpider->setHealth(closestSpider->getHealth() - damage);
					closestSpider->setBeingAttacked(true);

					// Reset the clock
					attackClock.restart();
				}

				// Check if the spider is dead
				if (closestSpider->getHealth() <= 0.f) {
					closestSpider->death();
					for (auto it = spiders.begin(); it != spiders.end(); ++it) {
						if (!it->isAlive) {
							it = spiders.erase(it);
							if (it == spiders.end()) {
								break;
							}
						}
					}
				}
			}
		}
	}

	void mineRadiation(Mine& mineObj, std::vector<Ant>& ants)
	{
		const std::vector<sf::CircleShape>& mineShapes = mineObj.getMineShapes();
		const float radiationDamage = 9.1f;

			for (auto& mine : mineShapes)
			{
				sf::Vector2f minePos = mine.getPosition();
				sf::Vector2f antPos = getPosition();
				float distance = std::sqrt((antPos.x - minePos.x) * (antPos.x - minePos.x) + (antPos.y - minePos.y) * (antPos.y - minePos.y));

				if (distance <= mine.getRadius())
				{
					if (radiationClock.getElapsedTime().asSeconds() >= 1.f) {
						setHealth(getHealth() - radiationDamage);
						std::cout << getHealth() << "\n";
						radiationClock.restart();
					}

					if (getHealth() <= 0.f)
					{
						isAlive = false;
					}

					if (!isAlive)
					{
						death(ants);
					}
				}
			}
	}


	void behavior(std::vector<Ant>& ants, const sf::RenderWindow& window, const Food& food)
	{
		sf::Vector2f foodPos = food.getPositionMouse(window);
		sf::Vector2f direction = foodPos - position;
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance < 150)
		{
			// Normalize the direction vector
			direction /= distance;

			// Add some noise to the direction
			sf::Vector2f noise((std::rand() % 201 - 100), (std::rand() % 201 - 100));
			noise *= 0.01f;
			direction += noise;

			// Randomize the direction further
			sf::Vector2f randomDirection((std::rand() % 201 - 100), (std::rand() % 201 - 100));
			randomDirection *= 0.01f;
			direction += randomDirection;

			// Adjust velocity towards the food position
			velocity += direction * speed * persistence;
			velocity *= maxSpeed;
		}
	}

	void rules(std::vector<Ant>& ants)
	{
		if (position.x < 30) {
			position.x = 30;
			randomizeVelocity(130.f, 130.f, ants);
		}
		else if (position.x > 800) {
			position.x = 800;
			randomizeVelocity(130.f, 130.f, ants);
		}

		if (position.y < 30) {
			position.y = 30;
			randomizeVelocity(130.f, 130.f, ants);
		}
		else if (position.y > 600) {
			position.y = 600;
			randomizeVelocity(130.f, 130.f, ants);
		}
	}

	void setHealth(float hp) {
		health = hp;
		if (health <= 0.f) {
			isAlive = false;
		}
	}

	float getHealth() const {
		return health;
	}

	void death(std::vector<Ant>& ants) {
		if (health <= 0.f) {
			isAlive = false;
			for (auto it = ants.begin(); it != ants.end(); ++it) {
				if (!it->isAlive) {
					it = ants.erase(it);
					if (it == ants.end()) {
						break;
					}
				}
			}
		}
	}

	void draw(sf::RenderWindow& window) {
		// Draw the ant as a small circle at its current position
		sf::CircleShape spider(2.f);
		spider.setFillColor(sf::Color(255, 51, 153));
		spider.setPosition(position);
		window.draw(spider);
	}

	void MaxVision(sf::RenderWindow& window, const Ant& ant)
	{
		const int maxSeeingDistance = 150;
		sf::ConvexShape vision;

		vision.setPointCount(10);
		vision.setPoint(0, sf::Vector2f(0.f, 0.f));
		vision.setFillColor(sf::Color(153, 0, 76, 10));

		// Define the vertices of the semicircle
		for (int i = 1; i < vision.getPointCount(); ++i) {
			float angle = (i - 1) * 10.f * 3.14f / 180.f;
			float x = maxSeeingDistance * std::cos(angle);
			float y = maxSeeingDistance * std::sin(angle);

			vision.setPoint(i, sf::Vector2f(x, y));
		}

		// Set the position of the semicircle to be the position of the spider
		vision.setPosition(ant.position);

		// Rotate the semicircle by the same angle as the spider
		vision.setRotation(ant.rotation);

		window.draw(vision);
	}


	void drawtrail(sf::RenderWindow& window)
	{
		sf::VertexArray trail(sf::PrimitiveType::LineStrip, trailPositions.size());
		for (int i = 0; i < trailPositions.size(); ++i) {
			trail[i].position = trailPositions[i];
			int alpha = 255 + i * 255 / trailPositions.size() + 1;

			trail[i].color = sf::Color(255, 204, 229, alpha);
		}
		window.draw(trail);
	}


};
