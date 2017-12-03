#include "Brick.h"

Brick::Brick(FloatRect r, int hp)
{
	this->setSize(Vector2f(200, 50));
	this->hp = hp;
}

void Brick::hit()
{
	--(this->hp);
}

bool Brick::isDead()
{
	return (this->hp) <= 0;
}


Brick::~Brick()
{
}
