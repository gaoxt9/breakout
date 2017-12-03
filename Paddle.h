#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Paddle : public sf::RectangleShape
{
public:
	float velocity;
	void setDirection(Vector2f);
	Vector2f getDirection();
	void addVelocity();
	void setVelocity(float);

	Paddle();
	void setVelocity(float);
	void movePaddle(float);

	~Paddle();
};
