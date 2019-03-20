#include "Utilities.h"

using namespace glm;
using namespace std;

Mesh * PoolGameUtilities::CreateMesh(const char * name, const vector<VertexFormat>& vertices, const vector<unsigned short>& indices, const unsigned int primitive)
{
	unsigned int VAO = 0, VBO = 0, IBO = 0;

	// Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Create the IBO and bind it
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);
	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));
	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));
	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh* mesh = new Mesh(name);
	mesh->SetDrawMode(primitive);
	mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	mesh->vertices = vertices;
	mesh->indices = indices;
	return mesh;
}

PoolGameUtilities::Camera::Camera(const vec3 &position, const vec3 center, const vec3 &up)
{
	Camera::set(position, center, up);
}

PoolGameUtilities::Camera::Camera()
{
	type = FirstPerson;

	position		 = vec3(-1.5, 5, 6.5);
	previous_position = position;
	forward			 = vec3(1, 0, 0);
	up				 = vec3(0, 1, 0);
	right			 = vec3(0, 0, 1);
	distancetotarget = length(vec3(2, 2.5, 6.5) - position);
	movespeed		 = 2.0f;
	zoomspeed		 = 1.0f;
	projectionMatrix = perspective(RADIANS(80), 1280.0f / 720.0f, 0.01f, 200.0f);

	projectioninformation.fieldOfViewY = RADIANS(80);
	projectioninformation.aspectRatio  = 1280.0f / 720.0f;
	projectioninformation.zNear        = 0.01f;
	projectioninformation.zFar         = 200.0f;
	projectioninformation.left		   = 0.0f;
	projectioninformation.right		   = 0.0f;
	projectioninformation.bottom	   = 0.0f;
	projectioninformation.top		   = 0.0f;

	sensivityOX = 0.001f;
	sensivityOY = 0.001f;
}

PoolGameUtilities::Camera::~Camera()
{
}

void PoolGameUtilities::Camera::set(const vec3 &position, const vec3 center, const vec3 &up)
{
	this->position = position;
	this->forward = normalize(center - position);
	this->right = cross(forward, up);
	this->up = cross(right, forward);
	distancetotarget = length(center - position);
}

void PoolGameUtilities::Camera::moveForward(const float distance)
{
	vec3 direction = normalize(vec3(forward.x, 0, forward.z));
	this->position += direction * distance;
}

void PoolGameUtilities::Camera::translateForward(const float distance)
{
	this->position += this->forward * distance;
}

void PoolGameUtilities::Camera::translateUpword(const float distance)
{
	this->position += vec3(0, 1, 0) * distance;
}

void PoolGameUtilities::Camera::translateRight(const float distance)
{
	this->position += this->right * distance;
}

void PoolGameUtilities::Camera::rotateFirstPerson_OX(const float angle)
{
	this->forward = normalize(vec3(rotate(mat4(1.0f), angle, this->right) * vec4(this->forward, 1.0f)));
	this->up	  = normalize(vec3(rotate(mat4(1.0f), angle, this->right) * vec4(this->up, 1.0f)));
}

void PoolGameUtilities::Camera::rotateFirstPerson_OY(const float angle)
{
	this->forward = normalize(vec3(rotate(mat4(1.0f), angle, vec3(0, 1, 0)) * vec4(this->forward, 1.0f)));
	this->up	  = normalize(vec3(rotate(mat4(1.0f), angle, vec3(0, 1, 0)) * vec4(this->up, 1.0f)));
	this->right	  = normalize(vec3(rotate(mat4(1.0f), angle, vec3(0, 1, 0)) * vec4(this->right, 1.0f)));
}

void PoolGameUtilities::Camera::rotateFirstPerson_OZ(const float angle)
{
	this->up    = normalize(vec3(rotate(mat4(1.0f), angle, this->forward) * vec4(this->up, 1.0f)));
	this->right = normalize(vec3(rotate(mat4(1.0f), angle, this->forward) * vec4(this->right, 1.0f)));
}

void PoolGameUtilities::Camera::rotateThirdPerson_OX(const float angle)
{
	translateForward(distancetotarget);
	rotateFirstPerson_OX(angle);
	translateForward(-1 * distancetotarget);
}

void PoolGameUtilities::Camera::rotateThirdPerson_OY(const float angle)
{
	translateForward(distancetotarget);
	rotateFirstPerson_OY(angle);
	translateForward(-1 * distancetotarget);

}

void PoolGameUtilities::Camera::rotateThirdPerson_OZ(const float angle)
{
	translateForward(distancetotarget);
	rotateFirstPerson_OZ(angle);
	translateForward(-1 * distancetotarget);
}

void PoolGameUtilities::Camera::SetPerspective(const float fieldOfViewY, const float aspectRatio, const float zNear, const float zFar)
{
	projectionMatrix = perspective(fieldOfViewY, aspectRatio, zNear, zFar);

	projectioninformation.fieldOfViewY = fieldOfViewY;
	projectioninformation.aspectRatio  = aspectRatio;
	projectioninformation.zNear		   = zNear;
	projectioninformation.zFar		   = zFar;
	projectioninformation.left		   = 0.0f;
	projectioninformation.right		   = 0.0f;
	projectioninformation.bottom	   = 0.0f;
	projectioninformation.top		   = 0.0f;
}

void PoolGameUtilities::Camera::SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionMatrix = ortho(left, right, bottom, top, zNear, zFar);
	
	projectioninformation.fieldOfViewY = 0.0f;
	projectioninformation.aspectRatio  = 0.0f;
	projectioninformation.left		   =left;
	projectioninformation.right		   = right;
	projectioninformation.bottom	   = bottom;
	projectioninformation.top		   = top;
	projectioninformation.zNear		   = zNear;
	projectioninformation.zFar		   = zFar;
}

const mat4 PoolGameUtilities::Camera::getViewMatrix()
{
	return lookAt(position, position + forward, up);
}

const mat4 PoolGameUtilities::Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

const vec3 PoolGameUtilities::Camera::getTargetPosition()
{
	return position + forward * distancetotarget;
}

const vec3 PoolGameUtilities::Camera::getEyePosition()
{
	return this->position;
}

void PoolGameUtilities::Camera::setToPreviousEyePosition()
{
	this->position = this->previous_position;
}

const vec3 PoolGameUtilities::Camera::getPreviousEyePosition()
{
	return this->previous_position;
}

void PoolGameUtilities::Camera::setPreviousEyePosition(vec3 previous_position)
{
	this->previous_position = previous_position;
}

const vec3  PoolGameUtilities::Camera::getForward()
{
	return this->forward;
}

const vec3 PoolGameUtilities::Camera::getRight()
{
	return this->right;
}

const vec3 PoolGameUtilities::Camera::getUp()
{
	return this->up;
}