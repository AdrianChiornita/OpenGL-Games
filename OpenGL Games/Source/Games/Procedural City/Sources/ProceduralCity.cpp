#include "ProceduralCity.h"

ProceduralCity::ProceduralCity::ProceduralCity() {}

ProceduralCity::ProceduralCity::~ProceduralCity() {}

void  ProceduralCity::ProceduralCity::Init()
{
	{
		const std::string texlocation = "Source/Games/Procedural City/Textures/";
		camera = new CityCamera::Camera();

		skybox = new SkyBox(".jpg", 
			"Source/Games/Procedural City/Textures/skyboxtex/", 
			"Source/Games/Procedural City/Shaders/SkyboxShaders/", 
			"SkyboxVertexShader.VS.glsl", 
			"SkyboxFragmentShader.FS.glsl"
		);
		skybox->Init();

		street_generator = new StreetGenerator();
		street_generator->generate_street_meshes(glm::vec2(0.5f, 0.5f), glm::vec2(32.5f, 32.5f));

		{
			Material material;
			material.texture = new Texture2D();
			material.texture->Load2D((texlocation + "street.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.08f;
			material.Ka = 0.3f;
			material.Kd = 0.2f;
			material.Ks = 0.3f;
			material.shininess = 40;
			materialmap["street"] = material;
		}

		{
			Material material;
			material.texture = new Texture2D();
			material.texture->Load2D((texlocation + "grass.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.3f;
			material.Kd = 0.2f;
			material.Ks = 0.2f;
			material.shininess = 10;
			materialmap["grass"] = material;
		}

		{
			Material material;
			material.texture = new Texture2D();
			material.texture->Load2D((texlocation + "roof.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.2f;
			material.Kd = 0.2f;
			material.Ks = 0.2f;
			material.shininess = 10;
			materialmap["roof"] = material;
		}

		{
			Material material;
			material.texture = new Texture2D();
			material.texture->Load2D((texlocation + "window.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.2f;
			material.Kd = 0.2f;
			material.Ks = 0.3f;
			material.shininess = 20;
			materialmap["window"] = material;
		}
		{
			Material material;
			material.texture = new Texture2D();
			material.texture->Load2D((texlocation + "intersection.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.08f;
			material.Ka = 0.3f;
			material.Kd = 0.2f;
			material.Ks = 0.3f;
			material.shininess = 40;
			materialmap["intersection"] = material;
		}

		{
			Material material;
			material.texture = new Texture2D();
			material.texture->Load2D((texlocation + "sidewalk.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.3f;
			material.Kd = 0.2f;
			material.Ks = 0.3f;
			material.shininess = 20;
			materialmap["sidewalk"] = material;
		}

		// light parameters
		{
			light.position	= glm::vec3(16.25f, 18, 16.25f);
			light.direction = glm::vec3(0, -1, 0);
			light.emisive	= glm::vec3(0.2f, 0.2f, 0.2f);
			light.ambient	= glm::vec3(0.2f, 0.2f, 0.2f);
			light.diffuse	= glm::vec3(0.2f, 0.2f, 0.2f);
			light.specular	= glm::vec3(1, 1, 1);


			spots[0].direction = glm::vec3(0, 1, 0);
			spots[0].emisive = glm::vec3(1, 1, 1);
			spots[0].ambient = glm::vec3(1, 1, 1);
			spots[0].diffuse = glm::vec3(1, 1, 1);
			spots[0].specular = glm::vec3(1, 1, 1);

			spots[1].direction = glm::vec3(0, 1, 0);
			spots[1].emisive = glm::vec3(1, 1, 1);
			spots[1].ambient = glm::vec3(1, 1, 1);
			spots[1].diffuse = glm::vec3(1, 1, 1);
			spots[1].specular = glm::vec3(1, 1, 1);

			spots[2].direction = glm::vec3(0, 1, 0);
			spots[2].emisive = glm::vec3(1, 1, 1);
			spots[2].ambient = glm::vec3(1, 1, 1);
			spots[2].diffuse = glm::vec3(1, 1, 1);
			spots[2].specular = glm::vec3(1, 1, 1);

			spots[3].direction = glm::vec3(0, 1, 0);
			spots[3].emisive = glm::vec3(1, 1, 1);
			spots[3].ambient = glm::vec3(1, 1, 1);
			spots[3].diffuse = glm::vec3(1, 1, 1);
			spots[3].specular = glm::vec3(1, 1, 1);
		}

		{
			Shader* shader = new Shader("ObjectShader");
			shader->AddShader("Source/Games/Procedural City/Shaders/ObjectShaders/VertexShader.VS.glsl", GL_VERTEX_SHADER);
			shader->AddShader("Source/Games/Procedural City/Shaders/ObjectShaders/FragmentShader.FS.glsl", GL_FRAGMENT_SHADER);

			shader->CreateAndLink();
			shaders[shader->GetName()] = shader;
		}

		{
			Shader* shader = new Shader("StreetShader");
			shader->AddShader("Source/Games/Procedural City/Shaders/StreetShaders/VertexShader.VS.glsl", GL_VERTEX_SHADER);
			shader->AddShader("Source/Games/Procedural City/Shaders/StreetShaders/FragmentShader.FS.glsl", GL_FRAGMENT_SHADER);

			shader->CreateAndLink();
			shaders[shader->GetName()] = shader;
		}
	}
}

void ProceduralCity::ProceduralCity::RenderMesh(const Mesh * mesh, const Material material, const Shader * shader, const glm::mat4 & modelMatrix, bool isbuilding)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	shader->Use();

	glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	if (isbuilding) {
		glUniform3fv(glGetUniformLocation(shader->program, "spots[0].position"), 1, glm::value_ptr(spots[0].position));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[0].direction"), 1, glm::value_ptr(spots[0].direction));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[0].emisive_color"), 1, glm::value_ptr(spots[0].emisive));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[0].ambient_color"), 1, glm::value_ptr(spots[0].ambient));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[0].diffuse_color"), 1, glm::value_ptr(spots[0].diffuse));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[0].specular_color"), 1, glm::value_ptr(spots[0].specular));
			
		glUniform3fv(glGetUniformLocation(shader->program, "spots[1].position"), 1, glm::value_ptr(spots[1].position));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[1].direction"), 1, glm::value_ptr(spots[1].direction));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[1].emisive_color"), 1, glm::value_ptr(spots[1].emisive));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[1].ambient_color"), 1, glm::value_ptr(spots[1].ambient));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[1].diffuse_color"), 1, glm::value_ptr(spots[1].diffuse));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[1].specular_color"), 1, glm::value_ptr(spots[1].specular));

		glUniform3fv(glGetUniformLocation(shader->program, "spots[2].position"), 1, glm::value_ptr(spots[2].position));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[2].direction"), 1, glm::value_ptr(spots[2].direction));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[2].emisive_color"), 1, glm::value_ptr(spots[2].emisive));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[2].ambient_color"), 1, glm::value_ptr(spots[2].ambient));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[2].diffuse_color"), 1, glm::value_ptr(spots[2].diffuse));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[2].specular_color"), 1, glm::value_ptr(spots[2].specular));

		glUniform3fv(glGetUniformLocation(shader->program, "spots[3].position"), 1, glm::value_ptr(spots[3].position));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[3].direction"), 1, glm::value_ptr(spots[3].direction));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[3].emisive_color"), 1, glm::value_ptr(spots[3].emisive));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[3].ambient_color"), 1, glm::value_ptr(spots[3].ambient));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[3].diffuse_color"), 1, glm::value_ptr(spots[3].diffuse));
		glUniform3fv(glGetUniformLocation(shader->program, "spots[3].specular_color"), 1, glm::value_ptr(spots[3].specular));
	}

	glUniform3fv(glGetUniformLocation(shader->program, "light.position"), 1, glm::value_ptr(light.position));
	glUniform3fv(glGetUniformLocation(shader->program, "light.direction"), 1, glm::value_ptr(light.direction));
	glUniform3fv(glGetUniformLocation(shader->program, "light.emisive_color"), 1, glm::value_ptr(light.emisive));
	glUniform3fv(glGetUniformLocation(shader->program, "light.ambient_color"), 1, glm::value_ptr(light.ambient));
	glUniform3fv(glGetUniformLocation(shader->program, "light.diffuse_color"), 1, glm::value_ptr(light.diffuse));
	glUniform3fv(glGetUniformLocation(shader->program, "light.specular_color"), 1, glm::value_ptr(light.specular));
	
	

	glUniform1f(glGetUniformLocation(shader->program, "material.Ke"), material.Ke);
	glUniform1f(glGetUniformLocation(shader->program, "material.Ka"), material.Ka);
	glUniform1f(glGetUniformLocation(shader->program, "material.Kd"), material.Kd);
	glUniform1f(glGetUniformLocation(shader->program, "material.Ks"), material.Ks);
	glUniform1i(glGetUniformLocation(shader->program, "material.shininess"), material.shininess);

	glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(camera->getEyePosition()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.texture->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "texturemap"), 0);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void  ProceduralCity::ProceduralCity::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);	
}

void  ProceduralCity::ProceduralCity::Update(float deltaTimeSeconds)
{
	for (int i = 0; i < street_generator->street_meshes.size(); ++i) {
		if (std::strcmp(street_generator->street_meshes[i]->GetMeshID(), "intersection") == 0)
			RenderMesh(street_generator->street_meshes[i], materialmap["intersection"], shaders["StreetShader"], glm::mat4(1));
		else
			RenderMesh(street_generator->street_meshes[i], materialmap["street"], shaders["StreetShader"], glm::mat4(1));
	}

	for (int i = 0; i < street_generator->sidewalk_meshes.size(); ++i)
		RenderMesh(street_generator->sidewalk_meshes[i], materialmap["sidewalk"], shaders["StreetShader"], glm::mat4(1));

	for (int i = 0; i < street_generator->building_meshes.size(); ++i)
	{
		spots[0].position = glm::vec3(
			street_generator->building_meshes[i]->footprint.first.x,
			street_generator->building_meshes[i]->footprint.first.y,
			street_generator->building_meshes[i]->footprint.first.z
		);

		spots[1].position = glm::vec3(
			street_generator->building_meshes[i]->footprint.first.x,
			street_generator->building_meshes[i]->footprint.first.y,
			street_generator->building_meshes[i]->footprint.second.z
		);

		spots[2].position = glm::vec3(
			street_generator->building_meshes[i]->footprint.second.x,
			street_generator->building_meshes[i]->footprint.first.y,
			street_generator->building_meshes[i]->footprint.first.z
		);

		spots[3].position = glm::vec3(
			street_generator->building_meshes[i]->footprint.second.x,
			street_generator->building_meshes[i]->footprint.first.y,
			street_generator->building_meshes[i]->footprint.second.z
		);


		for (int j = 0; j < street_generator->building_meshes[i]->mesh.size(); ++j) 
			if (std::strcmp(street_generator->building_meshes[i]->mesh[j]->GetMeshID(), "top") == 0)
				RenderMesh(street_generator->building_meshes[i]->mesh[j], materialmap["roof"], shaders["ObjectShader"], glm::mat4(1), true);
			else if (std::strcmp(street_generator->building_meshes[i]->mesh[j]->GetMeshID(), "park") == 0)
				RenderMesh(street_generator->building_meshes[i]->mesh[j], materialmap["grass"], shaders["StreetShader"], glm::mat4(1));
			else
				RenderMesh(street_generator->building_meshes[i]->mesh[j], materialmap["window"], shaders["ObjectShader"], glm::mat4(1), true);
	}
}

void  ProceduralCity::ProceduralCity::FrameEnd()
{
	skybox->Render(camera->getViewMatrix(), camera->getProjectionMatrix(), camera->getEyePosition());
}

void  ProceduralCity::ProceduralCity::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_1))
		camera->SetPerspective(RADIANS(80), 1280.0f / 720.0f, 0.01f, 200.0f);

	if (window->KeyHold(GLFW_KEY_V) && camera->projection_information.fieldOfViewY < RADIANS(90))
		camera->SetPerspective(
			camera->projection_information.fieldOfViewY + RADIANS(camera->zoomspeed),
			camera->projection_information.aspectRatio,
			camera->projection_information.zNear,
			camera->projection_information.zFar);

	if (window->KeyHold(GLFW_KEY_B) && camera->projection_information.fieldOfViewY > RADIANS(40))
		camera->SetPerspective(
			camera->projection_information.fieldOfViewY - RADIANS(camera->zoomspeed),
			camera->projection_information.aspectRatio,
			camera->projection_information.zNear,
			camera->projection_information.zFar);

	if (window->KeyHold(GLFW_KEY_W)) camera->translateForward(deltaTime * camera->movespeed);
	if (window->KeyHold(GLFW_KEY_A)) camera->translateRight(-1 * deltaTime * camera->movespeed);
	if (window->KeyHold(GLFW_KEY_S)) camera->translateForward(-1 * deltaTime * camera->movespeed);
	if (window->KeyHold(GLFW_KEY_D)) camera->translateRight(deltaTime * camera->movespeed);
	if (window->KeyHold(GLFW_KEY_Q)) camera->translateUpword(deltaTime * camera->movespeed);
	if (window->KeyHold(GLFW_KEY_E)) camera->translateUpword(-1 * deltaTime * camera->movespeed);
}

void  ProceduralCity::ProceduralCity::OnKeyPress(int key, int mods)
{
}

void  ProceduralCity::ProceduralCity::OnKeyRelease(int key, int mods)
{
}

void  ProceduralCity::ProceduralCity::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (camera->type == CityCamera::FirstPerson) {
			camera->rotateFirstPerson_OX(-1 * deltaY * camera->sensivityOX);
			camera->rotateFirstPerson_OY(-1 * deltaX * camera->sensivityOY);
		}
		if (camera->type == CityCamera::ThirdPerson) {
			camera->rotateFirstPerson_OX(-1 * deltaY * camera->sensivityOX);
			camera->rotateThirdPerson_OY(-1 * deltaX * camera->sensivityOY);
		}
	}
}

void  ProceduralCity::ProceduralCity::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void  ProceduralCity::ProceduralCity::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void  ProceduralCity::ProceduralCity::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void  ProceduralCity::ProceduralCity::OnWindowResize(int width, int height)
{
}