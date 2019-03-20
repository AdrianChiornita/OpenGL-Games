#pragma once

#include <Core/Engine.h>
#include "../Utilities/Util.h"
#include "BuildingGenerator.h"
#include <vector>
#include <ctime>
#include <queue>
#include <iostream>

class StreetGenerator {
	private:
		static const float STREET_UNIT;
		static const unsigned int MIN_STREET_UNITS;
		static const unsigned int MAX_STREET_UNITS;
		
		static const float ALTITUDE;

		std::vector < std::pair<float, std::vector<float>>> streets;
		std::vector<std::pair<glm::vec2, glm::vec2>> building_rectangles;
		std::vector<std::pair<glm::vec2, glm::vec2>> building_plots;

		unsigned int random_distance() {
			return (	rand() % 
						(MAX_STREET_UNITS - MIN_STREET_UNITS + 1)
				   ) + MIN_STREET_UNITS;
		}

		float next(const float last) {
			return last + random_distance() * STREET_UNIT;
		}

		Mesh* create_intersection(const glm::vec2 center);
		Mesh* create_horizontal_street(const glm::vec2 start, const glm::vec2 end);
		Mesh* create_vertical_street(const glm::vec2 start, const glm::vec2 end);
		Mesh* create_sidewalk(const std::pair<glm::vec2, glm::vec2> sidewalk);

		void create_horizontal_meshes(const std::pair<float, std::vector<float>> street);
		void create_vertical_meshes(const std::pair<float, std::vector<float>> street, const float last_x);

		void generate_streets(const glm::vec2 left_bottom, const glm::vec2 right_top);
		void generate_sidewalks(const float current_x, const std::pair<float, std::vector<float>> street);
		void generate_buildings();

	public:
		std::vector< Mesh *> street_meshes;
		std::vector< Mesh *> sidewalk_meshes;
		std::vector< Building *> building_meshes;
		
		StreetGenerator() {
			srand((unsigned int)time(NULL));
		}

		void generate_street_meshes (const glm::vec2 left_bottom, const glm::vec2 right_top);

};