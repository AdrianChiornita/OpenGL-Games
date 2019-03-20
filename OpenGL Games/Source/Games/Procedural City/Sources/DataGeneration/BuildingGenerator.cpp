#include "BuildingGenerator.h"

Building* BuildingGenerator::create_mesh(glm::vec3 leftbottom, glm::vec3 righttop)
{
	Building* building = new Building({ leftbottom, righttop });
	building->generate_meshes();

	return building;
}

void BuildingGenerator::generate_buildings_on_plot(const std::pair<glm::vec2, glm::vec2> plot)
{
	unsigned int size_x = (unsigned int) ((plot.second.x - plot.first.x) / BUILDING_UNIT);
	unsigned int size_z = (unsigned int) ((plot.second.y - plot.first.y) / BUILDING_UNIT);

	std::vector<std::pair<
		std::pair<unsigned int, unsigned int>,
		std::pair<unsigned int, unsigned int>
		>> pairs = generate_map_on_plot({ size_x, size_z });

	for (unsigned int index = 0; index < pairs.size(); ++index) {
		glm::vec3 left_bottom =
			glm::vec3(
				plot.first.x + pairs[index].first.first * BUILDING_UNIT,
				ALTITUDE,
				plot.first.y + pairs[index].first.second * BUILDING_UNIT
			);
		glm::vec3 right_top =
			glm::vec3(
				plot.first.x + pairs[index].second.first * BUILDING_UNIT,
				ALTITUDE,
				plot.first.y + pairs[index].second.second * BUILDING_UNIT
			);

		meshes.push_back(create_mesh(left_bottom, right_top));
	}
}

std::vector<std::pair<
	std::pair<unsigned int, unsigned int>,
	std::pair<unsigned int, unsigned int>
	>> BuildingGenerator::generate_map_on_plot(const std::pair<unsigned int, unsigned int> start)
{
	std::vector<std::pair<
		std::pair<unsigned int, unsigned int>,
		std::pair<unsigned int, unsigned int>
		>> pieces;
	std::queue<std::pair<
		std::pair<unsigned int, unsigned int>,
		std::pair<unsigned int, unsigned int>
		>> Q;

	Q.push({ { 0, 0}, {start.first, start.second} });

	while (!Q.empty()) {
		std::pair<
			std::pair<unsigned int, unsigned int>,
			std::pair<unsigned int, unsigned int>
		> el = Q.front();
		Q.pop();

		unsigned int size = random(
			min(min(el.second.first - el.first.first, el.second.second - el.first.second), MIN_BUILDING_UNITS),
			min(min(el.second.first - el.first.first, el.second.second - el.first.second), MAX_BUILDING_UNITS)
		);

		unsigned int f = random(el.first.first, el.second.first  - size);
		unsigned int s = random(el.first.second , el.second.second - size);

		pieces.push_back({
			{f, s},
			{f + size, s + size}
			});

		if (s > el.first.second)
			Q.push({
				{el.first.first, el.first.second},
				{el.second.first, s}
				});
		if (s + size < el.second.second)
			Q.push({
				{el.first.first, s + size},
				{el.second.first, el.second.second}
				});
		if (f > el.first.first)
			Q.push({
					{el.first.first, s},
					{f, s + size}
				});
		if (f + size < el.second.first)
			Q.push({
					{f + size, s},
					{el.second.first, s + size}
				});
	}
	return pieces;
}

Mesh * Building::primitive_cuboid(glm::vec3 left_bottom, glm::vec3 right_top)
{
	float size_x = (right_top.x - left_bottom.x) / WINDOW_UNIT;
	float size_y = (right_top.y - left_bottom.y) / WINDOW_UNIT;
	float size_z = (right_top.z - left_bottom.z) / WINDOW_UNIT;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices = {
		// face 1
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, size_y)
		),
		// face 2
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(0, size_y)
		),
		// face 3
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, size_y)
		),
		// faec 4
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(0, size_y)
		),
	};

	indices = {
		 0,  1,  3,		 1,  2,  3,
		 4,  5,  7,		 5,  6,  7,
		 8,  9, 11,		 9, 10, 11,
		12, 13, 15,		13, 14, 15
	};

	return  ProceduralCityUtil::CreateMesh("cuboid", vertices, indices, GL_TRIANGLES);
}

Mesh * Building::primitive_park(glm::vec3 left_bottom, glm::vec3 right_top)
{
	float size_x = (right_top.x - left_bottom.x) / WINDOW_UNIT;
	float size_y = (right_top.y - left_bottom.y) / WINDOW_UNIT;
	float size_z = (right_top.z - left_bottom.z) / WINDOW_UNIT;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices = {
		// face 1
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec2(0, size_y)
		),
		// face 2
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec2(0, size_y)
		),
		// face 3
		VertexFormat(
			glm::vec3(
				right_top.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec2(0, size_y)
		),
		// faec 4
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				left_bottom.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(size_x, size_y)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec2(0, size_y)
		),
		//face 5
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size_x, size_z)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				right_top.z),
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

	return  ProceduralCityUtil::CreateMesh("park", vertices, indices, GL_TRIANGLES);
}

Mesh * Building::top_cuboid(glm::vec3 left_bottom, glm::vec3 right_top)
{
	float size_x = (right_top.x - left_bottom.x) / WINDOW_UNIT;
	float size_z = (right_top.z - left_bottom.z) / WINDOW_UNIT;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices = {
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				left_bottom.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size_x, 0)
		),
		VertexFormat(
			glm::vec3(
				right_top.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(size_x, size_z)
		),
		VertexFormat(
			glm::vec3(
				left_bottom.x,
				right_top.y,
				right_top.z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec2(0, size_z)
		)
	};

	indices = {
		 0,  1,  3,		 1,  2,  3
	};

	return  ProceduralCityUtil::CreateMesh("top", vertices, indices, GL_TRIANGLES);
}

Mesh * Building::primitive_cylinder(glm::vec3 center, float height, float radius)
{
	float circle_length = 2 * M_PI * radius;
	float tx = (RADIANS(10.0f) * -radius) / WINDOW_UNIT, ty = height / WINDOW_UNIT;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	unsigned short index = 0;


	int count = 0;
	for (float step = 0; step < 2 * M_PI; step += RADIANS(10.0f)) {
		indices.push_back(index++);
		indices.push_back(index++);

		if (first == count) {
			step += (f_angle - RADIANS(10.0f));
			tx += (f_angle * radius) / WINDOW_UNIT;
		}
		else if (second == count) {
			step += (s_angle - RADIANS(10.0f));
			tx += (s_angle * radius) / WINDOW_UNIT;
		}
		else
			tx += (RADIANS(10.0f) * radius) / WINDOW_UNIT;

		vertices.push_back(
			VertexFormat(
				glm::vec3(
					center.x + sin(step) * radius,
					center.y,
					center.z + cos(step) * radius
				),
				glm::vec3(1, 0, 0),
				glm::vec3(sin(step), 0, cos(step)),
				glm::vec2(
					tx,
					0
				)
			)
		);

		vertices.push_back(
			VertexFormat(
				glm::vec3(
					center.x + sin(step) * radius,
					center.y + height,
					center.z + cos(step) * radius
				),
				glm::vec3(1, 0, 0),
				glm::vec3(sin(step), 0, cos(step)),
				glm::vec2(
					tx,
					ty
				)
			)
		);
		count++;
	}

	return  ProceduralCityUtil::CreateMesh("cylinder", vertices, indices, GL_QUAD_STRIP);
}

Mesh * Building::top_cylinder(glm::vec3 center, float height, float radius)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	unsigned short index = 0;

	vertices.push_back(VertexFormat(
		glm::vec3(
			center.x,
			height + ALTITUDE,
			center.z
		),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec2(0.5f, 0.5f)
	)
	);
	indices.push_back(index++);

	int count = 0;
	for (float step = 0; step < 2 * M_PI; step += RADIANS(10.0f)) {
		indices.push_back(index++);

		if (first == count) step += (f_angle - RADIANS(10.0f));
		else if (second == count) step += (s_angle - RADIANS(10.0f));

		vertices.push_back(
			VertexFormat(
				glm::vec3(
					center.x + sin(step) * radius,
					height + ALTITUDE,
					center.z + cos(step) * radius
				),
				glm::vec3(1, 0, 0),
				glm::vec3(0, 1, 0),
				glm::vec2(
					(cos(step) + 1) * 0.5f,
					(sin(step) + 1) * 0.5f
				)
			)
		);
		count++;
	}
	indices.push_back(1);

	return  ProceduralCityUtil::CreateMesh("top", vertices, indices, GL_TRIANGLE_FAN);
}

void Building::generate_meshes()
{
	float offset = 0.003f;
	footprint.first.x += offset;
	footprint.first.z += offset;
	footprint.second.x -= offset;
	footprint.second.x -= offset;

	if (type == rounded)
	{
		float radius = (footprint.second.x - footprint.first.x) / 2;

		glm::vec3 center =
			glm::vec3(
				footprint.first.x + (footprint.second.x - footprint.first.x) / 2,
				ALTITUDE,
				footprint.first.z + (footprint.second.z - footprint.first.z) / 2
			);

		if (random(0, 100) < 70) {
			first = random(2, 18) / 2;
			f_angle = RADIANS(random(2, 3) * 30.0f);
		}
		if (random(0, 100) < 70) {
			second = random(19, 34) / 2;
			s_angle = RADIANS(random(2, 3) * 30.0f);
		}

		mesh.push_back(primitive_cylinder(center, height, radius));
		mesh.push_back(top_cylinder(center, height, radius));
	}
	else if (type == classic)
	{
		glm::vec3 left_bottom = footprint.first;
		glm::vec3 right_top = footprint.second;

		right_top.y = ALTITUDE;
		float H = height;
		float S = right_top.x - left_bottom.x;

		while (H > 0 && S > WINDOW_UNIT / 4) {
			float seq = random(MIN_HEIGHT_UNITS / 2, height / (2 * WINDOW_UNIT));

			right_top.y += seq * WINDOW_UNIT;
			mesh.push_back(primitive_cuboid(left_bottom, right_top));
			mesh.push_back(top_cuboid(left_bottom, right_top));
			left_bottom.y += seq * WINDOW_UNIT;

			float offset = (frandom(1.0f, 2.0f) * S) / 10.0f;
			
			right_top.x -= offset;
			right_top.z -= offset;
			left_bottom.x += offset;
			left_bottom.z += offset;

			H -= seq;
			S -= 2 * offset;
		}
	}
	else if (type == blocky)
	{
		glm::vec3 center =
			glm::vec3(
				footprint.first.x + (footprint.second.x - footprint.first.x) / 2,
				ALTITUDE,
				footprint.first.z + (footprint.second.z - footprint.first.z) / 2
			);
		
		glm::vec3 left_bottom, right_top;
		
		float H = height;
		float S = footprint.second.x - footprint.first.x;
		float offset;

		glm::vec2 max_width  = glm::vec2(center.x, center.z);
		glm::vec2 max_length = glm::vec2(center.x, center.z);

		int count = 0;
		while (H > height / 4 && count < 4) {
			left_bottom = footprint.first;
			right_top = footprint.second;

			float seq = random(MIN_HEIGHT_UNITS / 2, 3 * MIN_HEIGHT_UNITS);

			offset = (frandom(0.0f, 4.0f) * S) / 10.0f;
			if (right_top.x - offset < center.x) continue;
			right_top.x -= offset;

			offset = (frandom(0.0f, 4.0f) * S) / 10.0f;
			if (right_top.z - offset < center.z) continue;
			right_top.z -= offset;

			offset = (frandom(0.0f, 4.0f) * S) / 10.0f;
			if (left_bottom.x + offset > center.x) continue;
			left_bottom.x += offset;

			offset = (frandom(0.0f, 4.0f) * S) / 10.0f;
			if (left_bottom.z + offset > center.z) continue;
			left_bottom.z += offset;

			bool del = true;
			if (left_bottom.x - max_length.x < WINDOW_UNIT / 8) {
				max_length.x = left_bottom.x;
				del = false;
			}
			if (right_top.x - max_length.y < WINDOW_UNIT / 8) {
				max_length.y = right_top.x;
				del = false;
			}
			if (left_bottom.z - max_width.x < WINDOW_UNIT / 8) {
				max_width.x = left_bottom.z;
				del = false;
			}
			if (right_top.z - max_width.y < WINDOW_UNIT / 8) {
				max_width.y = right_top.z;
				del = false;
			}
			if (del) continue;
			if (right_top.z - left_bottom.z < WINDOW_UNIT / 4 || right_top.x - left_bottom.x < WINDOW_UNIT / 4)
				continue;

			right_top.y = ALTITUDE + H;
			mesh.push_back(primitive_cuboid(left_bottom, right_top));
			mesh.push_back(top_cuboid(left_bottom, right_top));
			H -= seq;

			if (max_width.x == footprint.first.z) count++;
			if (max_width.y == footprint.second.z) count++;
			if (max_length.x == footprint.first.x) count++;
			if (max_length.y == footprint.second.x) count++;
		}
	}
	else if (type == standard)
	{
		mesh.push_back(primitive_cuboid(footprint.first, footprint.second));
		mesh.push_back(top_cuboid(footprint.first, footprint.second));
	}
	else if (type == park)
	{
		float offset = 0.005f;
		mesh.push_back(primitive_park(
			glm::vec3(
				footprint.first.x + offset,
				footprint.first.y,
				footprint.first.z + offset),
			glm::vec3(
				footprint.second.x - offset,
				footprint.first.y + offset,
				footprint.second.z - offset))
		);
	}

	footprint.first.x -= offset;
	footprint.first.z -= offset;
	footprint.second.x += offset;
	footprint.second.x += offset;
}
