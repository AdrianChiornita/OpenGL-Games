#include "Objects3D.h"

using namespace glm;
using namespace std;
using namespace Objects3D;

const float Objects3D::BallObject::radius = 0.09f;

Objects3D::BallObject::BallObject(const string name, const balltype type)
{
	this->name = name; 
	this->type = type;
	this->mesh = nullptr;

	this->translate_coordinates = vec3(0);

	this->rotate_angle = 0.0f;
	this->rotate_axis = vec3(0, 0, 1);

	this->trajectory = vec3(0);
	this->movement = vec3(0);
	this->velocity = 0.0f;
	this->mass = 0.2f;

	this->moving = false;
	this->isinpocket = false;
}

Objects3D::BallObject::~BallObject()
{
}

void Objects3D::BallObject::CreateMesh()
{
	this->mesh = new Mesh(this->name);
	this->mesh->LoadMesh("Source/Games/8 Ball Pool/Objects/", "ball.obj");
}

void Objects3D::BallObject::Update(const float deltaTime)
{
	if (!this->isinpocket)
	{
		if (this->velocity <= 0.0f)
		{
			this->velocity = 0.0f;
			this->moving = false;
		}
		else 
			this->moving = true;
		
		if (this->moving)
		{
			this->translate_coordinates += this->movement;

			this->rotate_angle -= length(this->movement) / radius;
			if (this->rotate_angle <= -2 * M_PI) 
				this->rotate_angle = 0.0f;
			this->rotate_axis = normalize(cross(this->movement, vec3(0, 1, 0)));

			this->velocity -= friction_coefficient * gravitational_acceleration * deltaTime;
		}
	}
	else {
		this->moving = false;
	}
}

void Objects3D::BallObject::Collisions(const vec3 relative, const float length, const float width)
{
	BanksColiision(relative, length, width);

	trajectory.y = 0;
	translate_coordinates.y = width / 2 + radius;
}

bool Objects3D::BallObject::isInPocket(const vec3 relative, const float length, const float width)
{
	vec3 pocketposition = relative;
	vec3 contacttotable = vec3(
		translate_coordinates.x, 
		translate_coordinates.y - radius, 
		translate_coordinates.z);
	
	if (glm::length(contacttotable - pocketposition) < O3D::pocket_radius) return true;
	
	pocketposition.x += length / 2;
	if (glm::length(contacttotable - pocketposition) < O3D::pocket_radius) return true;
	
	pocketposition.x += length / 2;
	if (glm::length(contacttotable - pocketposition) < O3D::pocket_radius) return true;
	
	pocketposition.z += width;
	if (glm::length(contacttotable - pocketposition) < O3D::pocket_radius) return true;
	
	pocketposition.x -= length / 2;
	if (glm::length(contacttotable - pocketposition) < O3D::pocket_radius) return true;
	
	pocketposition.x -= length / 2;
	if (glm::length(contacttotable - pocketposition) < O3D::pocket_radius) return true;

	return false;
}

bool Objects3D::BallObject::BankColiision(const vec3 leftbottom, const vec3 righttop, const vec3 normal)
{
	if (dot(trajectory, normal) > 0) return false;
	if (glm::length(translate_coordinates - RectangleClosestPoint(leftbottom, righttop)) < radius) return true;
	return false;
}

bool Objects3D::BallObject::BallColiision(BallObject* ball)
{
	if (this->moving)
	{
		vec3 movevec = this->movement - ball->movement;

		vec3 C = ball->translate_coordinates - this->translate_coordinates;
		if (length(movevec) + 2 * radius < length(C)) return false;

		vec3 N = normalize(movevec);
		float cosangle = dot(N, C);
		if (cosangle <= 0) return false;

		float closest = length(C) * length(C) - cosangle * cosangle;
		if (closest >= 4 * radius * radius) return false;


		float beforeintersection = 4 * radius * radius - closest;
		if (beforeintersection < 0) return false;

		float distance = cosangle - sqrt(beforeintersection);

		if (length(movevec) < distance) return false;

		movevec = normalize(movevec) * distance;

		this->movement *= length(movevec) / length(this->movement);
		ball->movement *= length(movevec) / length(this->movement);

		return true;
	}
	return false;
}

bool  Objects3D::BallObject::TreatBallCollision(BallObject* ball)
{
	if (this->BallColiision(ball))
	{
		if (this->mass > ball->mass)
		{
			float mass = this->mass;
			this->mass = ball->mass;
			ball->mass = mass;
		}
		vec3 centertocenter = this->translate_coordinates - ball->translate_coordinates;
		centertocenter = normalize(centertocenter);

		float P = (2.0 * (dot(this->movement, centertocenter)
			- dot(ball->movement, centertocenter))) /
			(this->mass + ball->mass);

		this->movement = this->movement - P * ball->mass * centertocenter;
		ball->movement = ball->movement + P * this->mass * centertocenter;

		this->trajectory = normalize(this->movement);
		ball->trajectory = normalize(ball->movement);
		
		float velocity;
		if (this->velocity > 0.0f && ball->velocity > 0.0f)
		{
			velocity = (this->velocity + ball->velocity) / 2;
			ball->velocity = velocity;
			this->velocity = velocity;
		}
		else
		{
			if (ball->velocity <= 0.0f)
				ball->velocity = 0.75f * this->velocity;
		}
		return true;
	}
	return false;
}

void  Objects3D::BallObject::BanksColiision(const vec3 relative, const float length, const float width)
{
	float border = (3 * O3D::pocket_radius) / 7 + radius + O3D::offsetbank + 0.5f;

	if (translate_coordinates.x > relative.x + border &&
		translate_coordinates.x < relative.x + length - border &&
		translate_coordinates.z > relative.z + border &&
		translate_coordinates.z < relative.z + width - border)
		return;

	vec3 normal;


	// corner 1
	if (translate_coordinates.x < relative.x + border &&
		translate_coordinates.z < relative.z + border)
	{
		normal = normalize(vec3(-1, 0, 1));
		if (BankColiision(
			vec3(
				relative.x + O3D::pocket_radius,
				relative.y,
				relative.z
			),
			vec3(
				relative.x + O3D::pocket_radius + offsetbig,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}

		normal = normalize(vec3(1, 0, -1));
		if (BankColiision(
			vec3(
				relative.x,
				relative.y,
				relative.z + O3D::pocket_radius
			),
			vec3(
				relative.x + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + O3D::pocket_radius + offsetbig
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// corner 2
	if (translate_coordinates.x < relative.x + border &&
		translate_coordinates.z > relative.z + width - border)
	{
		normal = normalize(vec3(-1, 0, -1));
		if (BankColiision(
			vec3(
				relative.x + O3D::pocket_radius,
				relative.y,
				relative.z + width - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank
			),
			vec3(
				relative.x + O3D::pocket_radius + offsetbig,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width

			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}

		normal = normalize(vec3(1, 0, 1));
		if (BankColiision(
			vec3(
				relative.x,
				relative.y,
				relative.z + width - O3D::pocket_radius - offsetbig
			),
			vec3(
				relative.x + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - O3D::pocket_radius
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// corner 3
	if (translate_coordinates.x > relative.x + length - border &&
		translate_coordinates.z < relative.z + border)
	{
		normal = normalize(vec3(1, 0, 1));
		if (BankColiision(
			vec3(
				relative.x + length - O3D::pocket_radius - offsetbig,
				relative.y,
				relative.z
			),
			vec3(
				relative.x + length - O3D::pocket_radius,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}

		normal = normalize(vec3(-1, 0, -1));
		if (BankColiision(
			vec3(
				relative.x + length,
				relative.y,
				relative.z + O3D::pocket_radius
			),
			vec3(
				relative.x + length - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + O3D::pocket_radius + offsetbig
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// corner 4
	if (translate_coordinates.x > relative.x + length - border &&
		translate_coordinates.z > relative.z + width - border)
	{
		normal = normalize(vec3(1, 0, -1));
		if (BankColiision(
			vec3(
				relative.x + length - O3D::pocket_radius - offsetbig,
				relative.y,
				relative.z + width
			),
			vec3(
				relative.x + length - O3D::pocket_radius,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}

		normal = normalize(vec3(-1, 0, 1));
		if (BankColiision(
			vec3(
				relative.x + length - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank,
				relative.y,
				relative.z + width - O3D::pocket_radius - offsetbig
			),
			vec3(
				relative.x + length,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - O3D::pocket_radius
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// corner 5
	if (translate_coordinates.x < relative.x + length / 2 + border &&
		translate_coordinates.x > relative.x + length / 2 - border &&
		translate_coordinates.z < relative.z + border)
	{
		normal = normalize(vec3(sin(RADIANS(30.0f)), 0, cos(RADIANS(30.0f))));
		if (BankColiision(
			vec3(
				relative.x + length / 2 - O3D::pocket_radius,
				relative.y,
				relative.z
			),
			vec3(
				relative.x + length / 2 - O3D::pocket_radius + offsetsmall,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}

		normal = normalize(vec3(-sin(RADIANS(30.0f)), 0, cos(RADIANS(30.0f))));
		if (BankColiision(
			vec3(
				relative.x + length / 2 + O3D::pocket_radius,
				relative.y,
				relative.z
			),
			vec3(
				relative.x + length / 2 + O3D::pocket_radius + offsetsmall,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// corner 6
	if (translate_coordinates.x < relative.x + length / 2 + border &&
		translate_coordinates.x > relative.x + length / 2 - border &&
		translate_coordinates.z > relative.z + width - border)
	{
		normal = normalize(vec3(sin(RADIANS(30.0f)), 0, -cos(RADIANS(30.0f))));
		if (BankColiision(
			vec3(
				relative.x + length / 2 - O3D::pocket_radius,
				relative.y,
				relative.z + width - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank
			),
			vec3(
				relative.x + length / 2 - O3D::pocket_radius + offsetsmall,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}

		normal = normalize(vec3(-sin(RADIANS(30.0f)), 0, -cos(RADIANS(30.0f))));
		if (BankColiision(
			vec3(
				relative.x + length / 2 + O3D::pocket_radius,
				relative.y,
				relative.z + width - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank
			),
			vec3(
				relative.x + length / 2 + O3D::pocket_radius + offsetsmall,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// border 1
	if (translate_coordinates.x < relative.x + length / 2 &&
		translate_coordinates.z < relative.z + border)
	{
		normal = vec3(0, 0, 1);
		if (BankColiision(
			vec3(
				relative.x + O3D::pocket_radius + offsetbig,
				relative.y,
				relative.z + (3 * O3D::pocket_radius) / 7
			),
			vec3(
				relative.x + length / 2 - O3D::pocket_radius - offsetsmall,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// border 2
	if (translate_coordinates.x > relative.x + length / 2 &&
		translate_coordinates.z < relative.z + border)
	{
		normal = vec3(0, 0, 1);
		if (BankColiision(
			vec3(
				relative.x + length / 2 + O3D::pocket_radius + offsetsmall,
				relative.y,
				relative.z + (3 * O3D::pocket_radius) / 7
			),
			vec3(
				relative.x + length - O3D::pocket_radius - offsetbig,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// border 3
	if (translate_coordinates.x <  relative.x + length / 2 &&
		translate_coordinates.z > relative.z + width - border)
	{
		normal = vec3(0, 0, -1);
		if (BankColiision(
			vec3(
				relative.x + O3D::pocket_radius + offsetbig,
				relative.y,
				relative.z + width - (3 * O3D::pocket_radius) / 7
			),
			vec3(
				relative.x + length / 2 - O3D::pocket_radius - offsetsmall,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}
	
	// border 4
	if (translate_coordinates.x > relative.x + length / 2 &&
		translate_coordinates.z > relative.z + width - border)
	{
		normal = vec3(0, 0, -1);
		if (BankColiision(
			vec3(
				relative.x + length / 2 + O3D::pocket_radius + offsetsmall,
				relative.y,
				relative.z + width - (3 * O3D::pocket_radius) / 7
			),
			vec3(
				relative.x + length - O3D::pocket_radius - offsetbig,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	//border 5
	if (translate_coordinates.x < relative.x + border)
	{
		normal = vec3(1, 0, 0);
		if (BankColiision(
			vec3(
				relative.x + (3 * O3D::pocket_radius) / 7,
				relative.y,
				relative.z + O3D::pocket_radius + offsetbig
			),
			vec3(
				relative.x + (3 * O3D::pocket_radius) / 7 + O3D::offsetbank,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - O3D::pocket_radius - offsetbig
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}

	// border 6
	if (translate_coordinates.x > relative.x + length - border)
	{
		normal = vec3(-1, 0, 0);
		if (BankColiision(
			vec3(
				relative.x + length - (3 * O3D::pocket_radius) / 7,
				relative.y,
				relative.z + O3D::pocket_radius + offsetbig
			),
			vec3(
				relative.x + length - (3 * O3D::pocket_radius) / 7 - O3D::offsetbank,
				relative.y + O3D::border_height - O3D::offsetup,
				relative.z + width - O3D::pocket_radius - offsetbig
			),
			normal
		))
		{
			trajectory = reflect(trajectory, normal);
			return;
		}
	}
}

const vec3 Objects3D::BallObject::RectangleClosestPoint(const vec3 leftbottom, const vec3 righttop)
{
	return 
		vec3(
			clamp(translate_coordinates.x, leftbottom.x, righttop.x),
			clamp(translate_coordinates.y, leftbottom.y, righttop.y),
			clamp(translate_coordinates.z, leftbottom.z, righttop.z)
		);
}

void Objects3D::BallObject::ResetBall(const vec3 translate)
{
	this->translate_coordinates = translate;
	this->rotate_angle = 0.0f;
	this->rotate_axis = vec3(0, 0, 1);
	this->trajectory = vec3(0);
	this->movement = vec3(0);
	this->velocity = 0.0f;

	this->moving = false;
	this->isinpocket = false;
}

const vec3 Objects3D::CueObject::position_relative = vec3(0.2, 0.2, 0.2);

Objects3D::CueObject::CueObject(const string name)
{
	this->name = name;
	this->translate_coordinates = vec3(0);
	this->rotate_angle = vec3(0);
	this->k = 10;
	this->distance = 0.0f;

	this->mesh = nullptr;
	this->line = nullptr;
}

Objects3D::CueObject::~CueObject()
{
}

void Objects3D::CueObject::CreateMesh()
{
	this->mesh = O3D::CreateCue(this->name.c_str(), vec3(0, 0, 0), 20 / 3);
}


Objects3D::PoolObject::PoolObject(const string name,
									Mesh* mesh,
									const glm::vec3 position)
{
	this->name = name;
	this->mesh = mesh;
	this->position = position;
}

Objects3D::PoolObject::~PoolObject()
{
}

Objects3D::PoolTable::PoolTable(const string name)
{
	this->name = name;
	this->position = glm::vec3(0, 0, 0);
	this->relative = this->position;
	this->lengthtable = 10;
	this->widthtable = 5;
}

Objects3D::PoolTable::~PoolTable()
{
}

void Objects3D::PoolTable::CreateTableMeshes()
{
	this->objects["TABLE"] =
		new PoolObject(
			"TABLE",
			O3D::CreateTable(
				"TABLE",
				this->position,
				this->lengthtable,
				this->widthtable
			),
			this->position
		);
	this->objects["FRUSTUM"] =
		new PoolObject(
			"FRUSTUM",
			O3D::CreateFrustumTable(
				"FRUSTUM",
				this->position,
				this->lengthtable,
				this->widthtable
			),
			this->position
		);
	this->objects["POCKET1"] =
		new PoolObject(
			"POCKET1",
			O3D::CreatePocket(
				"POCKET1",
				this->position
			),
			this->position
		);
	this->objects["POCKET2"] =
		new PoolObject(
			"POCKET2",
			O3D::CreatePocket(
				"POCKET2",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable / 2,
				this->position.y,
				this->position.z
			)
		);

	this->objects["POCKET3"] =
		new PoolObject(
			"POCKET3",
			O3D::CreatePocket(
				"POCKET3",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable,
				this->position.y,
				this->position.z
			)
		);

	this->objects["POCKET4"] =
		new PoolObject(
			"POCKET4",
			O3D::CreatePocket(
				"POCKET4",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable,
				this->position.y,
				this->position.z + this->widthtable
			)
		);
	
	this->objects["POCKET5"] =
		new PoolObject(
			"POCKET5",
			O3D::CreatePocket(
				"POCKET5",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable / 2,
				this->position.y,
				this->position.z + this->widthtable
			)
		);
	
	this->objects["POCKET6"] =
		new PoolObject(
			"POCKET6",
			O3D::CreatePocket(
				"POCKET6",
				this->position
			),
			vec3(
				this->position.x,
				this->position.y,
				this->position.z + this->widthtable
			)
		);

	
	this->objects["BOUND1"] =
		new PoolObject(
			"BOUND1",
			O3D::CreateBound(
				"BOUND1",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 2
				)
			),
			vec3(
				this->position.x + O3D::pocket_radius,
				this->position.y,
				this->position.z
			)
		);
	
	this->objects["BOUND2"] =
		new PoolObject(
			"BOUND2",
			O3D::CreateBound(
				"BOUND2",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 2
				)
			),
			vec3(
				this->position.x + this->lengthtable / 2 + O3D::pocket_radius,
				this->position.y,
				this->position.z
			)
		);

	this->objects["BOUND3"] =
		new PoolObject(
			"BOUND3",
			O3D::CreateBound(
				"BOUND3",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 2
				)
			),
			vec3(
				this->position.x + this->lengthtable / 2 + O3D::pocket_radius,
				this->position.y,
				this->position.z + this->widthtable + (3 * O3D::pocket_radius) / 2
			)
		);
	
	this->objects["BOUND4"] =
		new PoolObject(
			"BOUND4",
			O3D::CreateBound(
				"BOUND4",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 2
				)
			),
			vec3(
				this->position.x + O3D::pocket_radius,
				this->position.y,
				this->position.z + this->widthtable
			)
		);

	this->objects["BOUND5"] =
		new PoolObject(
			"BOUND5",
			O3D::CreateBound(
				"BOUND5",
				this->position,
				vec3(
					this->position.x - (3 * O3D::pocket_radius) / 2,
					this->position.y + O3D::border_height,
					this->position.z + this->widthtable - 2 * O3D::pocket_radius
				)
			),
			vec3(
				this->position.x,
				this->position.y,
				this->position.z + O3D::pocket_radius
			)
		);
	
	this->objects["BOUND6"] =
		new PoolObject(
			"BOUND6",
			O3D::CreateBound(
				"BOUND6",
				this->position,
				vec3(
					this->position.x - (3 * O3D::pocket_radius) / 2,
					this->position.y + O3D::border_height,
					this->position.z + this->widthtable - 2 * O3D::pocket_radius
				)
			),
			vec3(
				this->position.x + this->lengthtable + (3 * O3D::pocket_radius) / 2,
				this->position.y,
				this->position.z + O3D::pocket_radius
			)
		);
	
	this->objects["CORNER1"] =
		new PoolObject(
			"CORNER1",
			O3D::CreatePocketCorner(
				"CORNER1",
				this->position
			),
			this->position
		);
	
	this->objects["CORNER2"] =
		new PoolObject(
			"CORNER2",
			O3D::CreatePocketBound(
				"CORNER2",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable / 2,
				this->position.y,
				this->position.z
			)
		);

	this->objects["CORNER3"] =
		new PoolObject(
			"CORNER3",
			O3D::CreatePocketCorner(
				"CORNER3",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable,
				this->position.y,
				this->position.z
			)
		);

	this->objects["CORNER4"] =
		new PoolObject(
			"CORNER4",
			O3D::CreatePocketCorner(
				"CORNER4",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable,
				this->position.y,
				this->position.z + this->widthtable
			)
		);

	this->objects["CORNER5"] =
		new PoolObject(
			"CORNER5",
			O3D::CreatePocketBound(
				"CORNER5",
				this->position
			),
			vec3(
				this->position.x + this->lengthtable / 2,
				this->position.y,
				this->position.z + this->widthtable
			)
		);

	this->objects["CORNER6"] =
		new PoolObject(
			"CORNER6",
			O3D::CreatePocketCorner(
				"CORNER6",
				this->position
			),
			vec3(
				this->position.x,
				this->position.y,
				this->position.z + this->widthtable
			)
		);
	
	this->objects["LEG1"] =
		new PoolObject(
			"LEG1",
			O3D::CreateCylinder(
				"LEG1",
				this->position,
				this->widthtable / 6
			),
			vec3(
				this->position.x + O3D::offset + 3 * O3D::pocket_radius,
				this->position.y - this->widthtable / 2,
				this->position.z + O3D::offset + 3 * O3D::pocket_radius
			)
		);

	this->objects["LEG2"] =
		new PoolObject(
			"LEG2",
			O3D::CreateCylinder(
				"LEG2",
				this->position,
				this->widthtable / 6
			),
			vec3(
				this->position.x + O3D::offset + 3 * O3D::pocket_radius,
				this->position.y - this->widthtable / 2,
				this->position.z - O3D::offset - 3 * O3D::pocket_radius + this->widthtable
			)
		);

	this->objects["LEG3"] =
		new PoolObject(
			"LEG3",
			O3D::CreateCylinder(
				"LEG3",
				this->position,
				this->widthtable / 6
			),
			vec3(
				this->position.x - O3D::offset - 3 * O3D::pocket_radius + this->lengthtable,
				this->position.y - this->widthtable / 2,
				this->position.z + O3D::offset + 3 * O3D::pocket_radius
			)
		);

	this->objects["LEG4"] =
		new PoolObject(
			"LEG4",
			O3D::CreateCylinder(
				"LEG4",
				this->position,
				this->widthtable / 6
			),
			vec3(
				this->position.x - O3D::offset - 3 * O3D::pocket_radius + this->lengthtable,
				this->position.y - this->widthtable / 2,
				this->position.z - O3D::offset - 3 * O3D::pocket_radius + this->widthtable
			)
		);

	this->objects["BANK1"] =
		new PoolObject(
			"BANK1",
			O3D::CreateBank(
				"BANK1",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 7
				),
				offsetbig,
				offsetsmall
			),
			vec3(
				this->position.x + O3D::pocket_radius + offsetbig,
				this->position.y,
				this->position.z + (3 * O3D::pocket_radius) / 7
			)
		);

	this->objects["BANK2"] =
		new PoolObject(
			"BANK2",
			O3D::CreateBank(
				"BANK2",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 7
				),
				offsetsmall,
				offsetbig
			),
			vec3(
				this->position.x + this->lengthtable / 2 + O3D::pocket_radius + offsetsmall,
				this->position.y,
				this->position.z + (3 * O3D::pocket_radius) / 7
			)
		);

	this->objects["BANK3"] =
		new PoolObject(
			"BANK3",
			O3D::CreateBank(
				"BANK3",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 7
				),
				offsetbig,
				offsetsmall
			),
			vec3(
				this->position.x + this->lengthtable - O3D::pocket_radius - offsetbig,
				this->position.y,
				this->position.z + this->widthtable - (3 * O3D::pocket_radius) / 7
			)
		);

	this->objects["BANK4"] =
		new PoolObject(
			"BANK4",
			O3D::CreateBank(
				"BANK4",
				this->position,
				vec3(
					this->position.x + this->lengthtable / 2 - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 7
				),
				offsetsmall,
				offsetbig
			),
			vec3(
				this->position.x + this->lengthtable / 2 - O3D::pocket_radius - offsetsmall,
				this->position.y,
				this->position.z + this->widthtable - (3 * O3D::pocket_radius) / 7
			)
		);

	this->objects["BANK5"] =
		new PoolObject(
			"BANK5",
			O3D::CreateBank(
				"BANK5",
				this->position,
				vec3(
					this->position.x + this->widthtable - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 7
				),
				offsetbig,
				offsetbig
			),
			vec3(
				this->position.x + (3 * O3D::pocket_radius) / 7,
				this->position.y,
				this->position.z + this->widthtable - O3D::pocket_radius - offsetbig
			)
		);
	
	this->objects["BANK6"] =
		new PoolObject(
			"BANK5",
			O3D::CreateBank(
				"BANK5",
				this->position,
				vec3(
					this->position.x + this->widthtable - 2 * O3D::pocket_radius,
					this->position.y + O3D::border_height,
					this->position.z - (3 * O3D::pocket_radius) / 7
				),
				offsetbig,
				offsetbig
			),
			vec3(
				this->position.x - (3 * O3D::pocket_radius) / 7 + this->lengthtable,
				this->position.y,
				this->position.z + O3D::pocket_radius + offsetbig
			)
		);
}

Objects3D::PoolPlayer::PoolPlayer(string name)
{
	this->name = name;
	this->active = false;
	this->inpocket = -1;
	this->firsttouched = emptyball;
	this->playercolor = emptyball;
	this->faults = 0;
}

Objects3D::GameLogic::GameLogic()
{
	this->firstplayer = new PoolPlayer("Player I");
	this->firstplayer->active = true;
	this->secondplayer = new PoolPlayer("Player II");
}

Objects3D::GameLogic::~GameLogic()
{
}

void Objects3D::GameLogic::switchplayer()
{
	if (this->firstplayer->active)
	{
		this->statsplayer();

		this->firstplayer->active = false;
		this->firstplayer->firsttouched = emptyball;

		this->secondplayer->active = true;
		cout << endl << this->secondplayer->name << " TURN" << endl;
	}
	else
	{
		this->statsplayer();
		this->secondplayer->active = false;
		this->secondplayer->firsttouched = emptyball;

		this->firstplayer->active = true;
		cout << endl << this->firstplayer->name << " TURN" << endl;
	}
}

void Objects3D::GameLogic::statsplayer()
{
	if (this->firstplayer->active)
	{
		cout << "_____________________" << endl;
		cout << " :: " << this->firstplayer->name << " :: "
			<< endl << "FAULTS          --> " << this->firstplayer->faults
			<< endl << "BALLS ON TABLE --> " << 7 - this->firstplayer->inpocket
			<< endl;
		cout << "_____________________" << endl;
	}
	else
	{
		cout << "_____________________" << endl;
		cout << " :: " << this->secondplayer->name << " :: "
			<< endl << "FAULTS          --> " << this->secondplayer->faults
			<< endl << "BALLS ON TABLE --> " << 7 - this->secondplayer->inpocket
			<< endl;
		cout << "_____________________" << endl;
	}
}

Objects3D::PoolPlayer* Objects3D::GameLogic::getActivePlayer()
{
	if (this->firstplayer->active) return this->firstplayer;
	else return this->secondplayer;
}

Objects3D::PoolPlayer* Objects3D::GameLogic::getInactivePlayer()
{
	if (!this->firstplayer->active) return this->firstplayer;
	else return this->secondplayer;
}
