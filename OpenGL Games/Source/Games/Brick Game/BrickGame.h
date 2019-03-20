#pragma once

#include "T2D.h"
#include "O2D.h"
#include "Object.h"
#include "Level.h"

#include <vector>
#include <iostream>
#include <ctime>

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class BrickGame : public SimpleScene
{
	public:
		BrickGame();
		~BrickGame();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		barrier BrickGame::CollisionBrick(Brick* obstacle);
		barrier BrickGame::CollisionWall(Wall* obstacle);
		float BrickGame::CollisionPlatform();
		bool BrickGame::CollisionPowerUp(PowerUp* power);

		void startGame(bool newgame);

	protected:
		Level* level;
		int countlevels;

		Ball* ball;
		Platform* platform;
		std::vector<Brick *> bricks;
		std::vector<bool> terminated;
		int brokenbricks;
		bool start;

		glm::mat3 modelMatrix;
		float tx, ty;
		float sx, sy;
		float anglestep;
		float cosx, sinx;
};
