#include "Object.h"

glm::ivec2 Object::resolution = glm::ivec2(0, 0);

Object::Object(std::string name) 
{
	this->name = name;
	this->color = glm::vec3(1, 1, 1);

	this->translateX = 0;
	this->translateY = 0;
	
	this->scaleX = 1;
	this->scaleY = 1;
	
	this->angularStep = 0;
}

Object::Object() : Object("") 
{
}

Object::~Object() 
{
}

std::string Object::getName() 
{
	return this->name;
}

glm::vec3 Object::getColor() 
{
	return this->color;
}

float Object::getTranslateX() 
{
 	return this->translateX;
}

float Object::getTranslateY() 
{
	return this->translateY;
}

float Object::getScaleX() 
{
	return this->scaleX;
}

float Object::getScaleY() 
{
	return this->scaleY;
}

float Object::getAngularStep() 
{
	return this->angularStep;
}

void Object::setTranslate(float translateX, float translateY) 
{
	this->translateX = translateX;
	this->translateY = translateY;
}

void Object::setScale(float scaleX, float scaleY) 
{
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

void Object::setAngularStep(float angularStep) 
{
	this->angularStep = angularStep;
}

const float Ball::radius = 10;
int Ball::maxlife = 5;

Ball::Ball(std::string name, int lifes, float speed)
{
	this->name = name;
	this->color = glm::vec3(0.02, 0.52, 0.53);
	
	this->translateX = (float) Object::resolution.x / 2;
	this->translateY = 60;
	
	this->scaleX = 1;
	this->scaleY = 1;
	
	this->angularStep = 0;

	this->lifes = lifes;
	this->speed = speed;
	
	this->strength = false;
	this->big = false;
}

Ball::Ball() : Ball("", 0, 0) 
{
}

Ball::~Ball() 
{
}

void Ball::DecreaseLife() 
{
	this->lifes = (this->lifes > 0) ? (this->lifes - 1) : 0;
}

void Ball::IncreaseLife()
{
	this->lifes++;
}

void Ball::DecreaseSpeed() 
{
	this->speed /= 1.08f;
}

void Ball::IncreaseSpeed() 
{
	this->speed *= 1.08f;
}

void Ball::NormalSpeed() 
{
	this->speed = 1;
}

int Ball::GetLife() 
{
	return this->lifes;
}

float Ball::GetSpeed() 
{
	return this->speed;
}

Mesh * Ball::getMesh() 
{
	return O2D::CreateCircle(this->name, glm::vec3(0, 0, 0), Ball::radius, this->color, true);
}

Mesh * Ball::getMeshLife(int index) 
{
	return O2D::CreateCircle("life" + std::to_string(index), glm::vec3(0, 0, 0), Ball::radius, glm::vec3(0.54, 0.02, 0.13), true);
}

bool Ball::getStrength() 
{
	return this->strength;
}

void Ball::setStrength(bool strength) 
{
	this->strength = strength;
}

bool Ball::getSize() 
{
	return this->big;
}

void Ball::setSize(bool big) 
{
	this->big = big;
}

const float Platform::length = 200;
const float Platform::width = 10;

Platform::Platform(std::string name) 
{
	this->name = name;
	this->color = glm::vec3(0.28, 0.27, 0.28);

	this->translateX = Object::resolution.x / 2 - Platform::length / 2;
	this->translateY = 15;
	
	this->scaleX = 1;
	this->scaleY = 1;
	
	this->angularStep = 0;

	this->save = false;
}

Platform::Platform() : Platform("") {}

Platform::~Platform() {}

void Platform::setSave(bool save) {
	this->save = save;
}

bool Platform::getSave()
{
	return this->save;
}

Mesh * Platform::getMesh() 
{
	return O2D::CreateRectangle(this->name, glm::vec3(0, 0, 0), Platform::length, Platform::width, this->color, true);
}

int Brick::numberbricks = 0;
const float Brick::length = 50;
const float Brick::width = 30;

Brick::Brick(std::string name, int lifetime, PowerUp * power) 
{
	this->name = name;
	this->color = glm::vec3(0.71 - 0.05 * this->lifetime, 0.45 - 0.1 * this->lifetime, 0.5 - 0.15 * this->lifetime);
	
	this->translateX = 0;
	this->translateY = 0;
	
	this->scaleX = 1;
	this->scaleY = 1;

	this->angularStep = 0;

	this->lifetime = lifetime;
	this->power = power;

	Brick::numberbricks++;
}

Brick::Brick() : Brick("", 0, nullptr) {}

Brick::~Brick() 
{
	Brick::numberbricks--;
}

void Brick::DecreaseLife() 
{
	this->lifetime = (this->lifetime > 0) ? (this->lifetime - 1) : 0;
}

void Brick::IncreaseLife() 
{
	this->lifetime++;
}

int Brick::getLifeTime() 
{
	return this->lifetime;
}

void Brick::updateColor()
{
	this->color = glm::vec3(0.71 - 0.05 * this->lifetime, 0.45 - 0.1 * this->lifetime, 0.5 - 0.15 * this->lifetime);
}

PowerUp * Brick::getPowerUp() 
{
	return this->power;
}

Mesh * Brick::getMesh() 
{
	return O2D::CreateRectangle(this->name, glm::vec3(0, 0, 0), Brick::length, Brick::width, this->color, true);
}

int Wall::numberwalls = 0;

Wall::Wall(std::string name, bool vertical) 
{
	this->name = name;
	this->color = glm::vec3(0.46, 0.38, 0.29);
	
	this->translateX = 0;
	this->translateY = 0;
	
	this->scaleX = 1;
	this->scaleY = 1;
	
	this->angularStep = 0;
	
	this->vertical = vertical;
	
	Wall::numberwalls++;
}

Wall::Wall() : Wall("", true) 
{
}

Wall::~Wall() 
{
	Wall::numberwalls--;
}

bool Wall::getVertical() 
{
	return this->vertical;
}

Mesh * Wall::getMesh() 
{
	if (this->vertical)
		return O2D::CreateRectangle(this->name, glm::vec3(0, 0, 0), 20, (float)Object::resolution.y - 90, this->color, true);
	else
		return O2D::CreateRectangle(this->name, glm::vec3(0, 0, 0), (float)Object::resolution.x, 20, this->color, true);
}

const int PowerUp::powers = 5;

PowerUp::PowerUp(std::string name, powertype type, bool used) 
{
	this->name = name;
	switch (type)
	{
	case nothing:
		this->color = glm::vec3(1, 1, 1);
		break;
	case strongerball:
		this->color = glm::vec3(0.5, 0.49, 0.56);
		break;
	case saveplatform:
		this->color = glm::vec3(0.97, 0.76, 0.36);
		break;
	case greatballsoffire:
		this->color = glm::vec3(1, 0.6, 0.36);
		break;
	case life:
		this->color = glm::vec3(0.93, 0.61, 0.64);
		break;
	default:
		this->color = glm::vec3(0, 0, 0);
		break;
	}
	this->translateX = 0;
	this->translateY = 0;

	this->scaleX = 1;
	this->scaleY = 1;
	
	this->angularStep = 0;
	
	this->type = type;
	this->used = used;
	this->started = false;
}

PowerUp::PowerUp() : PowerUp("", nothing, false) 
{
}

PowerUp::~PowerUp() 
{
}

powertype PowerUp::getPoweUpType() 
{
	return this->type;
}

Mesh * PowerUp::getMesh() 
{
	return O2D::CreateRectangle(this->name, glm::vec3(0, 0, 0), 15, 15, this->color, true);
}

bool PowerUp::getUsed() 
{
	return this->used;
}

void PowerUp::setUsed(bool used)
{
	this->used = used;
}

void PowerUp::setTime()
{
	this->started = true;
	this->start = 0;
	this->translateX = -1;
	this->translateY = -1;
}

bool PowerUp::isStarted() 
{
	return this->started;
}

float PowerUp::verifyTimer(float deltaTimeSeconds)
{
	start += deltaTimeSeconds;
	if (start > 10) {
		this->used = true;
	}
	return start;
}
