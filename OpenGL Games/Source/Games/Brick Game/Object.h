#pragma once

#include <ctime>
#include <iostream>
#include <include/glm.h>
#include <Core/Engine.h>
#include "O2D.h"

typedef enum powertype {nothing, strongerball, saveplatform, greatballsoffire,life} powertype;
typedef enum barrier { none, vertical, horizontal, corner } barrier;

class Object 
{
public:
	static glm::ivec2 resolution;

	Object(std::string name);
	Object();
	~Object();

	virtual Mesh * getMesh() = 0;

	std::string getName();
	glm::vec3 getColor();

	float getTranslateX();
	float getTranslateY();

	float getScaleX();
	float getScaleY();

	float getAngularStep();
	
	void setTranslate(float translateX, float translateY);
	void setScale(float scaleX, float scaleY);
	void setAngularStep(float angularStep);
protected:
	std::string name;
	glm::vec3 color;
	float translateX, translateY;
	float scaleX, scaleY;
	float angularStep;
};

class PowerUp : public Object
{
public:
	static const int powers;

	PowerUp(std::string name, powertype type, bool used);
	PowerUp();
	~PowerUp();

	Mesh * getMesh();
	powertype getPoweUpType();

	bool getUsed();
	void setUsed(bool used);

	void setTime();
	float verifyTimer(float deltaTimeSeconds);
	bool isStarted();
private:
	powertype type;
	bool used, started;
	float start;
};

class Ball : public Object 
{
public:
	static const float radius;
	static int maxlife;

	Ball(std::string name, int lifes, float speed);
	Ball();
	~Ball();

	Mesh * getMesh();
	Mesh * Ball::getMeshLife(int index);

	void DecreaseLife();
	void IncreaseLife();
	int GetLife();

	void DecreaseSpeed();
	void IncreaseSpeed();
	void NormalSpeed();
	float GetSpeed();

	void setStrength(bool strength);
	bool getStrength();
	
	void setSize(bool big);
	bool getSize();
private:
	int lifes;
	float speed;
	bool strength, big;
};

class Platform : public Object 
{
public:
	static const float length;
	static const float width;

	Platform(std::string name);
	Platform();
	~Platform();

	Mesh * getMesh();

	void setSave(bool save);
	bool getSave();
private: 
	bool save;
};

class Brick : public Object 
{
public:
	static int numberbricks;
	static const float length;
	static const float width;
	
	Brick(std::string name, int lifetime, PowerUp *power);
	Brick();
	~Brick();
	
	PowerUp* getPowerUp();
	Mesh * getMesh();

	void DecreaseLife();
	void IncreaseLife();
	int getLifeTime();
	void updateColor();
private:
	int lifetime;
	PowerUp* power;
};

class Wall : public Object 
{
public:
	static int numberwalls;

	Wall(std::string name, bool vertical);
	Wall();
	~Wall();

	bool getVertical();
	Mesh * getMesh();

private:
	bool vertical;
};