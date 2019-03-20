 #include "PoolGame.h"

using namespace glm;
using namespace std;

using namespace PoolGameUtilities;
using namespace Objects3D;

PoolGame::PoolGame()
{
}

PoolGame::~PoolGame()
{
}

void PoolGame::Init()
{
	const string texturesLocation = "Source/Games/8 Ball Pool/Textures/";

	// init control
	{
		control.translate_cue = false;
		control.draw_triangle = true;
		control.start_game = false;
		control.draw_cue = false;
		control.shoot = false;
		control.moving_scene = false;
		control.shoot_try = false;
		control.fault = false;
		control.prepare_shot = false;
		control.calculate_stats = false;
		control.switch_players = false;
		control.helpline = false;
		control.first = emptyball;
		control.futurewhite = vec3(0);
		control.yellowcount = 0;
		control.redcount = 0;
	}
	
	// use pool shader
	{
		camera = new Camera();

		Shader *shader = new Shader("8BallPoolShader");
		
		shader->AddShader("Source/Games/8 Ball Pool/Shaders/VertexShader.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Games/8 Ball Pool/Shaders/FragmentShader.FS.glsl", GL_FRAGMENT_SHADER);
		
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// load textures and materials
	{
		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "lamptex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 1.0f;
			material.Kd = 1.0f;
			material.Ks = 1.0f;
			material.shininess = 70;
			materialmap["lamp"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "woodtex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.7f;
			material.Kd = 0.7f;
			material.Ks = 0.9f;
			material.shininess = 70;
			materialmap["wood"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "cuetex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.7f;
			material.Kd = 0.7f;
			material.Ks = 0.9f;
			material.shininess = 70;
			materialmap["cue"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "carpettex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.05f;
			material.Ka = 0.5f;
			material.Kd = 0.6f;
			material.Ks = 0.7f;
			material.shininess = 30;
			materialmap["carpet"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "walltex.png").c_str(), GL_REPEAT);
			material.Ke = 0.01f;
			material.Ka = 0.5f;
			material.Kd = 0.6f;
			material.Ks = 0.8f;
			material.shininess = 50;
			materialmap["walls"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "upwalltex.png").c_str(), GL_REPEAT);
			material.Ke = 0.1f;
			material.Ka = 0.8f;
			material.Kd = 0.8f;
			material.Ks = 1.0f;
			material.shininess = 40;
			materialmap["ceiling"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "steeltex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.1f;
			material.Ka = 0.8f;
			material.Kd = 0.8f;
			material.Ks = 1.0f;
			material.shininess = 80;
			materialmap["steel"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "cueballtex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.1f;
			material.Ka = 0.8f;
			material.Kd = 0.8f;
			material.Ks = 1.0f;
			material.shininess = 40;
			materialmap["whiteball"] = material;
		}

		{
			for (int i = 0; i < 7; ++i)
			{
				Material3D material;
				material.texture = new Texture2D();
				material.texture->Load2D((texturesLocation + "redballtex" + to_string(i) + ".jpg").c_str(), GL_REPEAT);
				material.Ke = 0.1f;
				material.Ka = 0.8f;
				material.Kd = 0.8f;
				material.Ks = 1.0f;
				material.shininess = 40;
				materialmap["redball" + to_string(i)] = material;
			}

			for (int i = 0; i < 7; ++i)
			{
				Material3D material;
				material.texture = new Texture2D();
				material.texture->Load2D((texturesLocation + "yellowballtex" + to_string(i) + ".jpg").c_str(), GL_REPEAT);
				material.Ke = 0.1f;
				material.Ka = 0.8f;
				material.Kd = 0.8f;
				material.Ks = 1.0f;
				material.shininess = 40;
				materialmap["yellowball" + to_string(i)] = material;
			}
			
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "blackballtex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.8f;
			material.Kd = 0.8f;
			material.Ks = 1.0f;
			material.shininess = 40;
			materialmap["blackball"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "pooltabletex.png").c_str(), GL_REPEAT);
			material.Ke = 0.0f;
			material.Ka = 0.5f;
			material.Kd = 0.6f;
			material.Ks = 0.7f;
			material.shininess = 20;
			materialmap["pooltable"] = material;
		}

		{
			Material3D material;
			material.texture = new Texture2D();
			material.texture->Load2D((texturesLocation + "blacktex.jpg").c_str(), GL_REPEAT);
			material.Ke = 0;
			material.Ka = 0;
			material.Kd = 0;
			material.Ks = 0;
			material.shininess = 0;
			materialmap["black"] = material;
		}
	}

	// load pooltable, room and cue
	{
		gameTable = new PoolTable("pooltable");
		gameTable->CreateTableMeshes();
		gameTable->relative = vec3(6, gameTable->widthtable / 2, 7);

		cue = new CueObject("cue");
		cue->CreateMesh();

		room = O3D::CreateRoom("ROOM",
			vec3(
				gameTable->relative.x - 4,
				0.03f,
				gameTable->relative.z - 4 + gameTable->widthtable / 2 - gameTable->lengthtable / 2
			),
			vec3(
				gameTable->relative.x + 4 + gameTable->lengthtable,
				gameTable->lengthtable - 2,
				gameTable->relative.z + 4 + gameTable->widthtable / 2 + gameTable->lengthtable / 2
			));
		camera->set(
			vec3(
				gameTable->relative.x - 2,
				gameTable->lengthtable / 2,
				gameTable->relative.z - 2 + gameTable->widthtable / 2 - gameTable->lengthtable / 2
			),
			vec3(
				gameTable->relative.x + gameTable->lengthtable / 2,
				gameTable->relative.y,
				gameTable->relative.z + gameTable->widthtable / 2
			),
			vec3(0, 1, 0)
		);
	}

	// light parameters
	{
		light.position =
			vec3(
				gameTable->relative.x + gameTable->lengthtable / 2,
				gameTable->lengthtable - 2.8f,
				gameTable->relative.z + gameTable->widthtable / 2
			);

		light.lampmesh = new Mesh("Lamp");
		light.lampmesh->LoadMesh("Source/Games/8 Ball Pool/Objects/", "lamp.obj");

		light.direction = vec3(0, -1, 0);
		light.emisive_color = vec3(1, 1, 1);
		light.ambient_color = vec3(1, 1, 1);
		light.diffuse_color = vec3(1, 1, 1);
		light.specular_color = vec3(1, 1, 1);
	}

	// load balls
	{
		white = new BallObject("cueball", cueball);
		white->CreateMesh();
		white->mass = 0.24999f;
		allballs.push_back(white);
		
		black = new BallObject("gameball", matchball);
		black->CreateMesh();
		black->mass = 0.2505f;
		allballs.push_back(black);

		int contor = 0;
		float epsilonmass = 0.00001f;

		for (int index = 0; index < 7; ++index)
		{
			yellows.push_back(new BallObject("yellowball" + to_string(index), yellowball));
			yellows[index]->CreateMesh();
			yellows[index]->mass = 0.25f + contor * epsilonmass;
			allballs.push_back(yellows[index]);
			
			reds.push_back(new BallObject("redball" + to_string(index), redball));
			reds[index]->CreateMesh();
			reds[index]->mass = 0.25f + (contor + 1) * epsilonmass;
			allballs.push_back(reds[index]);
			
			contor += 2;
		}
	}
}

void PoolGame::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);
}

void PoolGame::Update(float deltaTimeSeconds)
{
	// start/restart game
	{
		if (control.draw_triangle) {
			RestartGame();
			control.translate_cue = false;
			control.start_game = false;
			control.draw_cue = false;
			control.shoot = false;
			control.moving_scene = false;
			control.shoot_try = false;
			control.fault = false;
			control.prepare_shot = false;
			control.calculate_stats = false;
			control.switch_players = false;
			control.helpline = false;
			control.first = emptyball;
			control.yellowcount = 0;
			control.redcount = 0;

			logic = new GameLogic();
			cout << endl << logic->firstplayer->name << " TURN" << endl;
			control.draw_triangle = false;
		}
	}

	// draw room
	{
		mat4 modelMatrix = mat4(1);
		RenderMesh(
			room[0],
			materialmap["carpet"],
			shaders["8BallPoolShader"],
			modelMatrix
		);
		RenderMesh(
			room[1],
			materialmap["walls"],
			shaders["8BallPoolShader"],
			modelMatrix
		);
		RenderMesh(
			room[2],
			materialmap["ceiling"],
			shaders["8BallPoolShader"],
			modelMatrix
		);
	}

	// draw table and lamp
	{
		mat4 modelMatrix = mat4(1);
		modelMatrix *= T3D::Translate(
			gameTable->relative.x,
			gameTable->relative.y,
			gameTable->relative.z
		);
		UpdateTable(deltaTimeSeconds, modelMatrix);

		modelMatrix = mat4(1);
		modelMatrix *= T3D::Translate(
			light.position.x,
			light.position.y,
			light.position.z
		);
		RenderMesh(
			light.lampmesh,
			materialmap["lamp"],
			shaders["8BallPoolShader"],
			modelMatrix
		);
	}

	//see if the scene is moving
	{
		control.moving_scene = false;
		for (BallObject* ball : allballs)
		{
			if (ball->moving)
			{
				control.moving_scene = true;
				break;
			}
		}
	}

	// update game
	{
		if (control.moving_scene)
		{
			control.calculate_stats = true;
			control.yellowcount = 0;
			control.redcount = 0;
			for (int index = 0; index < allballs.size(); ++index)
			{
				if (allballs[index]->isinpocket)
				{
					if (allballs[index]->type == yellowball) control.yellowcount++;
					if (allballs[index]->type == redball) control.redcount++;
					continue;
				}
				else
				{
					if (allballs[index]->isInPocket(
						gameTable->relative,
						gameTable->lengthtable,
						gameTable->widthtable)
						)
					{
						allballs[index]->isinpocket = true;
						allballs[index]->moving = false;

						
						if (control.first == emptyball &&
							allballs[index]->type != cueball &&
							allballs[index]->type != matchball)
							control.first = allballs[index]->type;
						continue;
					}
				}

				allballs[index]->Collisions(
					gameTable->relative,
					gameTable->lengthtable,
					gameTable->widthtable
				);

				allballs[index]->movement =
					allballs[index]->trajectory
					* allballs[index]->velocity
					* deltaTimeSeconds;

				for (int next = 0; next < allballs.size(); ++next)
				{
					if (allballs[next]->isinpocket) continue;

					if (allballs[next]->name != allballs[index]->name)
						if(allballs[index]->TreatBallCollision(allballs[next]))
						{
							if (allballs[index]->type == cueball)
								if (logic->getActivePlayer()->firsttouched == emptyball &&
									logic->getActivePlayer()->playercolor != emptyball)
									logic->getActivePlayer()->firsttouched
										= allballs[next]->type;
						};
				}

				allballs[index]->Update(deltaTimeSeconds);
			}
		}
	}

	// shoot 
	{
		if (control.shoot) {
			white->moving = true;

			white->trajectory = normalize(
				vec3(
					camera->getForward().x,
					0,
					camera->getForward().z)
			);

			white->velocity = (cue->k * deltaTimeSeconds * cue->distance) / white->mass;
			cue->distance = 0.0f;

			control.shoot = false;
		}
	}

	// make top-down camera
	{
		if (control.moving_scene && camera->type == ThirdPerson)
		{
			camera->type = FirstPerson;

			camera->set(
				vec3(
					gameTable->relative.x + gameTable->lengthtable / 2,
					(3 * gameTable->widthtable) / 2 - 0.8f,
					gameTable->relative.z + gameTable->widthtable / 2),
				vec3(
					gameTable->relative.x + gameTable->lengthtable / 2,
					gameTable->widthtable / 2 + O3D::border_height,
					gameTable->relative.z + gameTable->widthtable / 2),
				vec3(0, 0, 1));

			camera->SetPerspective(RADIANS(75.0f), 1280.0f / 720.0f, 0.01f, 200.0f);

			control.draw_cue = false;
			cue->rotate_angle = vec3(0);
		}
	}

	// draw cue
	{
		if (control.draw_cue)
		{
			mat4 modelMatrix = mat4(1);
			modelMatrix *= T3D::Translate(
				camera->getTargetPosition().x,
				camera->getTargetPosition().y,
				camera->getTargetPosition().z
			);
			modelMatrix *= T3D::RotateOY(cue->rotate_angle.y);
			modelMatrix *= T3D::RotateOZ(RADIANS(75.0f));
			modelMatrix *= T3D::Translate(0, 0.5f, 0);

			control.translate_cue = true;
			RenderMesh(
				cue->mesh,
				materialmap["cue"],
				shaders["8BallPoolShader"],
				modelMatrix
			);
			control.translate_cue = false;

			if (control.helpline)
			{
				cue->line = O3D::CreateLine(
					"LINE",
					white->translate_coordinates,
					calculateSecondPoint(deltaTimeSeconds)
				);
				RenderMesh(
					cue->line,
					materialmap["lamp"],
					shaders["8BallPoolShader"],
					mat4(1)
				);
			}
		}
	}

	// draw balls
	{
		mat4 modelMatrix;

		if (!white->isinpocket)
		{
			modelMatrix = mat4(1);
			modelMatrix = translate(modelMatrix, white->translate_coordinates);
			modelMatrix = rotate(modelMatrix, white->rotate_angle, white->rotate_axis);
			RenderMesh(
				white->mesh,
				materialmap["whiteball"],
				shaders["8BallPoolShader"],
				modelMatrix
			);
		}

		if (!black->isinpocket)
		{
			modelMatrix = mat4(1);
			modelMatrix = translate(modelMatrix, black->translate_coordinates);
			modelMatrix = rotate(modelMatrix, black->rotate_angle, black->rotate_axis);
			RenderMesh(
				black->mesh,
				materialmap["blackball"],
				shaders["8BallPoolShader"],
				modelMatrix
			);
		}

		
		for (int i = 0; i < reds.size(); ++i)
		{
			if (!reds[i]->isinpocket)
			{
				modelMatrix = mat4(1);
				modelMatrix = mat4(1);
				modelMatrix = translate(modelMatrix, reds[i]->translate_coordinates);
				modelMatrix = rotate(modelMatrix, reds[i]->rotate_angle, reds[i]->rotate_axis);
				RenderMesh(
					reds[i]->mesh,
					materialmap["redball" + to_string(i)],
					shaders["8BallPoolShader"],
					modelMatrix
				);
			}
		}

		for (int i = 0; i < yellows.size(); ++i)
		{
			if (!yellows[i]->isinpocket)
			{
				modelMatrix = mat4(1);
				modelMatrix = translate(modelMatrix, yellows[i]->translate_coordinates);
				modelMatrix = rotate(modelMatrix, yellows[i]->rotate_angle, yellows[i]->rotate_axis);
				RenderMesh(
					yellows[i]->mesh,
					materialmap["yellowball" + to_string(i)],
					shaders["8BallPoolShader"],
					modelMatrix
				);
			}
		}
	}
	
	// game stats
	{
		if (!control.moving_scene && control.calculate_stats)
		{
			// black in pocket
			if (black->isinpocket)
			{
				control.draw_triangle = true;

				camera->set(
					vec3(
						gameTable->relative.x - 3,
						gameTable->widthtable,
						gameTable->relative.z + gameTable->widthtable / 2
					),
					vec3(
						gameTable->relative.x + gameTable->lengthtable / 2,
						gameTable->widthtable / 2 + O3D::border_height,
						gameTable->relative.z + gameTable->widthtable / 2),
					vec3(0, 1, 0));
				camera->SetPerspective(RADIANS(80.0f), 1280.0f / 720.0f, 0.01f, 200.0f);

				if (logic->getActivePlayer()->inpocket == 7)
				{
					if (white->isinpocket)
					{
						cout << "Fault at game ball, the cue ball is in pocket" << endl;
						logic->getActivePlayer()->faults++;
						logic->switchplayer();
						cout << logic->getActivePlayer()->name << " wins!" << endl;
						logic->statsplayer();
					}
					else
					{
						cout << logic->getActivePlayer()->name << " wins!" << endl;
						logic->switchplayer();
						logic->statsplayer();
					}
				}
				else
				{
					cout << "Fault at game ball" << endl;
					logic->getActivePlayer()->faults++;
					logic->switchplayer();
					cout << logic->getActivePlayer()->name << " wins!" << endl;
					logic->statsplayer();
				}
			}
			else
			{
				// set players color
				if (control.first != emptyball && logic->getActivePlayer()->playercolor == emptyball)
				{
					cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
					if (control.first == yellowball)
					{
						logic->getActivePlayer()->playercolor = yellowball;
						logic->getInactivePlayer()->playercolor = redball;
						cout << logic->getActivePlayer()->name
							<< " plays with YELLOW"
							<< endl
							<< logic->getInactivePlayer()->name
							<< " plays with RED"
							<< endl;
					}
					else
					{
						logic->getActivePlayer()->playercolor = redball;
						logic->getInactivePlayer()->playercolor = yellowball;
						cout << logic->getActivePlayer()->name
							<< " plays with RED"
							<< endl
							<< logic->getInactivePlayer()->name
							<< " plays with YELLOW"
							<< endl;
					}
					cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				}

				// white in pocket or wrong first ball tocuhed
				if ((
					white->isinpocket ||
					(
						logic->getActivePlayer()->firsttouched != logic->getActivePlayer()->playercolor
						&& logic->getActivePlayer()->firsttouched != emptyball
						&& logic->getActivePlayer()->playercolor != emptyball
						)
					)
				)
				{
					if (white->isinpocket)
					{
						cout << "Fault at cue ball" << endl;
						logic->getActivePlayer()->faults++;
						control.switch_players = true;
					}
					if (logic->getActivePlayer()->firsttouched != logic->getActivePlayer()->playercolor
						&& logic->getActivePlayer()->firsttouched != emptyball
						&& logic->getActivePlayer()->playercolor != emptyball
						)
					{
						if (logic->getActivePlayer()->inpocket != 7 ||
							logic->getActivePlayer()->firsttouched != matchball)
						{
							cout << "Fault, wrong first ball touched" << endl;
							logic->getActivePlayer()->faults++;
							control.switch_players = true;
						}
					}

					control.fault = true;
					white->ResetBall(
						vec3(
							gameTable->relative.x + gameTable->lengthtable / 4,
							gameTable->relative.y + BallObject::radius,
							gameTable->relative.z + gameTable->widthtable / 2
						));

					camera->set(
						vec3(
							gameTable->relative.x - 3,
							gameTable->widthtable,
							gameTable->relative.z + gameTable->widthtable / 2
						),
						vec3(
							gameTable->relative.x + gameTable->lengthtable / 2,
							gameTable->widthtable / 2 + O3D::border_height,
							gameTable->relative.z + gameTable->widthtable / 2),
						vec3(0, 1, 0));
					camera->SetPerspective(RADIANS(80.0f), 1280.0f / 720.0f, 0.01f, 200.0f);
				}
				else
				{
					// in pocket in that turn
					if (control.yellowcount != -1 && control.redcount != -1)
					{
						if (logic->getActivePlayer()->playercolor == yellowball)
						{
							if (logic->getActivePlayer()->inpocket >= control.yellowcount)
							{
								logic->getActivePlayer()->inpocket = control.yellowcount;
								logic->getInactivePlayer()->inpocket = control.redcount;
								control.switch_players = true;
							}
							else
							{
								logic->getActivePlayer()->inpocket = control.yellowcount;
								logic->getInactivePlayer()->inpocket = control.redcount;
							}
						}
						else
						{
							if (logic->getActivePlayer()->inpocket >= control.redcount)
							{
								logic->getActivePlayer()->inpocket = control.redcount;
								logic->getInactivePlayer()->inpocket = control.yellowcount;
								control.switch_players = true;
							}
							else
							{
								logic->getActivePlayer()->inpocket = control.redcount;
								logic->getInactivePlayer()->inpocket = control.yellowcount;
							}
						}

						if (control.redcount == 0 && control.yellowcount == 0)
							control.switch_players = true;

						control.yellowcount = -1;
						control.redcount = -1;
					}
				}

				// reset first ball touched
				if (logic->getActivePlayer()->firsttouched != emptyball)
					logic->getActivePlayer()->firsttouched = emptyball;
				
				if (control.switch_players)
				{
					control.switch_players = false;
					logic->switchplayer();
				}
			}
			control.calculate_stats = false;
		}
	}
}

vec3 PoolGame::calculateSecondPoint(float deltaTimeSeconds)
{
	float border = (3 * O3D::pocket_radius) / 7
		+ BallObject::radius
		+ O3D::offsetbank + 0.01f;
	bool collide = false;
	vec3 trajectory = normalize(
		vec3(
			camera->getForward().x,
			0,
			camera->getForward().z)
	);
	float velocity = (cue->k * deltaTimeSeconds * cue->distance) / white->mass;
	vec3 newpoint = white->translate_coordinates + trajectory * velocity;

	if (newpoint.x < gameTable->relative.x + border ||
		newpoint.x > gameTable->relative.x + gameTable->lengthtable - border ||
		newpoint.z < gameTable->relative.z + border ||
		newpoint.z > gameTable->relative.z + gameTable->widthtable - border)
		collide = true;


	for (BallObject* ball : allballs)
	{
		vec3 C = ball->translate_coordinates - white->translate_coordinates;

		if (length(trajectory * velocity) + 2 * BallObject::radius < length(C)) 
			continue;

		vec3 N = normalize(trajectory * velocity);
		float cosangle = dot(N, C);
		if (cosangle <= 0) continue;

		float closest = length(C) * length(C) - cosangle * cosangle;
		if (closest >= 4 * BallObject::radius * BallObject::radius) continue;


		float beforeintersection = 4 * BallObject::radius * BallObject::radius - closest;
		if (beforeintersection < 0) continue;

		float distance = cosangle - sqrt(beforeintersection);

		if (length(trajectory * velocity) < distance) continue;

		collide = true;
	}

	if (!collide)
		control.futurewhite = newpoint;

	return control.futurewhite;
}

void PoolGame::FrameEnd()
{
	DrawCoordinatSystem(camera->getViewMatrix(), camera->getProjectionMatrix());
}

void PoolGame::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && 
		!control.moving_scene && 
		control.start_game &&
		control.shoot_try
		)
		if (cue->distance < 8.0f) cue->distance += (8.0f / 2.0f) * deltaTime;
		else cue->distance = 0.0f;

	if (window->KeyHold(GLFW_KEY_1))
		camera->SetPerspective(RADIANS(80), 1280.0f / 720.0f, 0.01f, 200.0f);

	if (window->KeyHold(GLFW_KEY_V) && camera->projectioninformation.fieldOfViewY < RADIANS(90))
		camera->SetPerspective(
			camera->projectioninformation.fieldOfViewY + RADIANS(camera->zoomspeed),
			camera->projectioninformation.aspectRatio,
			camera->projectioninformation.zNear,
			camera->projectioninformation.zFar);

	if (window->KeyHold(GLFW_KEY_B) && camera->projectioninformation.fieldOfViewY > RADIANS(40))
		camera->SetPerspective(
			camera->projectioninformation.fieldOfViewY - RADIANS(camera->zoomspeed),
			camera->projectioninformation.aspectRatio,
			camera->projectioninformation.zNear,
			camera->projectioninformation.zFar);

	if (!control.start_game || control.fault)
	{
		if (window->KeyHold(GLFW_KEY_LEFT_SHIFT))
		{
			float border = (3 * O3D::pocket_radius) / 7 
				+ BallObject::radius 
				+ O3D::offsetbank + 0.01f;

			if (window->KeyHold(GLFW_KEY_W))
			{
				if (white->translate_coordinates.x <= gameTable->relative.x
					+ (control.fault ? 
						gameTable->lengthtable 
						: (gameTable->lengthtable / 4)) 
					- border && !white_is_in_other_ball(
						vec3(
							white->translate_coordinates.x + deltaTime * 3,
							white->translate_coordinates.y,
							white->translate_coordinates.z
						)
					))
					white->translate_coordinates.x += deltaTime * 3;
			}
			if (window->KeyHold(GLFW_KEY_A))
			{
				if (white->translate_coordinates.z >= gameTable->relative.z
					+ border && !white_is_in_other_ball(
						vec3(
							white->translate_coordinates.x,
							white->translate_coordinates.y,
							white->translate_coordinates.z - deltaTime * 3
						)
					))
					white->translate_coordinates.z -= deltaTime * 3;
			}
			if (window->KeyHold(GLFW_KEY_S))
			{
				if (white->translate_coordinates.x >= gameTable->relative.x
					+ border && !white_is_in_other_ball(
						vec3(
							white->translate_coordinates.x - deltaTime * 3,
							white->translate_coordinates.y,
							white->translate_coordinates.z
						)
					))
					white->translate_coordinates.x -= deltaTime * 3;
			}
			if (window->KeyHold(GLFW_KEY_D))
			{
				
				if (white->translate_coordinates.z <= gameTable->relative.z
					+ gameTable->widthtable
					- border && 
					!white_is_in_other_ball(
						vec3(
							white->translate_coordinates.x,
							white->translate_coordinates.y,
							white->translate_coordinates.z + deltaTime * 3
						)
					))
					white->translate_coordinates.z += deltaTime * 3;
			}
		}
		else {
			camera->setPreviousEyePosition(camera->getEyePosition());

			if (window->KeyHold(GLFW_KEY_W)) camera->translateForward(deltaTime * camera->movespeed);
			if (window->KeyHold(GLFW_KEY_A)) camera->translateRight(-1 * deltaTime * camera->movespeed);
			if (window->KeyHold(GLFW_KEY_S)) camera->translateForward(-1 * deltaTime * camera->movespeed);
			if (window->KeyHold(GLFW_KEY_D)) camera->translateRight(deltaTime * camera->movespeed);
			if (window->KeyHold(GLFW_KEY_Q)) camera->translateUpword(deltaTime * camera->movespeed);
			if (window->KeyHold(GLFW_KEY_E)) camera->translateUpword(-1 * deltaTime * camera->movespeed);

			if (camera->getEyePosition().x > gameTable->relative.x - (3 * O3D::pocket_radius) / 2 - 0.05f &&
				camera->getEyePosition().x < gameTable->relative.x + gameTable->lengthtable + (3 * O3D::pocket_radius) / 2 + 0.05f &&
				camera->getEyePosition().y < gameTable->relative.y + 0.05f + O3D::border_height &&
				camera->getEyePosition().y > gameTable->relative.y - gameTable->widthtable / 2 - 0.05f &&
				camera->getEyePosition().z > gameTable->relative.z - (3 * O3D::pocket_radius) / 2 - 0.05f &&
				camera->getEyePosition().z < gameTable->relative.z + gameTable->widthtable + (3 * O3D::pocket_radius) / 2 + 0.05f
				) 
			{
				camera->setToPreviousEyePosition();
			}

			if (camera->getEyePosition().x > gameTable->relative.x + gameTable->lengthtable + 4 - 0.2f ||
				camera->getEyePosition().x < gameTable->relative.x - 4 + 0.2f ||
				camera->getEyePosition().y < 0.2f ||
				camera->getEyePosition().y > gameTable->lengthtable - 2.2f ||
				camera->getEyePosition().z > gameTable->relative.z + 4 + gameTable->widthtable / 2 + gameTable->lengthtable / 2 - 0.2f ||
				camera->getEyePosition().z < gameTable->relative.z - 4 + gameTable->widthtable / 2 - gameTable->lengthtable / 2 + 0.2f
				) 
			{
				camera->setToPreviousEyePosition();
			}
		}
	}
}

bool PoolGame::white_is_in_other_ball(const vec3 translate)
{
	bool result = false;
	for (int index = 0; index < allballs.size(); ++index)
	{
		if (allballs[index]->name != white->name)
			if (length(allballs[index]->translate_coordinates
				- translate) <= 2 * BallObject::radius)
			{
				result = true;
				break;
			}
	}
	return result;
}

void PoolGame::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_H)
	{
		control.helpline = !control.helpline;
	}

	if (!control.moving_scene && key == GLFW_KEY_SPACE && camera->type != ThirdPerson)
	{
		control.start_game = true;
		control.futurewhite = white->translate_coordinates;
		control.draw_cue = true;
		control.shoot_try = true;
		control.fault = false;

		camera->type = ThirdPerson;
		camera->set(
			vec3(
				white->translate_coordinates.x - 1.5,
				white->translate_coordinates.y + 0.75,
				white->translate_coordinates.z
			),
			white->translate_coordinates,
			vec3(0, 1, 0)
		);
		camera->SetPerspective(RADIANS(80), 1280.0f / 720.0f, 0.01f, 200.0f);
	}
}

void PoolGame::OnKeyRelease(int key, int mods)
{
}

void PoolGame::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (camera->type == FirstPerson) {
			camera->rotateFirstPerson_OX(-1 * deltaY * camera->sensivityOX);
			camera->rotateFirstPerson_OY(-1 * deltaX * camera->sensivityOY);
		}
		if (camera->type == ThirdPerson) {
			camera->rotateThirdPerson_OY(-1 * deltaX * camera->sensivityOY);
			cue->rotate_angle += vec3(0, -1 * deltaX * camera->sensivityOY, 0);
		}
	}
}

void PoolGame::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void PoolGame::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && 
		control.start_game && 
		!control.moving_scene && 
		control.shoot_try
		)
	{
		control.shoot = true;
		control.shoot_try = false;
	}
}

void PoolGame::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void PoolGame::OnWindowResize(int width, int height)
{
}

void PoolGame::RenderMesh(const Mesh * mesh, const Material3D material, const Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	shader->Use();

	// get and set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	// get and set shader uniform "View" to viewMatrix
	glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	// get and set shader uniform "Projection" to projectionMatrix
	glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	//Set shader uniforms for light & material properties
	glUniform3f(glGetUniformLocation(shader->program, "light.position"),
		light.position.x,
		light.position.y,
		light.position.z);

	glUniform3f(glGetUniformLocation(shader->program, "light.direction"),
		light.direction.x,
		light.direction.y,
		light.direction.z);

	glUniform3f(glGetUniformLocation(shader->program, "light.ambient_color"),
		light.ambient_color.x,
		light.ambient_color.y,
		light.ambient_color.z);

	glUniform3f(glGetUniformLocation(shader->program, "light.diffuse_color"),
		light.diffuse_color.x,
		light.diffuse_color.y,
		light.diffuse_color.z);

	glUniform3f(glGetUniformLocation(shader->program, "light.specular_color"),
		light.specular_color.x,
		light.specular_color.y,
		light.specular_color.z);

	glUniform3f(glGetUniformLocation(shader->program, "light.emisive_color"),
		light.emisive_color.x,
		light.emisive_color.y,
		light.emisive_color.z);

	glUniform1f(glGetUniformLocation(shader->program, "material.Ke"), material.Ke);
	glUniform1f(glGetUniformLocation(shader->program, "material.Ka"), material.Ka);
	glUniform1f(glGetUniformLocation(shader->program, "material.Kd"), material.Kd);
	glUniform1f(glGetUniformLocation(shader->program, "material.Ks"), material.Ks);
	glUniform1i(glGetUniformLocation(shader->program, "material.shininess"), material.shininess);
	
	// get an set eye position (camera position) uniform
	vec3 eye_position = camera->getEyePosition();
	glUniform3f(glGetUniformLocation(shader->program, "eye_position"),
		eye_position.x,
		eye_position.y,
		eye_position.z);

	if (control.translate_cue)
		glUniform1i(glGetUniformLocation(shader->program, "is_cue"), 1);
	else
		glUniform1i(glGetUniformLocation(shader->program, "is_cue"), 0);

	glUniform1f(glGetUniformLocation(shader->program, "translate_cue"), cue->distance);

	//activate texture location, bind the texture ID and send terture uniform value
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.texture->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "texturemap"), 0);

	// draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void PoolGame::UpdateTable(float deltaTimeSeconds, mat4 modelMatrix)
{
	mat4 modelMatrixLocal;
	{
		modelMatrixLocal = mat4(1);

		RenderMesh(
			gameTable->objects["TABLE"]->mesh,
			materialmap["pooltable"],
			shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);

		RenderMesh(
			gameTable->objects["FRUSTUM"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);

		RenderMesh(
			gameTable->objects["POCKET1"]->mesh,
			materialmap["black"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2,
			gameTable->position.y,
			gameTable->position.z
		);

		RenderMesh(
			gameTable->objects["POCKET2"]->mesh,
			materialmap["black"],
			shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable,
			gameTable->position.y,
			gameTable->position.z
		);

		RenderMesh(
			gameTable->objects["POCKET3"]->mesh,
			materialmap["black"],
			shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable
		);

		RenderMesh(
			gameTable->objects["POCKET4"]->mesh,
			materialmap["black"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable
		);

		RenderMesh(
			gameTable->objects["POCKET5"]->mesh,
			materialmap["black"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable
		);

		RenderMesh(
			gameTable->objects["POCKET6"]->mesh,
			materialmap["black"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z
		);

		RenderMesh(
			gameTable->objects["BOUND1"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2 + O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z
		);

		RenderMesh(
			gameTable->objects["BOUND2"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2 + O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable + (3 * O3D::pocket_radius) / 2
		);

		RenderMesh(
			gameTable->objects["BOUND3"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable + (3 * O3D::pocket_radius) / 2
		);

		RenderMesh(
			gameTable->objects["BOUND4"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x,
			gameTable->position.y,
			gameTable->position.z + O3D::pocket_radius
		);

		RenderMesh(
			gameTable->objects["BOUND5"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable + (3 * O3D::pocket_radius) / 2,
			gameTable->position.y,
			gameTable->position.z + O3D::pocket_radius
		);

		RenderMesh(
			gameTable->objects["BOUND6"]->mesh,
			materialmap["wood"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(90.0f));

		RenderMesh(
			gameTable->objects["CORNER1"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2,
			gameTable->position.y,
			gameTable->position.z
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(90.0f));

		RenderMesh(
			gameTable->objects["CORNER2"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable,
			gameTable->position.y,
			gameTable->position.z
		);

		RenderMesh(
			gameTable->objects["CORNER3"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(-90.0f));

		RenderMesh(
			gameTable->objects["CORNER4"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(-90.0f));

		RenderMesh(
			gameTable->objects["CORNER5"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(180.0f));

		RenderMesh(
			gameTable->objects["CORNER6"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + O3D::offset + 3 * O3D::pocket_radius,
			gameTable->position.y - gameTable->widthtable / 2,
			gameTable->position.z + O3D::offset + 3 * O3D::pocket_radius
		);

		RenderMesh(
			gameTable->objects["LEG1"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + O3D::offset + 3 * O3D::pocket_radius,
			gameTable->position.y - gameTable->widthtable / 2,
			gameTable->position.z - O3D::offset - 3 * O3D::pocket_radius + gameTable->widthtable
		);

		RenderMesh(
			gameTable->objects["LEG2"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x - O3D::offset - 3 * O3D::pocket_radius + gameTable->lengthtable,
			gameTable->position.y - gameTable->widthtable / 2,
			gameTable->position.z + O3D::offset + 3 * O3D::pocket_radius
		);

		RenderMesh(
			gameTable->objects["LEG3"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x - O3D::offset - 3 * O3D::pocket_radius + gameTable->lengthtable,
			gameTable->position.y - gameTable->widthtable / 2,
			gameTable->position.z - O3D::offset - 3 * O3D::pocket_radius + gameTable->widthtable
		);

		RenderMesh(
			gameTable->objects["LEG4"]->mesh,
			materialmap["steel"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z + (3 * O3D::pocket_radius) / 7
		);

		RenderMesh(
			gameTable->objects["BANK1"]->mesh,
			materialmap["pooltable"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2 + O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z + (3 * O3D::pocket_radius) / 7
		);

		RenderMesh(
			gameTable->objects["BANK2"]->mesh,
			materialmap["pooltable"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable - O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable - (3 * O3D::pocket_radius) / 7
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(180.0f));

		RenderMesh(
			gameTable->objects["BANK3"]->mesh,
			materialmap["pooltable"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + gameTable->lengthtable / 2 - O3D::pocket_radius,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable - (3 * O3D::pocket_radius) / 7
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(180.0f));

		RenderMesh(
			gameTable->objects["BANK4"]->mesh,
			materialmap["pooltable"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x + (3 * O3D::pocket_radius) / 7,
			gameTable->position.y,
			gameTable->position.z + gameTable->widthtable - O3D::pocket_radius
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(90.0f));

		RenderMesh(
			gameTable->objects["BANK5"]->mesh,
			materialmap["pooltable"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}

	{
		modelMatrixLocal = mat4(1);
		modelMatrixLocal *= T3D::Translate(
			gameTable->position.x - (3 * O3D::pocket_radius) / 7 + gameTable->lengthtable,
			gameTable->position.y,
			gameTable->position.z + O3D::pocket_radius
		);
		modelMatrixLocal *= T3D::RotateOY(RADIANS(270.0f));

		RenderMesh(
			gameTable->objects["BANK6"]->mesh,
			materialmap["pooltable"], shaders["8BallPoolShader"],
			modelMatrix * modelMatrixLocal
		);
	}
}

void PoolGame::RestartGame() {
	float epsilon = 0.001f;
	white->ResetBall(
		vec3(
			gameTable->relative.x + gameTable->lengthtable / 4,
			gameTable->relative.y + BallObject::radius,
			gameTable->relative.z + gameTable->widthtable / 2
		));
	reds[0]->ResetBall(
		vec3(
			gameTable->relative.x + gameTable->lengthtable - gameTable->lengthtable / 4,
			gameTable->relative.y + BallObject::radius,
			gameTable->relative.z + gameTable->widthtable / 2
		));
	reds[1]->ResetBall(
		vec3(
			reds[0]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[0]->translate_coordinates.y,
			reds[0]->translate_coordinates.z - BallObject::radius - epsilon
		));
	yellows[0]->ResetBall(
		vec3(
			reds[0]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[0]->translate_coordinates.y,
			reds[0]->translate_coordinates.z + BallObject::radius + epsilon
		));
	yellows[1]->ResetBall(
		vec3(
			reds[1]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[1]->translate_coordinates.y,
			reds[1]->translate_coordinates.z - BallObject::radius - epsilon
		));
	black->ResetBall(
		vec3(
			reds[1]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[1]->translate_coordinates.y,
			reds[0]->translate_coordinates.z
		));
	reds[2]->ResetBall(
		vec3(
			yellows[0]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			yellows[0]->translate_coordinates.y,
			yellows[0]->translate_coordinates.z + BallObject::radius + epsilon
		));
	reds[3]->ResetBall(
		vec3(
			yellows[1]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			yellows[1]->translate_coordinates.y,
			yellows[1]->translate_coordinates.z - BallObject::radius - epsilon
		));
	reds[4]->ResetBall(
		vec3(
			yellows[1]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			yellows[1]->translate_coordinates.y,
			reds[1]->translate_coordinates.z
		));
	yellows[2]->ResetBall(
		vec3(
			black->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			black->translate_coordinates.y,
			yellows[0]->translate_coordinates.z
		));
	yellows[3]->ResetBall(
		vec3(
			reds[2]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[2]->translate_coordinates.y,
			reds[2]->translate_coordinates.z + BallObject::radius + epsilon
		));
	yellows[4]->ResetBall(
		vec3(
			reds[3]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[3]->translate_coordinates.y,
			reds[3]->translate_coordinates.z - BallObject::radius - epsilon
		));
	reds[5]->ResetBall(
		vec3(
			reds[3]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[3]->translate_coordinates.y,
			yellows[1]->translate_coordinates.z
		));
	yellows[5]->ResetBall(
		vec3(
			reds[4]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			reds[4]->translate_coordinates.y,
			reds[0]->translate_coordinates.z
		));
	yellows[6]->ResetBall(
		vec3(
			yellows[2]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			yellows[2]->translate_coordinates.y,
			reds[2]->translate_coordinates.z
		));
	reds[6]->ResetBall(
		vec3(
			yellows[3]->translate_coordinates.x + 2 * BallObject::radius - BallObject::radius / 4,
			yellows[3]->translate_coordinates.y,
			yellows[3]->translate_coordinates.z + BallObject::radius + epsilon
		));
}
