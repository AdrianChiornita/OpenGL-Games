#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <vector>
#include <string>
#include <iostream>

#include "O3D.h"
#include "T3D.h"
#include "Utilities.h"
#include "Objects3D.h"

using namespace std; 
using namespace glm;

using namespace PoolGameUtilities;
using namespace Objects3D;

typedef struct GameControl {
	bool draw_triangle;
	bool start_game;
	bool draw_cue;
	bool shoot;
	bool moving_scene;
	bool shoot_try;
	bool fault;
	bool prepare_shot;
	bool translate_cue;
	bool calculate_stats;
	bool switch_players;
	bool helpline;
	balltype first;
	vec3 futurewhite;
	int yellowcount; 
	int redcount;
	
}GameControl;

class PoolGame : public SimpleScene
{
	public:
		PoolGame();
		~PoolGame();

		void Init() override;
		void RenderMesh(const Mesh * mesh, const Material3D material, const Shader * shader, const mat4 & modelMatrix);
	
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

		void UpdateTable(float deltaTimeSeconds, mat4 modelMatrix);
		void RestartGame();

		bool white_is_in_other_ball(const vec3 translate);
		vec3 calculateSecondPoint(float deltaTimeSeconds);

	protected:
		Camera *camera;
		Light light;
		unordered_map<string, Material3D> materialmap;
		PoolTable *gameTable;
		BallObject *white;
		BallObject *black;
		vector<BallObject *> yellows;
		vector<BallObject *> reds;
		vector<BallObject *> allballs;
		CueObject *cue;
		GameControl control;
		GameLogic* logic;
		vector<Mesh *> room;
};
