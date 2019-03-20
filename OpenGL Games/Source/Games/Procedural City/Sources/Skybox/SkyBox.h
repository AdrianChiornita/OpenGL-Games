#pragma once

#include <Core/Engine.h>
#include <stb/stb_image.h>
#include <iostream>

class SkyBox {
	private:
		GLuint textureID = 0;
		std::string textureEXT;
		std::string tex_location;
		Shader* shader;
		GLuint VBO = 0, VAO = 0, IBO = 0;

		std::string shd_location;
		std::string vertex_shd_name;
		std::string fragment_shd_name;
		

		void LoadSkyBoxTex();

	public:
		SkyBox(
			std::string textureEXT, 
			std::string tex_location,
			std::string shd_location, 
			std::string vertex_shd_name, 
			std::string fragment_shd_name
		);

		void Init();
		void Render(glm::mat4 View, glm::mat4 Projection, glm::vec3 eye);
};