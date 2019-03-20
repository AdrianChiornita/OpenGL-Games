#include <Core/Engine.h>
#include "O2D.h"

Mesh* O2D::CreateRectangle (std::string name, glm::vec3 leftBottomCorner, float length, float width, 
																		glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		rectangle -> SetDrawMode(GL_LINE_LOOP);
	}  else {
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle -> InitFromData (vertices, indices);
	return rectangle;
}

Mesh * O2D::CreateCircle(std::string name, glm::vec3 circleCenter, float radius, glm::vec3 color, bool fill) {
	
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	unsigned short index = 0;

	Mesh* circle = new Mesh(name);

	if (fill) {
		circle -> SetDrawMode(GL_TRIANGLE_FAN);

		vertices.push_back(VertexFormat(circleCenter, color));
		indices.push_back(index++);

		for (float step = 0; step <= 2 * M_PI; step += 0.001f) {
			indices.push_back(index++);
			vertices.push_back(VertexFormat(glm::vec3(circleCenter.x + sin(step) * radius,
				circleCenter.y + cos(step) * radius,
				0), color));
		}
	}
	else {
		circle->SetDrawMode(GL_LINE_LOOP);

		for (float step = 0; step <= 2 * M_PI; step += 0.001f) {
			indices.push_back(index++);
			vertices.push_back(VertexFormat(glm::vec3(circleCenter.x + sin(step) * radius,
				circleCenter.y + cos(step) * radius,
				0), color));
		}
	}
	indices.push_back(1);

	circle -> InitFromData(vertices, indices);
	return circle;
}
