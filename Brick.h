#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Brick : public RectangleShape
{
private:
	int hp;

public:
	Brick(FloatRect, int);
	void hit();
	bool isDead();
	~Brick();
};

