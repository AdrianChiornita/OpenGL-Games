#pragma once

#include <include/glm.h>
#include <Core/Engine.h>

#include "O3D.h"
#include "T3D.h"

using namespace std;
using namespace glm;

namespace Objects3D 
{	
	typedef enum balltype { emptyball, cueball, matchball, redball, yellowball } balltype;

	constexpr float friction_coefficient = 0.05f;
	constexpr float gravitational_acceleration = 9.8065f;

	constexpr float offsetbig = 0.18f;
	constexpr float offsetsmall = 0.08f;

	class PoolPlayer
	{
	public:
		string name;
		balltype playercolor;
		balltype firsttouched;
		int faults;
		int inpocket;
		bool active;

		PoolPlayer(string name);
	};

	class GameLogic
	{
	public:
		PoolPlayer* firstplayer;
		PoolPlayer* secondplayer;

		GameLogic();
		~GameLogic();

		void switchplayer();
		void statsplayer();
		PoolPlayer* getActivePlayer();
		PoolPlayer* getInactivePlayer();
	};

	class BallObject
	{
		public: 
			static const float radius;
			
			string name;
			balltype type;
			Mesh* mesh;

			vec3 translate_coordinates;
			vec3 rotate_axis;
			float rotate_angle;
			
			vec3 trajectory;
			vec3 movement;
			float velocity;
			float mass;

			bool moving;
			bool isinpocket;

			BallObject(const string name, const balltype type);
			~BallObject();
			void CreateMesh();

			void Collisions(const vec3 relative, const float length, const float width);
			void BanksColiision(const vec3 relative, const float length, const float width);
			bool TreatBallCollision(BallObject* ball);

			bool isInPocket(const vec3 relative, const float length, const float width);
			bool BankColiision(const vec3 leftbottom, const vec3 righttop, const vec3 normal);
			bool BallColiision(BallObject* ball);

			const vec3 RectangleClosestPoint(const vec3 leftbottom, const vec3 righttop);

			void Update(const float deltaTime);
			void ResetBall(const vec3 translate);
	};

	class CueObject
	{
		public:
			static const vec3 position_relative;

			string name;
			vec3 translate_coordinates;
			vec3 rotate_angle;
			float k;
			float distance;
			Mesh* mesh;
			Mesh* line;

			CueObject(const string name);
			~CueObject();
			void CreateMesh();
	};

	class PoolObject
	{
		public:
			string name;
			vec3 position;
			Mesh* mesh;

			PoolObject(const string name, Mesh* mesh, const vec3 position);
			~PoolObject();
	};

	class PoolTable
	{
		public:
			string name;
			unordered_map<string, PoolObject *> objects;
			vec3 position;
			vec3 relative;
			float lengthtable;
			float widthtable;

			PoolTable(const string name);
			~PoolTable();

			void CreateTableMeshes();
	};
}