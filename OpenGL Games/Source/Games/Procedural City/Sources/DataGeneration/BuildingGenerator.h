#pragma once

#include <Core/Engine.h>
#include "../Utilities/Util.h"
#include <vector>
#include <ctime>
#include <queue>
#include <iostream>

#define min(a,b) (((a) < (b)) ? (a) : (b))

constexpr float WINDOW_UNIT = 0.5f;
constexpr float BUILDING_UNIT = 0.5f;
constexpr unsigned int MIN_BUILDING_UNITS = 1;
constexpr unsigned int MAX_BUILDING_UNITS = 4;
constexpr unsigned int MIN_HEIGHT_UNITS = 2;
constexpr unsigned int MAX_HEIGHT_UNITS = 8;
constexpr float ALTITUDE = 0.13f;

typedef enum buildingtype {
	blocky, rounded, classic, standard, park
} buildingtype;

class Building {
	private:
		float height;
		buildingtype type;

		int first, second;
		float f_angle, s_angle;

		Mesh* primitive_cuboid(glm::vec3 left_bottom, glm::vec3 right_top);
		Mesh* primitive_park(glm::vec3 left_bottom, glm::vec3 right_top);
		Mesh* top_cuboid(glm::vec3 left_bottom, glm::vec3 right_top);
		Mesh* primitive_cylinder(glm::vec3 center, float height, float radius);
		Mesh* top_cylinder(glm::vec3 center, float height, float radius);

		unsigned int random(const unsigned int lower, const unsigned int upper) {
			return (rand() %
				(upper - lower + 1)
				) + lower;
		}

		float frandom(const float lower, const float upper) {
			return lower + 
				static_cast <float> (rand()) / 
				(static_cast <float> (RAND_MAX / (upper - lower))
					);
		}

	public:
		std::pair<glm::vec3, glm::vec3> footprint;
		std::vector<Mesh *> mesh;

		Building(std::pair<glm::vec3, glm::vec3> footprint) {
			this->footprint = footprint;
			this->height = random(MIN_HEIGHT_UNITS, MAX_HEIGHT_UNITS) * WINDOW_UNIT;
			this->footprint.second.y += this->height;

			first = second = -1;
			f_angle = s_angle = -1.0f;
			
			if (height == MIN_HEIGHT_UNITS * WINDOW_UNIT)
			{
				if (random(0, 100) < 70)
					type = standard;
				else
					type = park;
			}
			else {
				type = (buildingtype) random(blocky, classic);
			}
		}

		void generate_meshes();
};

class BuildingGenerator {
private:
	Building* create_mesh(glm::vec3 leftbottom, glm::vec3 righttop);

public:
	std::vector<Building *> meshes;

	BuildingGenerator() {
		srand((unsigned int)time(NULL));
	}

	unsigned int random(const unsigned int lower, const unsigned int upper) {
		return (rand() %
			(upper - lower + 1)
			) + lower;
	}

	void generate_buildings_on_plot(const std::pair<glm::vec2, glm::vec2> plot);

	std::vector<std::pair<
		std::pair<unsigned int, unsigned int>,
		std::pair<unsigned int, unsigned int>
		>> generate_map_on_plot(const std::pair<unsigned int, unsigned int> start);
};

