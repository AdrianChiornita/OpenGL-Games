#include "CityCamera.h"

CityCamera::Camera::Camera(const glm::vec3 &position, const glm::vec3 center, const glm::vec3 &up)
{
	Camera::set(position, center, up);
}

CityCamera::Camera::Camera()
{
	type = FirstPerson;

	position = glm::vec3(0.5, 3.5, 16.5);
	forward = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(0, 0, 1);
	distancetotarget = length(glm::vec3(2, 2.5, 6.5) - position);
	movespeed = 2.0f;
	zoomspeed = 1.0f;
	projectionMatrix = glm::perspective(RADIANS(70.0f), 1280.0f / 720.0f, 0.01f, 200.0f);

	projection_information.fieldOfViewY = RADIANS(70.0f);
	projection_information.aspectRatio = 1280.0f / 720.0f;
	projection_information.zNear = 0.01f;
	projection_information.zFar = 200.0f;
	projection_information.left = 0.0f;
	projection_information.right = 0.0f;
	projection_information.bottom = 0.0f;
	projection_information.top = 0.0f;

	sensivityOX = 0.001f;
	sensivityOY = 0.001f;
}

CityCamera::Camera::~Camera()
{
}

void CityCamera::Camera::set(const glm::vec3 &position, const glm::vec3 center, const glm::vec3 &up)
{
	this->position = position;
	this->forward = glm::normalize(center - position);
	this->right = glm::cross(forward, up);
	this->up = glm::cross(right, forward);
	distancetotarget = glm::length(center - position);
}

void CityCamera::Camera::moveForward(const float distance)
{
	glm::vec3 direction = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	this->position += direction * distance;
}

void CityCamera::Camera::translateForward(const float distance)
{
	this->position += this->forward * distance;
}

void CityCamera::Camera::translateUpword(const float distance)
{
	this->position += glm::vec3(0, 1, 0) * distance;
}

void CityCamera::Camera::translateRight(const float distance)
{
	this->position += this->right * distance;
}

void CityCamera::Camera::rotateFirstPerson_OX(const float angle)
{
	this->forward = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				this->right) 
			* glm::vec4(
				this->forward, 
				1.0f)
		)
	);
	this->up = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				this->right) 
			* glm::vec4(
				this->up, 
				1.0f)
		)
	);
}

void CityCamera::Camera::rotateFirstPerson_OY(const float angle)
{
	this->forward = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				glm::vec3(0, 1, 0)) 
			* glm::vec4(
				this->forward, 
				1.0f)
		)
	);
	this->up = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				glm::vec3(0, 1, 0)) 
			* glm::vec4(
				this->up, 
				1.0f)
		)
	);
	this->right = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				glm::vec3(0, 1, 0)) 
			* glm::vec4(
				this->right, 
				1.0f)
		)
	);
}

void CityCamera::Camera::rotateFirstPerson_OZ(const float angle)
{
	this->up = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				this->forward) 
			* glm::vec4(
				this->up, 
				1.0f)
		)
	);
	this->right = normalize(
		glm::vec3(
			glm::rotate(
				glm::mat4(1.0f), 
				angle, 
				this->forward) 
			* glm::vec4(
				this->right, 
				1.0f)
		)
	);
}

void CityCamera::Camera::rotateThirdPerson_OX(const float angle)
{
	translateForward(distancetotarget);
	rotateFirstPerson_OX(angle);
	translateForward(-1 * distancetotarget);
}

void CityCamera::Camera::rotateThirdPerson_OY(const float angle)
{
	translateForward(distancetotarget);
	rotateFirstPerson_OY(angle);
	translateForward(-1 * distancetotarget);

}

void CityCamera::Camera::rotateThirdPerson_OZ(const float angle)
{
	translateForward(distancetotarget);
	rotateFirstPerson_OZ(angle);
	translateForward(-1 * distancetotarget);
}

void CityCamera::Camera::SetPerspective(const float fieldOfViewY, const float aspectRatio, const float zNear, const float zFar)
{
	projectionMatrix = glm::perspective(fieldOfViewY, aspectRatio, zNear, zFar);

	projection_information.fieldOfViewY = fieldOfViewY;
	projection_information.aspectRatio = aspectRatio;
	projection_information.zNear = zNear;
	projection_information.zFar = zFar;
	projection_information.left = 0.0f;
	projection_information.right = 0.0f;
	projection_information.bottom = 0.0f;
	projection_information.top = 0.0f;
}

void CityCamera::Camera::SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

	projection_information.fieldOfViewY = 0.0f;
	projection_information.aspectRatio = 0.0f;
	projection_information.left = left;
	projection_information.right = right;
	projection_information.bottom = bottom;
	projection_information.top = top;
	projection_information.zNear = zNear;
	projection_information.zFar = zFar;
}

const glm::mat4 CityCamera::Camera::getViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

const glm::mat4 CityCamera::Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

const glm::vec3 CityCamera::Camera::getTargetPosition()
{
	return position + forward * distancetotarget;
}

const glm::vec3 CityCamera::Camera::getEyePosition()
{
	return this->position;
}

const glm::vec3  CityCamera::Camera::getForward()
{
	return this->forward;
}

const glm::vec3 CityCamera::Camera::getRight()
{
	return this->right;
}

const glm::vec3 CityCamera::Camera::getUp()
{
	return this->up;
}