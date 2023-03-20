#pragma once

#include <list>

struct Spider
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
	int m_id;
	bool beingAttacked = false;
	sf::Clock attackClock;
	sf::Clock ageClock;
	sf::Clock mineClock;
	std::vector<sf::Clock> mineClocks;


public:
	Spider(int m_id,float health, float damage, float speed, sf::Vector2f velocity, float agressivity_level, float persistence, float venom_level, sf::Vector2f position, float maxSpeed, bool hungry, float rotation)
		: m_id(m_id)
		, health(health)
		, damage(damage)
		, speed(speed)
		, velocity(velocity)
		, agressivity_level(agressivity_level)
		, persistence(persistence)
		, venom_level(venom_level)
		, position(position)
		, maxSpeed(maxSpeed)
		, hungry(hungry)
		, rotation(rotation)
		, isAlive(true)
	{
		srand(time(NULL));
		clock.restart();
	}

	bool isAlive = true;

	int getID() const { return m_id; }
	bool isBeingAttacked() const { return beingAttacked; }
	void setBeingAttacked(bool attacked) { beingAttacked = attacked; }
	

	void randomizeVelocity(float maxdx, float maxdy, std::vector<Spider>& spiders)
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

	void update(std::vector<Spider>& spiders, sf::RenderWindow& window, const Food& food, Mine& mineObj)
	{
		const float rotationSpeed = 0.5f;
		sf::CircleShape mine(20.f);

		plantMines(window, mine, beingAttacked, isAlive, mineObj);

		/*for (auto sp = spiders.begin(); sp != spiders.end(); ++sp)
		{
			if (!sp->isAlive)
			{
				continue;
			}

			for (auto it = mineObj.mineShapes.begin(); it != mineObj.mineShapes.end();)
			{
				if (mineObj.intersects(*it))
				{
					it = mineObj.mineShapes.erase(it);
				}
				else
				{
					++it;
				}
			}
		}*/

		float elapsedTime = clock.getElapsedTime().asMilliseconds();

		if (elapsedTime > 10)
		{

			if (std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) > 0) {
				rotation = std::atan2(velocity.y, velocity.x) * 180 / 3.14;
				rotation /= 1;
			}

			attack();
			rules(spiders);

			if (foodVisible)
			{
				// Move spiders towards the food
				behavior(spiders,window,food);

				position += velocity * 0.1f;
				randomizeVelocity(130.f, 130.f, spiders);
			}
			else
			{
				// Randomize spider velocities when food is not visible on screen

				randomizeVelocity(130.f, 130.f, spiders);
			}

			for (auto& trail : spiders)
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

	float getRadius() const { return getRadius(); }

	void behavior(std::vector<Spider>& spiders, sf::RenderWindow& window, const Food& food)
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

	void attack() 
	{

	}

	void plantMines(
		sf::RenderWindow& window
		, sf::CircleShape mine
		, bool beingAttacked
		, bool isAlive
		, Mine& pMine)
	{
		pMine.plantMine(window, mine, beingAttacked, isAlive, position);
	}



	void rules(std::vector<Spider>& spiders)
	{
		if (position.x < 30) {
			position.x = 30;
			randomizeVelocity(130.f, 130.f, spiders);
		}
		else if (position.x > 800) {
			position.x = 800;
			randomizeVelocity(130.f, 130.f, spiders);
		}

		if (position.y < 30) {
			position.y = 30;
			randomizeVelocity(130.f, 130.f, spiders);
		}
		else if (position.y > 600) {
			position.y = 600;
			randomizeVelocity(130.f, 130.f, spiders);
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

	void death() {
		if (health <= 0.f) {
			isAlive = false;
			setBeingAttacked(false);
		}
	}

	void draw(sf::RenderWindow& window) {
		if (isAlive) {
			sf::CircleShape spider(2.f);
			spider.setFillColor(sf::Color::Blue);
			spider.setPosition(position);
			window.draw(spider);
		}
	}

	void MaxVision(sf::RenderWindow& window, const Spider& spider)
	{
		const int maxSeeingDistance = 150;
		sf::ConvexShape vision;

		vision.setPointCount(10);
		vision.setPoint(0, sf::Vector2f(0.f, 0.f));
		vision.setFillColor(sf::Color(0, 0, 153, 10));

		// Define the vertices of the semicircle
		for (int i = 1; i < vision.getPointCount(); ++i) {
			float angle = (i - 1) * 10.f * 3.14f / 180.f;
			float x = maxSeeingDistance * std::cos(angle);
			float y = maxSeeingDistance * std::sin(angle);

			vision.setPoint(i, sf::Vector2f(x, y));
		}

		// Set the position of the semicircle to be the position of the spider
		vision.setPosition(spider.position);

		// Rotate the semicircle by the same angle as the spider
		vision.setRotation(spider.rotation);

		window.draw(vision);
	}


	void drawtrail(sf::RenderWindow& window)
	{
		sf::VertexArray trail(sf::PrimitiveType::LineStrip, trailPositions.size());
		for (int i = 0; i < trailPositions.size(); ++i) {
			trail[i].position = trailPositions[i];
			int alpha = 255 + i * 255 / trailPositions.size() +1;

			trail[i].color = sf::Color(204, 204, 255, alpha);
		}
		window.draw(trail);
	}

	sf::Vector2f getPosition() const
	{
		return position;
	}

};
