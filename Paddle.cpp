#include "paddle.h"

Paddle::Paddle()
{

}

void Paddle::setVelocity(float newVel)
{
	this->velocity = newVel;
}

void Paddle::movePaddle(float dt)
{
	this->setPosition(this->getPosition() + Vector2f(0, velocity * dt));
}

Paddle::~Paddle()
{}

