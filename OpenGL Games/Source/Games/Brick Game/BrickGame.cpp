#include "BrickGame.h"

using namespace std;

BrickGame::BrickGame(){}
BrickGame::~BrickGame(){}

void BrickGame::Init()
{
	{
		glm::ivec2 resolution = window->GetResolution();
		auto camera = GetSceneCamera();
		camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
		camera->SetPosition(glm::vec3(0, 0, 50));
		camera->SetRotation(glm::vec3(0, 0, 0));
		camera->Update();
		GetCameraInput()->SetActive(false);
	}

	{
		Object::resolution = window->GetResolution();
		Level::resolution = window->GetResolution();

		countlevels = 0;
		startGame(true);
	}
}

void BrickGame::startGame(bool newgame)
{
	if (newgame) {

		level = new Level(countlevels);
		countlevels++;
	}

	meshes.clear();
	bricks.clear();
	terminated.clear();

	start = false;
	brokenbricks = 0;
	cosx = 0;
	sinx = 1;

	ball = new Ball(Level::getBall()->getName(), Level::getBall()->GetLife(), Level::getBall()->GetSpeed());
	platform = new Platform(Level::getPlatform()->getName());

	AddMeshToList(ball->getMesh());
	AddMeshToList(platform->getMesh());
	AddMeshToList(level->getWalls()[0]->getMesh());
	AddMeshToList(level->getWalls()[1]->getMesh());
	AddMeshToList(level->getWalls()[2]->getMesh());

	int index = 0;
	for (int rowindex = 0; rowindex < level->getObstacles().numberoflines; ++rowindex)
	{
		for (int columnindex = 0; columnindex < level->getObstacles().numberofbricksperline; ++columnindex)
		{
			if (level->getObstacles().bricks[rowindex][columnindex] != nullptr)
			{
				bricks.push_back(new Brick(level->getObstacles().bricks[rowindex][columnindex]->getName(),
					level->getObstacles().bricks[rowindex][columnindex]->getLifeTime(),
					level->getObstacles().bricks[rowindex][columnindex]->getPowerUp()));
				bricks[index]->setTranslate(level->getObstacles().bricks[rowindex][columnindex]->getTranslateX(),
					level->getObstacles().bricks[rowindex][columnindex]->getTranslateY());
				terminated.push_back(false);

				AddMeshToList(bricks[index]->getMesh());
				AddMeshToList(bricks[index]->getPowerUp()->getMesh());
				index++;
			}
		}
	}

	for (int index = 0; index < Ball::maxlife; ++index)
	{
		AddMeshToList(ball->getMeshLife(index));
	}
}

void BrickGame::FrameStart()
{
	glClearColor((GLclampf)0.79, (GLclampf)0.76, (GLclampf)0.62, (GLclampf)1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window -> GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void BrickGame::Update(float deltaTimeSeconds)
{
	barrier collision;
	{
		if (brokenbricks == level->getObstacles().realbricks) {
			startGame(true);
			return;
		}
	}

	{
		if (ball->GetLife() > 0) {
			std::string life = "life";
			float tx = 0;
			float ty = 25;
			for (int index = 0; index < ball->GetLife(); ++index) {
				modelMatrix = glm::mat3(1);
				tx += 35;
				modelMatrix *= T2D::Translate(tx, ty);
				RenderMesh2D(meshes[life + to_string(index)], shaders["VertexColor"], modelMatrix);
			}
		}
		else {
			startGame(false);
			return;
		}
	}

	{
		modelMatrix = glm::mat3(1);
		modelMatrix *= T2D::Translate(level->getWalls()[0]->getTranslateX(), level->getWalls()[0]->getTranslateY());
		RenderMesh2D(meshes[level->getWalls()[0]->getName()], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= T2D::Translate(level->getWalls()[1]->getTranslateX(), level->getWalls()[1]->getTranslateY());
		RenderMesh2D(meshes[level->getWalls()[1]->getName()], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= T2D::Translate(level->getWalls()[2]->getTranslateX(), level->getWalls()[2]->getTranslateY());
		RenderMesh2D(meshes[level->getWalls()[2]->getName()], shaders["VertexColor"], modelMatrix);

		if (start) {
			for (int index = 0; index < (int)level->getWalls().size(); ++index) {
				collision = CollisionWall(level->getWalls()[index]);
				switch (collision) {
				case none:
					break;
				case horizontal:
					sinx *= -1;
					break;
				case vertical:
					cosx *= -1;
					break;
				case corner:
					if (sinx >= 0) sinx *= -1;
					if (cosx >= 0) cosx *= -1;
					break;
				default:
					break;
				}
			}
		}
	}

	ty = 300 * deltaTimeSeconds;
	tx = 300 * deltaTimeSeconds;
	sx = 2 * deltaTimeSeconds;
	sy = 2 * deltaTimeSeconds;
	anglestep = 2 * deltaTimeSeconds;
	
	{
		for (int index = 0; index < (int)bricks.size(); ++index) {
			
			modelMatrix = glm::mat3(1);

			if (bricks.at(index)->getLifeTime() > 0) {
				if (start) {
					collision = CollisionBrick(bricks.at(index));
					switch (collision) {
					case none:
						break;
					case horizontal:
						if (!ball->getStrength()) sinx *= -1;
						else {
							while (bricks.at(index)->getLifeTime() != 0) bricks.at(index)->DecreaseLife();
						}
						bricks.at(index)->DecreaseLife();
						break;
					case vertical:
						if (!ball->getStrength()) cosx *= -1;
						else {
							while (bricks.at(index)->getLifeTime() != 0) bricks.at(index)->DecreaseLife();
						}
						bricks.at(index)->DecreaseLife();
						break;
					case corner:
						if (!ball->getStrength()) {
							if (sinx >= 0) sinx *= -1;
							if (cosx >= 0) cosx *= -1;
						}
						else {
							while (bricks.at(index)->getLifeTime() != 0) bricks.at(index)->DecreaseLife();
						}
						bricks.at(index)->DecreaseLife();
						break;
					default:
						break;
					}
				}
				modelMatrix *= T2D::Translate(bricks.at(index)->getTranslateX(), bricks.at(index)->getTranslateY());
				bricks.at(index)->updateColor();
				RenderMesh2D(meshes[bricks.at(index)->getName()], modelMatrix, bricks.at(index)->getColor());
			}
			else {

				if (!terminated.at(index)) {
					terminated.at(index) = true;
					brokenbricks++;
				}

				if (bricks.at(index)->getScaleX() >= 0 && bricks.at(index)->getScaleY() >= 0) {
					bricks.at(index)->setScale(bricks.at(index)->getScaleX() - sx, bricks.at(index)->getScaleY() - sy);
					modelMatrix *= T2D::Translate(bricks.at(index)->getTranslateX() + Brick::length / 2, bricks.at(index)->getTranslateY() + Brick::width / 2);
					modelMatrix *= T2D::Scale(bricks.at(index)->getScaleX(), bricks.at(index)->getScaleY());
					modelMatrix *= T2D::Translate(-Brick::length / 2, -Brick::width / 2);
					bricks.at(index)->updateColor();
					RenderMesh2D(meshes[bricks.at(index)->getName()], modelMatrix, bricks.at(index)->getColor());
				}

				if (bricks.at(index)->getPowerUp()->getPoweUpType() != nothing) {

					if (!bricks.at(index)->getPowerUp()->getUsed()) {
						modelMatrix = glm::mat3(1);

						if (bricks.at(index)->getPowerUp()->getAngularStep() == 2 * M_PI)
							bricks.at(index)->getPowerUp()->setAngularStep(0);
						else
							bricks.at(index)->getPowerUp()->setAngularStep(bricks.at(index)->getPowerUp()->getAngularStep() + anglestep);

						bricks.at(index)->getPowerUp()->setTranslate(bricks.at(index)->getPowerUp()->getTranslateX(), bricks.at(index)->getPowerUp()->getTranslateY() - ty/2);
						modelMatrix *= T2D::Translate(bricks.at(index)->getPowerUp()->getTranslateX(), bricks.at(index)->getPowerUp()->getTranslateY());
						modelMatrix *= T2D::Rotate(bricks.at(index)->getPowerUp()->getAngularStep() + anglestep);
						modelMatrix *= T2D::Translate(-5, -5);
						RenderMesh2D(meshes[bricks.at(index)->getPowerUp()->getName()], shaders["VertexColor"], modelMatrix);

						float timer = 0;
						if (CollisionPowerUp(bricks.at(index)->getPowerUp())) {
							bricks.at(index)->getPowerUp()->setTime();
						}

						if (bricks.at(index)->getPowerUp()->isStarted() && !bricks.at(index)->getPowerUp()->getUsed()) {
							timer = bricks.at(index)->getPowerUp()->verifyTimer(deltaTimeSeconds);
						}

						switch (bricks.at(index)->getPowerUp()->getPoweUpType()) {
						case strongerball:
							if (timer == deltaTimeSeconds) {
								ball->setStrength(true);
								ball->IncreaseSpeed();
							}
							if (bricks.at(index)->getPowerUp()->getUsed()) {
								ball->setStrength(false);
								ball->DecreaseSpeed();
							}
							break;
						case saveplatform:
							if (timer == deltaTimeSeconds) {
								platform->setSave(true);
							}
							if (bricks.at(index)->getPowerUp()->getUsed()) {
								platform->setSave(false);
							}
							break;
						case greatballsoffire:
							if (timer == deltaTimeSeconds) {
								ball->setSize(true);
							}
							if (bricks.at(index)->getPowerUp()->getUsed()) {
								ball->setSize(false);
							}
							break;
						case life:
							if (timer == deltaTimeSeconds) {
								if (ball->GetLife() < Ball::maxlife) ball->IncreaseLife();
								bricks.at(index)->getPowerUp()->setUsed(true);
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	{

		if (start) {
			ball->setTranslate(ball->getTranslateX() + ball->GetSpeed() * cosx * tx, ball->getTranslateY() + ball->GetSpeed() * sinx * ty);

			if (ball->getTranslateX() < 0 || ball->getTranslateX() > Level::resolution.x || ball->getTranslateY() < 0 || ball->getTranslateY() > Level::resolution.y) {
				start = false;
				cosx = 0;
				sinx = 1;
				ball->setTranslate(platform->getTranslateX() + Platform::length/2, 60);
				ball->DecreaseLife();
			}
		}

		if (!ball->getSize()) {
			if (ball->getScaleX() > 1 && ball->getScaleY() > 1) {
				ball->setScale(ball->getScaleX() - sx, ball->getScaleY() - sy);
			}
		}
		else {
			if (ball->getScaleX() < 2 && ball->getScaleY() < 2) {
				ball->setScale(ball->getScaleX() + sx, ball->getScaleY() + sy);
			}
		}

		modelMatrix = glm::mat3(1);
		modelMatrix *= T2D::Translate(ball->getTranslateX() + Ball::radius / 2, ball->getTranslateY() + Ball::radius / 2);
		modelMatrix *= T2D::Scale(ball->getScaleX(), ball->getScaleY());
		modelMatrix *= T2D::Translate(-Ball::radius / 2, -Ball::radius / 2);
		RenderMesh2D(meshes[ball->getName()], shaders["VertexColor"], modelMatrix);
	}

	{
		if (start) {
			float xcolision = CollisionPlatform();
			if (xcolision != -1) {

				if (platform->getSave()) {
					cosx = (xcolision - platform->getTranslateX() - Platform::length / 2) / ((3 * Platform::length) / 2);
				}
				else {
					cosx = (xcolision - platform->getTranslateX() - Platform::length / 2) / (Platform::length / 2);
				}

				sinx = sqrt(1 - cosx * cosx);
			}
		}

		if (!platform->getSave()) {

			if (platform->getScaleX() > 1) {
				platform->setScale(platform->getScaleX() - 3 * deltaTimeSeconds, platform->getScaleY());
			}
		}
		else {
			if (platform->getScaleX() < 2) {
				platform->setScale(platform->getScaleX() + 3 * deltaTimeSeconds, platform->getScaleY());
			}
		}

		modelMatrix = glm::mat3(1);
		modelMatrix *= T2D::Translate(platform->getTranslateX() + Platform::length / 2, platform->getTranslateY() + Platform::width / 2);
		modelMatrix *= T2D::Scale(platform->getScaleX(), platform->getScaleY());
		modelMatrix *= T2D::Translate(-Platform::length / 2, -Platform::width / 2);
		RenderMesh2D(meshes[platform->getName()], shaders["VertexColor"], modelMatrix);
	}
}

void BrickGame::FrameEnd() {}

void BrickGame::OnInputUpdate(float deltaTime, int mods) {}

void BrickGame::OnKeyPress(int key, int mods) {}

void BrickGame::OnKeyRelease(int key, int mods) {}

void BrickGame::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (!start)
	{
		ball->setTranslate((float)mouseX, ball->getTranslateY());
	}

	platform->setTranslate((float) mouseX - Platform::length/2, platform->getTranslateY());
}

void BrickGame::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)
	{
		start = true;
	}
}

void BrickGame::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void BrickGame::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void BrickGame::OnWindowResize(int width, int height) {}

barrier BrickGame::CollisionBrick(Brick* obstacle)
{
	float radius = (ball->getSize()) ? (2 * Ball::radius) : Ball::radius;
	float closestXtoObstacle = fmaxf(obstacle->getTranslateX(), fminf(ball->getTranslateX(), obstacle->getTranslateX() + Brick::length));
	float closestYtoObstacle = fmaxf(obstacle->getTranslateY(), fminf(ball->getTranslateY(), obstacle->getTranslateY() + Brick::width));

	if ((ball->getTranslateX() - closestXtoObstacle) * (ball->getTranslateX() - closestXtoObstacle) +
		(ball->getTranslateY() - closestYtoObstacle) * (ball->getTranslateY() - closestYtoObstacle) > radius * radius)
	{
		return none;
	}
	else 
	{
		if (closestXtoObstacle == obstacle->getTranslateX() || closestXtoObstacle == obstacle->getTranslateX() + Brick::length)
		{
			if (closestYtoObstacle == obstacle->getTranslateY() || closestYtoObstacle == obstacle->getTranslateX() + Brick::width)
			{
				return corner;
			}
			else
			{
				return vertical;
			}
		}
		else
		{
			return horizontal;
		}
	}
}

barrier BrickGame::CollisionWall(Wall * obstacle)
{
	float radius = (ball->getSize()) ? (2 * Ball::radius) : Ball::radius;
	float width, length;
	
	if (obstacle->getVertical()) 
	{
		length = 20;
		width = (float) Object::resolution.y - 90;
	}
	else 
	{
		length = (float) Object::resolution.x;
		width = 20;
	}

	float closestXtoObstacle = fmaxf(obstacle->getTranslateX(), fminf(ball->getTranslateX(), obstacle->getTranslateX() + length));
	float closestYtoObstacle = fmaxf(obstacle->getTranslateY(), fminf(ball->getTranslateY(), obstacle->getTranslateY() + width));
	
	
	if ((ball->getTranslateX() - closestXtoObstacle) * (ball->getTranslateX() - closestXtoObstacle) +
		(ball->getTranslateY() - closestYtoObstacle) * (ball->getTranslateY() - closestYtoObstacle) > radius * radius) {
		return none;
	}
	else 
	{
		if (closestXtoObstacle == obstacle->getTranslateX() || closestXtoObstacle == obstacle->getTranslateX() + length) 
		{
			if (closestYtoObstacle == obstacle->getTranslateY() || closestYtoObstacle == obstacle->getTranslateX() + width)
			{
				return corner;
			}
			else
			{
				return vertical;
			}
		}
		else
		{
			return horizontal;
		}
	}
}

float BrickGame::CollisionPlatform()
{
	float radius = (ball->getSize()) ? (2 * Ball::radius) : Ball::radius;
	float closestXtoObstacle;
	float closestYtoObstacle = fmaxf(platform->getTranslateY(), fminf(ball->getTranslateY(), platform->getTranslateY() + Platform::width));

	if (platform->getSave()) 
	{
		closestXtoObstacle = fmaxf(platform->getTranslateX() - Platform::length/2, fminf(ball->getTranslateX(), platform->getTranslateX() + (3 * Platform::length)/2));
	}
	else 
	{
		closestXtoObstacle = fmaxf(platform->getTranslateX(), fminf(ball->getTranslateX(), platform->getTranslateX() + Platform::length));
	}

	if ((ball->getTranslateX() - closestXtoObstacle) * (ball->getTranslateX() - closestXtoObstacle) +
		(ball->getTranslateY() - closestYtoObstacle) * (ball->getTranslateY() - closestYtoObstacle) > radius * radius) 
	{
		return -1;
	}	
	else 
	{
		return closestXtoObstacle;
	}
}

bool BrickGame::CollisionPowerUp(PowerUp* power)
{
	float closestXtoObstacle;
	float closestYtoObstacle = fmaxf(platform->getTranslateY(), fminf(power->getTranslateY(), platform->getTranslateY() + Platform::width));

	if (platform->getSave()) 
	{
		closestXtoObstacle = fmaxf(platform->getTranslateX() - Platform::length / 2, fminf(power->getTranslateX(), platform->getTranslateX() + (3 * Platform::length) / 2));
	}
	else 
	{
		closestXtoObstacle = fmaxf(platform->getTranslateX(), fminf(power->getTranslateX(), platform->getTranslateX() + Platform::length));
	}
	if ((power->getTranslateX() - closestXtoObstacle) * (power->getTranslateX() - closestXtoObstacle) +
		(power->getTranslateY() - closestYtoObstacle) * (power->getTranslateY() - closestYtoObstacle) > 112.5) 
	{ 
		return false;
	}
	else
	{
		return true;
	}
}
