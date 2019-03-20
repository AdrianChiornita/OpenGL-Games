#pragma once

#include <Core/Engine.h>

namespace ProceduralCityUtil {

    inline Mesh * CreateMesh(const char * name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices, const unsigned int primitive) {

		unsigned int VAO = 0, VBO = 0, IBO = 0;

		// Create the VAO and bind it
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Create the VBO and bind it
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Send vertices data into the VBO buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// Create the IBO and bind it
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		// Send indices data into the IBO buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// ========================================================================
		// This section describes how the GPU Shader Vertex Shader program receives data

		// set vertex position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);
		// set vertex normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));
		// set texture coordinate attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));
		// set vertex color attribute
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
		// ========================================================================

		// Unbind the VAO
		glBindVertexArray(0);
		// Check for OpenGL errors
		CheckOpenGLError();

		// Mesh information is saved into a Mesh object
		Mesh* mesh = new Mesh(name);
		mesh->SetDrawMode(primitive);
		mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
		mesh->vertices = vertices;
		mesh->indices = indices;
		return mesh;
	}
}