#include "SkyBox.h"

void SkyBox::LoadSkyBoxTex()
{
	std::vector<std::string> faces = {"front", "back", "top", "bottom", "right", "left"};

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, channels;
	unsigned char* data;

	for (unsigned int index = 0; index < faces.size(); ++index) {
		data = stbi_load((tex_location + faces[index] + textureEXT).c_str(), &width, &height, &channels, 0);
		if (data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
				0,
				GL_RGB8,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

SkyBox::SkyBox(std::string textureEXT, std::string tex_location, std::string shd_location, std::string vertex_shd_name, std::string fragment_shd_name)
{
	this->textureEXT = textureEXT;
	this->tex_location = tex_location;
	this->shd_location = shd_location;
	this->vertex_shd_name = vertex_shd_name;
	this->fragment_shd_name = fragment_shd_name;
}

void SkyBox::Init()
{
	shader = new Shader("SkyBoxShader");

	shader->AddShader(shd_location + vertex_shd_name, GL_VERTEX_SHADER);
	shader->AddShader(shd_location + fragment_shd_name, GL_FRAGMENT_SHADER);
	shader->CreateAndLink();

	LoadSkyBoxTex();

	glm::vec3 vertices[] = {
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f)
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 432, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
	
	glBindVertexArray(0);
	CheckOpenGLError();
}

void SkyBox::Render(glm::mat4 View, glm::mat4 Projection, glm::vec3 eye)
{
	glDepthFunc(GL_LEQUAL);
	
	shader->Use();
	
	glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniform3fv(shader->GetUniformLocation("eye_position"), 1, glm::value_ptr(eye));
	
	glBindVertexArray(VAO);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDepthFunc(GL_LESS);
}
