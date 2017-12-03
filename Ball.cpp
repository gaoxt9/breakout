#include "Ball.h"

Ball::Ball()
{}

void Ball::setDirection(Vector2f newDirection)
{
	this->direction = newDirection;
}

Vector2f Ball::getDirection()
{
	return this->direction;
}

void Ball::addVelocity()
{
	this->velocity += 30.f;
}

void Ball::setVelocity(float velocity)
{
	this->velocity = velocity;
}

void Ball::moveBall(float dt)
{
	this->setPosition(this->getPosition() + this->direction * this->velocity * dt);
}

Ball::~Ball()
{
}
