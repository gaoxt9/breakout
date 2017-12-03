#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
 
#include <vector>
#include <cstdlib>
#include <memory>
#include <cmath>
#include <ctime>

#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"

using namespace sf;

const int gameWidth = 1500;
const int gameHeight = 1100;

int life = 3;
Text lifeTxt, scoreTxt, restartTxt;
int score = 0;
int level = 0;
float ballVelocity = 500.f;

float PI = 3.1415926;

float blockWidth = 200;
float blockHeight = 50;
const float blockGap = 5;

Paddle basePaddle;
Vector2f oriPaddleSize(150, 30);
Ball baseBall;
float ballRadius = 18.f;
const float paddleSpeed = 600.f;
double ballAngle = 0.f;
const float ballSpeed = 400.f;
bool shootBall = false;
float initVelocity = 200.f;

SoundBuffer buf1, buf2, buf3, buf4, buf5, buf6, buf7;
Sound paddleHit, brickDestroyed, brickDamage, ballBounce, loseLife, win, hitStone;
Music background;

Texture normalBr, strongBr, stoneBr, movingBr;

std::vector<std::unique_ptr<Brick>> bricks;
std::vector<std::unique_ptr<Brick>> stones;
RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "Max's Breakout!");

void create_level1_bricks();
void create_level2_bricks();
void create_level3_bricks();
void update_state(float);
void render_frame();
void reset_game();
void reset_ball();

bool intersects(const Brick & b, const CircleShape & cir)
{
	FloatRect r1 = b.getGlobalBounds();
	FloatRect r2 = cir.getGlobalBounds();
	return r1.intersects(r2);
}


int main()
{
	Font font;
	font.loadFromFile("arial.TTF");

	normalBr.loadFromFile("normal_br.jpeg");
	strongBr.loadFromFile("strong_br.png");
	stoneBr.loadFromFile("stone_br.jpg");

	lifeTxt.setFont(font);
	lifeTxt.setCharacterSize(50);
	lifeTxt.setFillColor(sf::Color::Red);
	lifeTxt.setStyle(Text::Bold);
	lifeTxt.setPosition(20, gameHeight - 80);

	scoreTxt.setFont(font);
	scoreTxt.setCharacterSize(50);
	scoreTxt.setFillColor(sf::Color::Red);
	scoreTxt.setStyle(Text::Bold);
	scoreTxt.setPosition(gameWidth - 250, gameHeight - 80);

	restartTxt.setFont(font);
	restartTxt.setCharacterSize(70);
	restartTxt.setFillColor(sf::Color::Red);
	restartTxt.setStyle(Text::Bold);
	restartTxt.setPosition(gameWidth / 4, gameHeight / 2);

	buf1.loadFromFile("knock.wav");
	buf2.loadFromFile("break.wav");
	buf3.loadFromFile("crack.wav");
	buf4.loadFromFile("bounce.wav");
	buf5.loadFromFile("lose_life.wav");
	buf6.loadFromFile("win.wav");
	buf7.loadFromFile("hit_stone.wav");

	paddleHit.setBuffer(buf1);
	brickDestroyed.setBuffer(buf2);
	brickDamage.setBuffer(buf3);
	ballBounce.setBuffer(buf4);
	loseLife.setBuffer(buf5);
	win.setBuffer(buf6);
	hitStone.setBuffer(buf7);

	background.openFromFile("background.flac");
	background.play();
	background.setLoop(true);

	Clock clock;
	create_level1_bricks();
	bool isPlaying = true;
	std::srand(std::time(0));
	
	basePaddle.setSize(oriPaddleSize);

	// Create the ball
	baseBall.setRadius(ballRadius);
	baseBall.setOutlineThickness(3);
	baseBall.setOutlineColor(sf::Color::Black);
	baseBall.setFillColor(sf::Color::White);
	
	// Create the base paddle
	basePaddle.setSize(oriPaddleSize - sf::Vector2f(3, 3));
	basePaddle.setOutlineThickness(3);
	basePaddle.setOutlineColor(sf::Color::Black);
	basePaddle.setFillColor(sf::Color(100, 100, 200));
	basePaddle.setPosition(gameWidth / 2 - oriPaddleSize.x / 2, gameHeight - 35);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		if (isPlaying)
		{
			float dt = clock.restart().asSeconds();

			float factor = ballSpeed * dt;

			if (Keyboard::isKeyPressed(Keyboard::Left) && (basePaddle.getPosition().x > 0))
			{
				basePaddle.move(-paddleSpeed * dt, 0.f);;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right) && (basePaddle.getPosition().x + oriPaddleSize.x < gameWidth))
			{
				basePaddle.move(paddleSpeed * dt, 0.f);
			}

			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				shootBall = true;
				int flag = std::rand() % 78;
				double randomAngle = static_cast<double>(flag) / 100;
				baseBall.setDirection(Vector2f(pow(-1, flag) * sin(randomAngle), -cos(randomAngle)));
				baseBall.setVelocity(ballVelocity);
			}

			if (!shootBall)
			{
				baseBall.setPosition(basePaddle.getPosition().x + oriPaddleSize.x / 2 - ballRadius, basePaddle.getPosition().y - ballRadius * 2 - 8.f);
			}

			// Check collisions between the ball and the screen
			if (baseBall.getPosition().x <= 0.f && baseBall.getDirection().x < 0)
			{
				baseBall.setDirection(Vector2f(-baseBall.getDirection().x, baseBall.getDirection().y));
			}
			if (baseBall.getPosition().x + ballRadius * 2 >= gameWidth  && baseBall.getDirection().x > 0)
			{
				baseBall.setDirection(Vector2f(-baseBall.getDirection().x, baseBall.getDirection().y));
			}
			if (baseBall.getPosition().y <= 0.f  && baseBall.getDirection().y < 0)
			{		
				baseBall.setDirection(Vector2f(baseBall.getDirection().x, -baseBall.getDirection().y));
			}
			if (baseBall.getPosition().y + ballRadius * 2 >= gameHeight)
			{
				loseLife.play();
				life--;
				if (life == 0)
				{
					isPlaying = false;
					//reset_game();
				}
				else 
				{
					reset_ball();
				}
			}

			
            // Check the collisions from paddles
            if (baseBall.getGlobalBounds().intersects(basePaddle.getGlobalBounds()))
            {
				paddleHit.play();

				if (baseBall.getPosition().x >= basePaddle.getPosition().x + oriPaddleSize.x)
				{
					baseBall.setDirection(Vector2f(sin(PI/3), -cos(PI /3)));
				}
				else if (baseBall.getPosition().x >= basePaddle.getPosition().x + oriPaddleSize.x * 2 / 3)
				{
					baseBall.setDirection(Vector2f(sin(PI / 4), -cos(PI / 4)));
				}
				else if (baseBall.getPosition().x >= basePaddle.getPosition().x + oriPaddleSize.x / 3)
				{
					int flag = std::rand() % 30;
					double randomAngle = static_cast<double>(flag) / 100;
					baseBall.setDirection(Vector2f(pow(-1, flag) * sin(randomAngle), -cos(randomAngle)));
				}
				else if (baseBall.getPosition().x > basePaddle.getPosition().x)
				{
					baseBall.setDirection(Vector2f(-sin(PI / 4), -cos(PI / 4)));
				}
				else if (baseBall.getPosition().x <= basePaddle.getPosition().x)
				{
					baseBall.setDirection(Vector2f(-sin(PI / 3), -cos(PI / 3)));
				}
				else
				{
					baseBall.setDirection(Vector2f(sin(0), -cos(0)));
				}

            }

			update_state(dt);
			render_frame();
		}
		else
		{
			restartTxt.setString("Press \"Enter\" to restart");
			window.draw(restartTxt);
			window.display();

			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
				reset_game();
				isPlaying = true;
			}
		}

	}

	return 0;
}

void reset_game()
{
	life = 3;
	ballVelocity = 500;
	score = 0;
	level = 0;
	bricks.clear();
	create_level1_bricks();
	shootBall = false;
	baseBall.setVelocity(0);
	basePaddle.setPosition(gameWidth / 2 - oriPaddleSize.x / 2, gameHeight - 35);
}

void reset_ball()
{
	shootBall = false;
	baseBall.setVelocity(0);
	baseBall.setPosition(basePaddle.getPosition().x + oriPaddleSize.x / 2 - ballRadius, basePaddle.getPosition().y - ballRadius * 2 - 8.f);
}

void create_level1_bricks()
{
	FloatRect r(0, 0, blockWidth, blockHeight);
	int hp = 1;
	float px = 40, py = 120;

	for (int i = 0; i < 21; i++)
	{
		if (px + blockWidth >= gameWidth)
		{
			px = 40.f;
			py = py + blockHeight + blockGap;
		}

		Brick *b = new Brick(r, hp);

		b->setTexture(&normalBr);
		b->setPosition(px, py);
		px += blockWidth + blockGap;
		bricks.push_back(move(std::unique_ptr<Brick>(b)));
	}

	for (int i = 0; i < 2; i++)
	{
		Brick *s = new Brick(r, 999);

		s->setTexture(&stoneBr);
		s->setPosition(300 + i * 600, 550);
		stones.push_back(move(std::unique_ptr<Brick>(s)));
	}

}

void create_level2_bricks()
{
	FloatRect r(0, 0, blockWidth, blockHeight);
	int hp = 1;
	float px = 40, py = 120;

	for (int i = 0; i < 21; i++)
	{
		if (px + blockWidth >= gameWidth)
		{
			px = 40.f;
			py = py + blockHeight + blockGap;
		}

		Brick *b = new Brick(r, hp);
		Brick *moveBr = new Brick(r, 2);


		if (i >= 8 && i <= 12)
		{
			moveBr->setTexture(&strongBr);
			moveBr->setPosition(px, py);
			px += blockWidth + blockGap;
			bricks.push_back(move(std::unique_ptr<Brick>(moveBr)));
		}
		else
		{
			b->setTexture(&normalBr);
			b->setPosition(px, py);
			px += blockWidth + blockGap;
			bricks.push_back(move(std::unique_ptr<Brick>(b)));
		}
	}
}

void create_level3_bricks()
{
	FloatRect r(0, 0, blockWidth, blockHeight);
	int hp = 1;
	float px = 40, py = 120;

	for (int i = 0; i < 28; i++)
	{
		if (px + blockWidth >= gameWidth)
		{
			px = 40.f;
			py = py + blockHeight + blockGap;
		}

		Brick *b = new Brick(r, hp);
		Brick *moveBr = new Brick(r, 2);

		if (i > 7 && i < 13)
		{
			moveBr->setTexture(&strongBr);
			moveBr->setFillColor(Color(255, 255, 255, 0));
			moveBr->setPosition(px, py);
			px += blockWidth + blockGap;
			bricks.push_back(move(std::unique_ptr<Brick>(moveBr)));
		}
		else
		{
			b->setTexture(&normalBr);
			b->setPosition(px, py);
			px += blockWidth + blockGap;
			bricks.push_back(move(std::unique_ptr<Brick>(b)));
		}
	}
}

void render_frame()
{
	lifeTxt.setString("Life: " + std::to_string(life));
	scoreTxt.setString("Score: " + std::to_string(score));
	window.clear();
	window.draw(basePaddle);
	window.draw(baseBall);
	for (unsigned int i = 0; i < bricks.size(); ++i)
	{
		window.draw(*bricks[i]);		
	}
	if (level % 3 == 0)
	{
		for (unsigned int i = 0; i < stones.size(); ++i)
		{
			window.draw(*stones[i]);
		}
	}
	window.draw(lifeTxt);
	window.draw(scoreTxt);
	window.display();
}

void update_state(float dt)
{
	baseBall.moveBall(dt);

	for (unsigned int i = 0; i < bricks.size();)
	{
		Brick *b = bricks[i].get();

		if (intersects(*b, baseBall))
		{
			brickDamage.play();
			b->hit();
			b->setTexture(&normalBr);

			if (baseBall.getPosition().x >= b->getPosition().x + oriPaddleSize.x && baseBall.getDirection().x < 0)
			{
				baseBall.setDirection(Vector2f(-baseBall.getDirection().x, baseBall.getDirection().y));
			}
			else if (baseBall.getPosition().x <= b->getPosition().x && baseBall.getDirection().x > 0)
			{
				baseBall.setDirection(Vector2f(-baseBall.getDirection().x, baseBall.getDirection().y));
			}
			else if (baseBall.getPosition().y + ballRadius * 2 <= b->getPosition().y && baseBall.getDirection().y > 0)
			{
				baseBall.setDirection(Vector2f(baseBall.getDirection().x, -baseBall.getDirection().y));
			}
			else if (baseBall.getPosition().y - ballRadius * 2 >= b->getPosition().y && baseBall.getDirection().y < 0)
			{
				baseBall.setDirection(Vector2f(baseBall.getDirection().x, -baseBall.getDirection().y));
			}

			if (b->isDead())
			{
				brickDestroyed.play();
				bricks.erase(bricks.begin() + i);
				score++;
				continue;
			}
		}

		++i;
	}

	if (level % 3 == 0)
	{
		for (unsigned int i = 0; i < stones.size();)
		{
			Brick *s = stones[i].get();

			if (intersects(*s, baseBall))
			{
				hitStone.play();
				s->hit();
				if (baseBall.getPosition().x >= s->getPosition().x + oriPaddleSize.x && baseBall.getDirection().x < 0)
				{
					baseBall.setDirection(Vector2f(-baseBall.getDirection().x, baseBall.getDirection().y));
				}
				else if (baseBall.getPosition().x <= (s->getPosition()).x && baseBall.getDirection().x > 0)
				{
					baseBall.setDirection(Vector2f(-baseBall.getDirection().x, baseBall.getDirection().y));
				}
				else if (baseBall.getPosition().y + oriPaddleSize.y <= s->getPosition().y && baseBall.getDirection().y > 0)
				{
					baseBall.setDirection(Vector2f(baseBall.getDirection().x, -baseBall.getDirection().y));
				}
				else if (baseBall.getPosition().y - oriPaddleSize.y >= s->getPosition().y && baseBall.getDirection().y < 0)
				{
					baseBall.setDirection(Vector2f(baseBall.getDirection().x, -baseBall.getDirection().y));
				}
			}
			++i;
		}
	}


	if (bricks.size() <= 0)
	{
		win.play();
		level++;
		ballVelocity += 50;
		reset_ball();

		switch (level % 3)
		{
		case 0:
			create_level1_bricks();
			break;
		case 1:
			create_level2_bricks();
			break;
		case 2:
			create_level3_bricks();
			break;
		default:
			break;
		}
	}
}


