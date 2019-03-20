#pragma once
#include <include/glm.h>
#include <include/math.h>
#include <Core/Engine.h>

#include <iostream>

using namespace glm;
using namespace std;

namespace PoolGameUtilities
{

	Mesh * CreateMesh(const char *name, const vector<VertexFormat>& vertices, const vector<unsigned short>& indices, const unsigned int primitive);

	typedef enum CameraType { 
		FirstPerson,
		ThirdPerson
	} CameraType;

	typedef struct Projection
	{
		float fieldOfViewY;
		float zNear;
		float zFar;
		float aspectRatio;
		float left;
		float right;
		float bottom;
		float top;
	}Projection;

	typedef struct Light {
		Mesh* lampmesh;
		glm::vec3 position;
		glm::vec3 ambient_color;
		glm::vec3 diffuse_color;
		glm::vec3 specular_color;
		glm::vec3 emisive_color;
		glm::vec3 direction;
	}Light;

	typedef struct Material3D {
		float Ke;
		float Ka;
		float Kd;
		float Ks;
		int shininess;
		Texture2D* texture;
	}Material3D;

	class Camera
	{
		public:
			CameraType type;

			float zoomspeed;
			float movespeed;
			float sensivityOX;
			float sensivityOY;

			Projection projectioninformation;

			Camera(const vec3 &position, const vec3 center, const vec3 &up);
			Camera();
			~Camera();

			void set(const vec3 &position, const vec3 center, const vec3 &up);
			void moveForward(const float distance);

			void translateForward(const float distance);
			void translateUpword(const float distance);
			void translateRight(const float distance);

			void rotateFirstPerson_OX(const float angle);
			void rotateFirstPerson_OY(const float angle);
			void rotateFirstPerson_OZ(const float angle);
			
			void rotateThirdPerson_OX(const float angle);
			void rotateThirdPerson_OY(const float angle);
			void rotateThirdPerson_OZ(const float angle);

			void SetPerspective(const float fieldOfViewY, const float aspectRatio, const float zNear, const float zFar);
			void SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

			const mat4 getViewMatrix();
			const mat4 getProjectionMatrix();

			const vec3 getTargetPosition();
			const vec3 getForward();
			const vec3 getRight();
			const vec3 getUp();

			const vec3 getEyePosition();
			const vec3 getPreviousEyePosition();
			void setToPreviousEyePosition();
			void setPreviousEyePosition(const vec3 previous_position);
		
		private:
			vec3 position, forward, right, up;
			vec3 previous_position;
			float distancetotarget;
			mat4 projectionMatrix;
		};
}