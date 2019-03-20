#pragma once

#include <include/glm.h>
#include <include/math.h>

namespace CityCamera {

    typedef enum CameraType { 
	    FirstPerson, ThirdPerson
	} CameraType;

	typedef struct Projection {
		float fieldOfViewY;
		float zNear;
		float zFar;
		float aspectRatio;
		float left;
		float right;
		float bottom;
		float top;
	} Projection;

	class Camera {
		public:
            Camera  (const glm::vec3 &position, const  glm::vec3 center, const  glm::vec3 &up);
			Camera  ();
			~Camera ();

			void set         (const  glm::vec3 &position, const  glm::vec3 center, const  glm::vec3 &up);
			void moveForward (const float distance);

			void translateForward (const float distance);
			void translateUpword  (const float distance);
			void translateRight   (const float distance);

			void rotateFirstPerson_OX (const float angle);
			void rotateFirstPerson_OY (const float angle);
			void rotateFirstPerson_OZ (const float angle);
			
			void rotateThirdPerson_OX (const float angle);
			void rotateThirdPerson_OY (const float angle);
			void rotateThirdPerson_OZ (const float angle);

			void SetPerspective  (const float fieldOfViewY, const float aspectRatio, const float zNear, const float zFar);
			void SetOrthographic (float left, float right, float bottom, float top, float zNear, float zFar);

			const  glm::mat4 getViewMatrix       ();
			const  glm::mat4 getProjectionMatrix ();
			const  glm::vec3 getTargetPosition   ();
			const  glm::vec3 getEyePosition      ();
            
            const  glm::vec3 getForward ();
			const  glm::vec3 getRight   ();
			const  glm::vec3 getUp      ();

        public:
            CameraType type;
            Projection projection_information;

			float zoomspeed;
			float movespeed;
			float sensivityOX;
			float sensivityOY;

		private:
			glm::vec3 position, forward, right, up;
            glm::mat4 projectionMatrix;
			float distancetotarget;
		
        };
}