#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <iostream>

#include "Camera/CityCamera.h"
#include "Skybox/SkyBox.h"
#include "DataGeneration/StreetGeneration.h"

namespace ProceduralCity {

	typedef struct Material {
		float Ke;
		float Ka;
		float Kd;
		float Ks;
		int shininess;
		Texture2D* texture;
	}Material;

	typedef struct Light {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emisive;
		glm::vec3 direction;
	}Light;

	class ProceduralCity : public SimpleScene
	{
	public:
		ProceduralCity();
		~ProceduralCity();

		void Init() override;
		void RenderMesh(const Mesh* mesh, const Material material, const Shader* shader, const glm::mat4 & modelMatrix, bool isbuilding = false);

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

	protected:
		CityCamera::Camera *camera;
		SkyBox* skybox;
		Light light;
		Light spots[4];

		std::unordered_map<std::string, Material> materialmap;

		StreetGenerator* street_generator;

	};
}