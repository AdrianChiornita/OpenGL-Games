#include "StreetGeneration.h"

const float StreetGenerator::ALTITUDE = 0.1f;
const float StreetGenerator::STREET_UNIT = 1.0f;
const unsigned int StreetGenerator::MAX_STREET_UNITS = 6;
const unsigned int StreetGenerator::MIN_STREET_UNITS = 3;

void StreetGenerator::generate_streets(const glm::vec2 left_bottom, const glm::vec2 right_top)
{
	streets.push_back({ left_bottom.x, {left_bottom.y, right_top.y } });

	while (right_top.x - streets.back().first > MAX_STREET_UNITS *  STREET_UNIT) {
		float highway = next(streets.back().first);

		if (right_top.x - highway > MIN_STREET_UNITS *  STREET_UNIT) {
			std::vector<float> intersections = { left_bottom.y };

			while (right_top.y - intersections.back() > MAX_STREET_UNITS * STREET_UNIT) {
				float intersection = next(intersections.back());

				if (right_top.y - intersection >= MIN_STREET_UNITS * STREET_UNIT)
					intersections.push_back(intersection);
			}

			intersections.push_back(right_top.y);
			streets.push_back({highway, intersections});
		}
	}

	std::vector<float> intersections = { left_bottom.y };

	while (right_top.y - intersections.back() > MAX_STREET_UNITS * STREET_UNIT) {
		float intersection = next(intersections.back());

		if (right_top.y - intersection >= MIN_STREET_UNITS * STREET_UNIT)
			intersections.push_back(intersection);
	}

	intersections.push_back(right_top.y);
	streets.push_back({ right_top.x, intersections });
}

void StreetGenerator::generate_street_meshes(const glm::vec2 left_bottom, const glm::vec2 right_top)
{
	generate_streets(left_bottom, right_top);

	for (unsigned int index = 0; index < streets.size(); ++index) {
		
		if (index != 0) {
			create_vertical_meshes(streets[index], streets[index - 1].first);
		}

		if (index != streets.size() - 1) {
			generate_sidewalks(streets[index].first, streets[index + 1]);

			streets[index].second.insert(
				streets[index].second.end(),
				streets[index + 1].second.begin(),
				streets[index + 1].second.end()
			);
			std::sort(streets[index].second.begin(), streets[index].second.end());
		}
		create_horizontal_meshes(streets[index]);
	}

	for (int index = 0; index < building_rectangles.size(); ++index) {
		sidewalk_meshes.push_back(create_sidewalk(building_rectangles[index]));
	}
	generate_buildings();
}

void StreetGenerator::generate_sidewalks(const float current_x, const std::pair<float, std::vector<float>> street)
{
	float offset = STREET_UNIT / 2;

	for (unsigned int index = 0; index < street.second.size() - 1; ++index) {
		building_rectangles.push_back({
				glm::vec2(
					current_x + offset,
					street.second[index] + offset
				),
				glm::vec2(
					street.first - offset,
					street.second[index + 1] -offset
				)
			});
	}
}

void StreetGenerator::generate_buildings()
{
	float offset = STREET_UNIT / 4;

	for (int index = 0; index < building_rectangles.size(); ++index) {
		building_plots.push_back({
				glm::vec2(
					building_rectangles[index].first.x + offset,
					building_rectangles[index].first.y + offset
				),
				glm::vec2(
					building_rectangles[index].second.x - offset,
					building_rectangles[index].second.y - offset
				)
			});
	}
	BuildingGenerator* bgenerator = new BuildingGenerator();

	for (int index = 0; index < building_rectangles.size(); ++index) {
		bgenerator->generate_buildings_on_plot(building_plots[index]);
	}

	building_meshes.insert(
		building_meshes.end(),
		bgenerator->meshes.begin(),
		bgenerator->meshes.end()
	);
}

void StreetGenerator::create_horizontal_meshes(const std::pair<float, std::vector<float>> street)
{
	for (unsigned int index = 0; index < street.second.size() - 1; ++index) {
		if (street.second[index] != street.second[index + 1]) {
			street_meshes.push_back(create_intersection(
				glm::vec2(street.first, street.second[index])
			));
			street_meshes.push_back(create_horizontal_street(
				glm::vec2(street.first, street.second[index]),
				glm::vec2(street.first, street.second[index + 1])
			));
		}
	}
	street_meshes.push_back(create_intersection(
		glm::vec2(street.first, street.second[street.second.size() - 1])
	));
}

void StreetGenerator::create_vertical_meshes(const std::pair<float, std::vector<float>> street, const float last_x)
{
	for (unsigned int index = 0; index < street.second.size(); ++index) {
			street_meshes.push_back(create_vertical_street(
				glm::vec2(street.first, street.second[index]),
				glm::vec2(last_x, street.second[index])
			));
	}
}

Mesh * StreetGenerator::create_intersection(const glm::vec2 center)
{
	float offset = STREET_UNIT / 2;

	std::vector<VertexFormat> vertices = {
		VertexFormat(
			glm::vec3(center.x - offset, ALTITUDE, center.y - offset),
			glm::vec3( 1, 0, 0),
			glm::vec3( 0, 1, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(center.x + offset, ALTITUDE, center.y - offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 1)
		),
		VertexFormat(
			glm::vec3(center.x + offset, ALTITUDE, center.y + offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(1, 1)
		),
		VertexFormat(
			glm::vec3(center.x - offset, ALTITUDE, center.y + offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(1, 0)
		)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,	2, 3, 0
	};

	return ProceduralCityUtil::CreateMesh("intersection", vertices, indices, GL_TRIANGLES);
}

Mesh * StreetGenerator::create_horizontal_street(const glm::vec2 start, const glm::vec2 end)
{

	float offset = STREET_UNIT / 2;
	float size = ((int) (end.y - start.y)) / STREET_UNIT;

	std::vector<VertexFormat> vertices = {
		VertexFormat(
			glm::vec3(start.x - offset, ALTITUDE, start.y + offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(start.x + offset, ALTITUDE, start.y + offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 1)
		),
		VertexFormat(
			glm::vec3(end.x + offset, ALTITUDE, end.y - offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size, 1)
		),
		VertexFormat(
			glm::vec3(end.x - offset, ALTITUDE, end.y - offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size, 0)
		)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,	2, 3, 0
	};

	return ProceduralCityUtil::CreateMesh("horizontal_street", vertices, indices, GL_TRIANGLES);
}

Mesh * StreetGenerator::create_vertical_street(const glm::vec2 start, const glm::vec2 end)
{
	float offset = STREET_UNIT / 2;
	float size = ((int)(end.x - start.x)) / STREET_UNIT;

	std::vector<VertexFormat> vertices = {
		VertexFormat(
			glm::vec3(start.x - offset, ALTITUDE, start.y + offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(start.x - offset, ALTITUDE, start.y - offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 1)
		),
		VertexFormat(
			glm::vec3(end.x + offset, ALTITUDE, end.y - offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size, 1)
		),
		VertexFormat(
			glm::vec3(end.x + offset, ALTITUDE, end.y + offset),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size, 0)
		)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,	2, 3, 0
	};

	return ProceduralCityUtil::CreateMesh("vertical_street", vertices, indices, GL_TRIANGLES);
}

Mesh* StreetGenerator::create_sidewalk(const std::pair<glm::vec2, glm::vec2> sidewalk)
{
	float offset = 0.03f;
	glm::vec3 leftbottom = glm::vec3(sidewalk.first.x, ALTITUDE, sidewalk.first.y);
	glm::vec3 righttop = glm::vec3(sidewalk.second.x, ALTITUDE + offset, sidewalk.second.y);

	float size_x = (sidewalk.second.x - sidewalk.first.x) / (STREET_UNIT / 4);
	float size_z = (sidewalk.second.y - sidewalk.first.y) / (STREET_UNIT / 4);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices = {
		// face 1
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				leftbottom.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				leftbottom.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, offset)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, offset)
		),
		// face 2
		VertexFormat(
			glm::vec3(
				righttop.x,
				leftbottom.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(size_x, offset)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(0, offset)
		),
		// face 3
		VertexFormat(
			glm::vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(size_x, offset)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(0, offset)
		),
		// faec 4
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				leftbottom.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(size_x, offset)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(0, offset)
		),
		// face 5
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size_x, size_z)
		),
		VertexFormat(
			glm::vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, size_z)
		)
	};

	indices = {
		 0,  1,  3,		 1,  2,  3,
		 4,  5,  7,		 5,  6,  7,
		 8,  9, 11,		 9, 10, 11,
		12, 13, 15,		13, 14, 15,
		16, 17, 19,		17, 18, 19
	};

	return  ProceduralCityUtil::CreateMesh("sidewalk", vertices, indices, GL_TRIANGLES);
}