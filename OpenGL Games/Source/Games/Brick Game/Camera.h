#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace BrickGameCamera
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(0, 0, 0);
				forward = glm::vec3(0, 0, 1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 2;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera()
			{ }

			// Update camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
				// movement will keep the camera at the same height always
				// Example: If you rotate up/down your head and walk forward you will still keep the same relative distance (height) to the ground!
				// Translate the camera using the DIR vector computed from forward
				this->position += dir * distance;
			}

			void TranslateForward(float distance)
			{
				this->position += this->forward * distance;
			}

			void TranslateUpword(float distance)
			{
				this->position += glm::vec3(0, 1, 0) * distance;
			}

			void TranslateRight(float distance)
			{
				// Translate the camera using the "right" vector
				// Usually translation using camera "right' is not very useful because if the camera is rotated around the "forward" vector 
				// translation over the right direction will have an undesired effect; the camera will get closer or farther from the ground
				// Using the projected right vector (onto the ground plane) makes more sense because we will keep the same distance from the ground plane
				this->position += this->right * distance;
			}

			void RotateFirstPerson_OX(float angle)
			{
				glm::vec4 forward = glm::rotate(glm::mat4(1.0f), angle, this->right) * glm::vec4(this->forward, 1.0f);
				this->forward = glm::normalize(glm::vec3(forward));

				glm::vec4 up = glm::rotate(glm::mat4(1.0f), angle, this->right) *  glm::vec4(this->up, 1.0f);
				this->up = glm::normalize(glm::vec3(up));
			}

			void RotateFirstPerson_OY(float angle)
			{
				glm::vec4 forward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(this->forward, 1.0f);
				this->forward = glm::normalize(glm::vec3(forward));

				glm::vec4 up = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) *  glm::vec4(this->up, 1.0f);
				this->up = glm::normalize(glm::vec3(up));

				glm::vec4 right = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) *  glm::vec4(this->right, 1.0f);
				this->right = glm::normalize(glm::vec3(right));
			}

			void RotateFirstPerson_OZ(float angle)
			{
				glm::vec4 up = glm::rotate(glm::mat4(1.0f), angle, this->forward) *  glm::vec4(this->up, 1.0f);
				this->up = glm::normalize(glm::vec3(up));

				glm::vec4 right = glm::rotate(glm::mat4(1.0f), angle, this->forward) *  glm::vec4(this->right, 1.0f);
				this->right = glm::normalize(glm::vec3(right));
			}

			void RotateThirdPerson_OX(float angle)
			{
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OX(angle);
				TranslateForward(-1 * distanceToTarget);
			}

			void RotateThirdPerson_OY(float angle)
			{
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OY(angle);
				TranslateForward(-1 * distanceToTarget);
			}

			void RotateThirdPerson_OZ(float angle)
			{
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OZ(angle);
				TranslateForward(-1 * distanceToTarget);
			}

			glm::mat4 GetViewMatrix()
			{
				// Returns the View Matrix
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}