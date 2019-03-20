#include "O3D.h"

using namespace glm;
using namespace std;

Mesh* O3D::CreateCue(const char *name, const vec3 centerbase, float height)
{
	unsigned short counter = 0;
	float step = (2 * M_PI) / O3D::discretization_factor;
	float smallradius = 0.02f;
	float bigradius = 0.075f;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x,
				centerbase.y,
				centerbase.z),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 0)
		));
	int first = counter;
	for (int index = 0; index <= O3D::discretization_factor; ++index) {
		if (index < O3D::discretization_factor) {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(counter + 1);
		}
		else {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(first + 1);
		}
		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) * smallradius,
					centerbase.y,
					centerbase.z + cos(step * index) * smallradius),
				vec3(1, 0, 0),
				vec3(0, -1, 0),
				vec2(0.5f + sin(step * index) * 0.5f, 0.5f + cos(step * index) * 0.5f)
			));
		counter++;
	}


	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x,
				centerbase.y + height,
				centerbase.z),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(0, 0)
		));
	counter++;
	first = counter;
	for (int index = 0; index <= O3D::discretization_factor; ++index) {
		if (index < O3D::discretization_factor) {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(counter + 1);
		}
		else {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(first + 1);
		}
		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) * bigradius,
					centerbase.y + height,
					centerbase.z + cos(step * index) * bigradius),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(0.5f + sin(step * index) * 0.5f, 0.5f + cos(step * index) * 0.5f)
			));
		counter++;
	}

	for (int index = 0; index <= O3D::discretization_factor; ++index)
	{

		indices.push_back(counter);
		indices.push_back(counter + 2);
		indices.push_back(counter + 1);
		indices.push_back(counter + 3);
		indices.push_back(counter + 2);
		indices.push_back(counter + 1);

		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) * smallradius,
					centerbase.y,
					centerbase.z + cos(step * index) * smallradius),
				vec3(1, 0, 0),
				vec3(cos(step * index), 0, sin(step * index)),
				vec2(index / O3D::discretization_factor, 0)
			));

		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) * bigradius,
					centerbase.y + height,
					centerbase.z + cos(step* index) * bigradius),
				vec3(1, 0, 0),
				vec3(cos(step * index), 0, sin(step * index)),
				vec2(index / O3D::discretization_factor, 1)
			));
		counter += 2;
	}
	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x + sin(step) * 3 * smallradius,
				centerbase.y,
				centerbase.z + cos(step) * smallradius),
			vec3(1, 0, 0),
			vec3(cos(step), 0, sin(step)),
			vec2(0, 0)
		));

	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x + sin(step) * bigradius,
				centerbase.y + height,
				centerbase.z + cos(step) * bigradius),
			vec3(1, 0, 0),
			vec3(cos(step), 0, sin(step)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 2);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 2);
	indices.push_back(counter + 1);

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreateBank(const char *name, const vec3 leftbottom, const vec3 righttop, float offsetleft, float offsetright) {
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	vertices = {
		//down
		VertexFormat(
			vec3(
				leftbottom.x + offsetleft,
				leftbottom.y,
				leftbottom.z),
			vec3(0, 1, 0),
			vec3(0, -1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x - offsetright,
				leftbottom.y,
				leftbottom.z),
			vec3(0, 1, 0),
			vec3(0, -1, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, -1, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, -1, 0),
			vec2(0, 1)
		),
		// face
		VertexFormat(
			vec3(
				leftbottom.x + offsetleft,
				leftbottom.y,
				leftbottom.z),
			vec3(0, 1, 0),
			vec3(0, 0, 1),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x - offsetright,
				leftbottom.y,
				leftbottom.z),
			vec3(0, 1, 0),
			vec3(0, 0, 1),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x - offsetright,
				righttop.y - O3D::offsetup,
				leftbottom.z + O3D::offsetbank),
			vec3(0, 1, 0),
			vec3(0, 0, 1),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x + offsetleft,
				righttop.y - O3D::offsetup,
				leftbottom.z + O3D::offsetbank),
			vec3(0, 1, 0),
			vec3(0, 0, 1),
			vec2(0, 1)
		),
		// right
		VertexFormat(
			vec3(
				righttop.x - offsetright,
				leftbottom.y,
				leftbottom.z),
			vec3(0, 1, 0),
			vec3(1, 0, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(1, 0, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(1, 0, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				righttop.x - offsetright,
				righttop.y - O3D::offsetup,
				leftbottom.z + O3D::offsetbank),
			vec3(0, 1, 0),
			vec3(1, 0, 0),
			vec2(0, 1)
		),
		// back
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, 0, -1),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, 0, -1),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, 0, -1),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, 0, -1),
			vec2(0, 1)
		),
		// left
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(-1, 0, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x + offsetleft,
				leftbottom.y,
				leftbottom.z),
			vec3(0, 1, 0),
			vec3(-1, 0, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x + offsetleft,
				righttop.y - O3D::offsetup,
				leftbottom.z + O3D::offsetbank),
			vec3(0, 1, 0),
			vec3(-1, 0, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(-1, 0, 0),
			vec2(0, 1)
		),
		// up
		VertexFormat(
			vec3(
				leftbottom.x + offsetleft,
				righttop.y - O3D::offsetup,
				leftbottom.z + O3D::offsetbank),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x - offsetright,
				righttop.y - O3D::offsetup,
				leftbottom.z + O3D::offsetbank),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec2(0, 1)
		)
	};

	indices = {
		 0,  1,  3,		 1,  2,  3,
		 4,  5,  7,		 5,  6,  7,
		 8,  9, 11,		 9, 10, 11,
		12, 13, 15,		13, 14, 15,
		16, 17, 19,		17, 18, 19,
		20, 21, 23,		21, 22, 23
	};

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreateCylinder(const char *name, const vec3 centerbase, float height)
{
	unsigned short counter = 0;
	float step = (2 * M_PI) / O3D::discretization_factor;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x,
				centerbase.y,
				centerbase.z),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 0)
		));
	int first = counter;
	for (int index = 0; index <= O3D::discretization_factor; ++index) {
		if (index < O3D::discretization_factor) {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(counter + 1);
		}
		else {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(first + 1);
		}
		vertices.push_back(
			VertexFormat(
				vec3(	centerbase.x + sin(step * index) * O3D::leg_radius,
						centerbase.y,
						centerbase.z + cos(step * index) * O3D::leg_radius),
				vec3(1, 0, 0),
				vec3(0, -1, 0),
				vec2(0.5f + sin(step * index) * 0.5f, 0.5f + cos(step * index) * 0.5f)
			));
		counter++;
	}
	

	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x,
				centerbase.y + height,
				centerbase.z),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(0, 0)
		));
	counter++;
	first = counter;
	for (int index = 0; index <= O3D::discretization_factor; ++index) {
		if (index < O3D::discretization_factor) {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(counter + 1);
		}
		else {
			indices.push_back(first);
			indices.push_back(counter);
			indices.push_back(first + 1);
		}
		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) * O3D::leg_radius,
					centerbase.y + height,
					centerbase.z + cos(step * index) * O3D::leg_radius),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(0.5f + sin(step * index) * 0.5f, 0.5f + cos(step * index) * 0.5f)
			));
		counter++;
	}
	
	for (int index = 0; index <= O3D::discretization_factor; ++index)
	{
		
		indices.push_back(counter);
		indices.push_back(counter + 2);
		indices.push_back(counter + 1);
		indices.push_back(counter + 3);
		indices.push_back(counter + 2);
		indices.push_back(counter + 1);

		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) *  O3D::leg_radius,
					centerbase.y,
					centerbase.z + cos(step * index) * O3D::leg_radius),
				vec3(1, 0, 0),
				vec3(cos(step * index), 0, sin(step * index)),
				vec2(index / O3D::discretization_factor, 0)
			));

		vertices.push_back(
			VertexFormat(
				vec3(centerbase.x + sin(step * index) *  O3D::leg_radius,
					centerbase.y + height,
					centerbase.z + cos(step* index) * O3D::leg_radius),
				vec3(1, 0, 0),
				vec3(cos(step * index), 0, sin(step * index)),
				vec2(index / O3D::discretization_factor, 1)
			));
		counter += 2;
	}
	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x + sin(step) * 3 * O3D::pocket_radius,
				centerbase.y,
				centerbase.z + cos(step) * O3D::leg_radius),
			vec3(1, 0, 0),
			vec3(cos(step), 0, sin(step)),
			vec2(0, 0)
		));

	vertices.push_back(
		VertexFormat(
			vec3(centerbase.x + sin(step) * O3D::leg_radius,
				centerbase.y + height,
				centerbase.z + cos(step) * O3D::leg_radius),
			vec3(1, 0, 0),
			vec3(cos(step), 0, sin(step)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 2);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 2);
	indices.push_back(counter + 1);

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreateTable(const char *name, const vec3 cornertable, const float lengthtable, const float widthtable) 
{
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	vertices = {
		VertexFormat(
			vec3(cornertable.x,
					cornertable.y,
					cornertable.z),
			vec3(0.04, 0.42, 0.01),
			vec3(0, 1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(cornertable.x + lengthtable,
					cornertable.y,
					cornertable.z),
			vec3(0.04, 0.42, 0.01),
			vec3(0, 1, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(cornertable.x + lengthtable,
					cornertable.y,
					cornertable.z + widthtable),
			vec3(0.04, 0.42, 0.01),
			vec3(0, 1, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(cornertable.x,
					cornertable.y,
					cornertable.z + widthtable),
			vec3(0.04, 0.42, 0.01),
			vec3(0, 1, 0),
			vec2(0, 1)
		)
	};
	indices = {
		0, 1, 3,	1, 2, 3
	};

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreateFrustumTable(const char *name, const vec3 cornertable, const float lengthtable, const float widthtable) 
{
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	float height = widthtable / 3;
	vec3 center = vec3(cornertable.x + lengthtable / 2, cornertable.y - height / 2, cornertable.z + widthtable / 2);
	float lengthbase = lengthtable - O3D::offset;
	float widthbase = widthtable - O3D::offset;

	vertices = {
		//face 1
		VertexFormat(
			vec3(cornertable.x - (3 * O3D::pocket_radius) / 2 ,
				cornertable.y,
				cornertable.z - (3 * O3D::pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(cornertable.x + lengthtable + (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z - (3 * pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(center.x + lengthbase / 2,
				cornertable.y - height,
				center.z - widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(center.x - lengthbase / 2,
				cornertable.y - height,
				center.z - widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(0, 1)
		),
		//face 2
		VertexFormat(
			vec3(cornertable.x - (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z - (3 * O3D::pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(cornertable.x - (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z + widthtable + (3 * pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(center.x - lengthbase / 2,
				cornertable.y - height,
				center.z + widthbase / 2),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(center.x - lengthbase / 2,
				cornertable.y - height,
				center.z - widthbase / 2),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(0, 1)
		),
		//face 3
		VertexFormat(
			vec3(cornertable.x - (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z + widthtable + (3 * O3D::pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(cornertable.x + lengthtable + (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z + widthtable + (3 * O3D::pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(center.x + lengthbase / 2,
				cornertable.y - height,
				center.z + widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(center.x - lengthbase / 2,
				cornertable.y - height,
				center.z + widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(0, 1)
		),

		//face 4
		VertexFormat(
			vec3(cornertable.x + lengthtable + (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z + widthtable + (3 * O3D::pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(cornertable.x + lengthtable + (3 * O3D::pocket_radius) / 2,
				cornertable.y,
				cornertable.z - (3 * pocket_radius) / 2),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(center.x + lengthbase / 2,
				cornertable.y - height,
				center.z - widthbase / 2),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(center.x + lengthbase / 2,
				cornertable.y - height,
				center.z + widthbase / 2),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(0, 1)
		),
		//bottom face
		VertexFormat(
			vec3(center.x - lengthbase / 2,
				cornertable.y - height,
				center.z - widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(center.x + lengthbase / 2,
				cornertable.y - height,
				center.z - widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(center.x + lengthbase / 2,
				cornertable.y - height,
				center.z + widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(center.x - lengthbase / 2,
				cornertable.y - height,
				center.z + widthbase / 2),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 1)
		)
	};
	indices = {
		 0,  1,  3,		 1,  2,  3,
		 4,  5,  7,		 5,  6,  7,
		 8,  9, 11,		 9, 10, 11,
		12, 13, 15,		13, 14, 15,
		16, 17, 19,		17, 18, 19
	};

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreatePocketCorner(const char *name, vec3 pocketcenter)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	unsigned short counter = 0;
	float stepcircle = ((3 * M_PI) / 2) / O3D::discretization_factor;

	vec3 corner = vec3(
		pocketcenter.x + (3 * O3D::pocket_radius) / 2,
		pocketcenter.y,
		pocketcenter.z - (3 * O3D::pocket_radius) / 2);

	for (int index = 0; index <= O3D::discretization_factor; ++index)
	{
		if (index < O3D::discretization_factor) {
			indices.push_back(counter);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
			indices.push_back(counter + 3);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
		}
		vertices.push_back(
			VertexFormat(
				vec3(pocketcenter.x + sin(stepcircle * index) *  O3D::pocket_radius,
					corner.y,
					pocketcenter.z + cos(stepcircle * index) *  O3D::pocket_radius),
				vec3(1, 0, 0),
				vec3(-cos(stepcircle * index), 0, -sin(stepcircle * index)),
				vec2(index / O3D::discretization_factor, 0)
			));

		vertices.push_back(
			VertexFormat(
				vec3(pocketcenter.x + sin(stepcircle * index) *  O3D::pocket_radius,
					corner.y + O3D::border_height,
					pocketcenter.z + cos(stepcircle * index) *  O3D::pocket_radius),
				vec3(1, 0, 0),
				vec3(-cos(stepcircle * index), 0, -sin(stepcircle * index)),
				vec2(index / O3D::discretization_factor, 1)
			));
		counter += 2;
	}

	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				corner.y,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				corner.y,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				corner.y + O3D::border_height,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				pocketcenter.y + O3D::border_height,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;

	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(0) *  O3D::pocket_radius,
				corner.y,
				pocketcenter.z + cos(0) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(0) *  O3D::pocket_radius,
				pocketcenter.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;

	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				corner.y,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				corner.y + O3D::border_height,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				corner.z
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;

	float stepbound = (8 * O3D::pocket_radius) / O3D::discretization_factor;

	for (int index = 0; index <= O3D::discretization_factor; ++index)
	{
		if (index < O3D::discretization_factor)
		{
			indices.push_back(counter);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
			indices.push_back(counter + 3);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
		}

		vec3 discretebound = vec3(1);
		vec2 texbound = vec2(1);
		if (index <= (3 * O3D::discretization_factor) / 16) {
			discretebound = vec3(
				pocketcenter.x + sin(0) *  O3D::pocket_radius + stepbound * index,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			);
			texbound = vec2(0, 1 / 3 + index / O3D::discretization_factor);
		}
		if (index > (3 * O3D::discretization_factor) / 16 && index <= O3D::discretization_factor / 2) {
			discretebound = vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius - stepbound * (index - (3 * O3D::discretization_factor) / 16)
			);
			texbound = vec2((index - (3 * O3D::discretization_factor) / 16) / O3D::discretization_factor, 1);
		}
		
		if (index > O3D::discretization_factor / 2 && index <= (13 * O3D::discretization_factor) / 16) {
			discretebound = vec3(corner.x - stepbound * (index - O3D::discretization_factor / 2),
				corner.y + O3D::border_height,
				corner.z
			);
			texbound = vec2(1, 1 - (index - O3D::discretization_factor / 2) / O3D::discretization_factor);
		}
		if (index > (13 * O3D::discretization_factor) / 16 && index <= O3D::discretization_factor) {
			discretebound = vec3(pocketcenter.x + sin(O3D::discretization_factor * stepcircle) *  O3D::pocket_radius,
				corner.y + O3D::border_height,
				corner.z + cos((3 * M_PI) / 2) *  O3D::pocket_radius + stepbound * (index - (13 * O3D::discretization_factor) / 16)
			);
			texbound = vec2(0, 1 - (index - (13 * O3D::discretization_factor) / 16) / O3D::discretization_factor);
		}
		vertices.push_back(
			VertexFormat(
				vec3(pocketcenter.x + sin(stepcircle * index) *  O3D::pocket_radius,
					corner.y + O3D::border_height,
					pocketcenter.z + cos(stepcircle * index) *  O3D::pocket_radius),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(1 / 3 + sin(stepcircle * index) * (1 / 3) , 1/3 + cos(stepcircle * index) * (1 / 3))
			));
		vertices.push_back(
			VertexFormat(
				discretebound,
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				texbound
			));
		counter += 2;
	}

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreatePocketBound(const char *name, vec3 pocketcenter)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	unsigned short counter = 0;
	float stepcircle = M_PI / O3D::discretization_factor;

	vec3 corner = vec3(
		pocketcenter.x + (3 * O3D::pocket_radius) / 2,
		pocketcenter.y,
		pocketcenter.z);

	for (int index = 0; index <= O3D::discretization_factor; ++index)
	{
		if (index < O3D::discretization_factor) {
			indices.push_back(counter);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
			indices.push_back(counter + 3);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
		}
		vertices.push_back(
			VertexFormat(
				vec3(pocketcenter.x + sin(stepcircle * index) *  O3D::pocket_radius,
					corner.y,
					pocketcenter.z + cos(stepcircle * index) *  O3D::pocket_radius),
				vec3(1, 0, 0),
				vec3(-cos(stepcircle * index), 0, -sin(stepcircle * index)),
				vec2(index / O3D::discretization_factor, 0)
			));

		vertices.push_back(
			VertexFormat(
				vec3(pocketcenter.x + sin(stepcircle * index) *  O3D::pocket_radius,
					corner.y + O3D::border_height,
					pocketcenter.z + cos(stepcircle * index) *  O3D::pocket_radius),
				vec3(1, 0, 0),
				vec3(-cos(stepcircle * index), 0, -sin(stepcircle * index)),
				vec2(index / O3D::discretization_factor, 1)
			));
		counter += 2;
	}

	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				corner.y,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius,
				pocketcenter.y + O3D::border_height,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;
	
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(0) *  O3D::pocket_radius,
				corner.y,
				pocketcenter.z + cos(0) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(pocketcenter.x + sin(0) *  O3D::pocket_radius,
				pocketcenter.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;

	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 0)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(stepcircle * O3D::discretization_factor) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(stepcircle * O3D::discretization_factor), 0, sin(stepcircle * O3D::discretization_factor)),
			vec2(1, 1)
		));
	vertices.push_back(
		VertexFormat(
			vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			),
			vec3(1, 0, 0),
			vec3(cos(0), 0, sin(0)),
			vec2(0, 1)
		));
	indices.push_back(counter);
	indices.push_back(counter + 1);
	indices.push_back(counter + 3);
	indices.push_back(counter + 1);
	indices.push_back(counter + 2);
	indices.push_back(counter + 3);
	counter += 4;

	float stepbound = (5 * O3D::pocket_radius) / O3D::discretization_factor;

	for (int index = 0; index <= O3D::discretization_factor; ++index)
	{
		if (index < O3D::discretization_factor)
		{
			indices.push_back(counter);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
			indices.push_back(counter + 3);
			indices.push_back(counter + 2);
			indices.push_back(counter + 1);
		}

		vec3 discretebound = vec3(1);
		vec2 texbound = vec2(1);

		if (index <= (3 * O3D::discretization_factor) / 10) {
			discretebound = vec3(
				pocketcenter.x + sin(0) *  O3D::pocket_radius + stepbound * index,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius
			);
			texbound = vec2(0, index / O3D::discretization_factor);
		}
		if (index > (3 * O3D::discretization_factor) / 10 && index <= (7 * O3D::discretization_factor) / 10) {
			discretebound = vec3(corner.x,
				corner.y + O3D::border_height,
				pocketcenter.z + cos(0) *  O3D::pocket_radius - stepbound * (index - (3 * O3D::discretization_factor) / 10)
			);
			texbound = vec2((index - (3 * O3D::discretization_factor) / 10) / O3D::discretization_factor, 1);
		}

		if (index > (7 * O3D::discretization_factor) / 10 && index <= O3D::discretization_factor) {
			discretebound = vec3(corner.x - stepbound * (index - (7 * O3D::discretization_factor) / 10),
				corner.y + O3D::border_height,
				pocketcenter.z + cos(M_PI) *  O3D::pocket_radius
			);
			texbound = vec2(1, (index - (7 * O3D::discretization_factor) / 10) / O3D::discretization_factor);
		}
		vertices.push_back(
			VertexFormat(
				vec3(pocketcenter.x + sin(stepcircle * index) *  O3D::pocket_radius,
					corner.y + O3D::border_height,
					pocketcenter.z + cos(stepcircle * index) *  O3D::pocket_radius),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(1 / 3 + sin(stepcircle * index) * (1 / 3), 1 / 3 + cos(stepcircle * index) * (1 / 3))
			));
		vertices.push_back(
			VertexFormat(
				discretebound,
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				texbound
			));
		counter += 2;
	}
	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreateBound(const char *name, vec3 leftbottom, vec3 righttop)
{
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	vertices = {
		//face 1
		VertexFormat(
			vec3 (
				leftbottom.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 1)
		),
		// face 2
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, 1),
			vec2(0, 1)
		),
		// face 3
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec2(0, 1)
		),
		// face 4
		VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(0, 1)
		),
		// faec 5
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(-1, 0, 0),
			vec2(0, 1)
		),
		// face 6
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(0, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(1, 0)
		),
		VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(1, 1)
		),
		VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				righttop.z),
			vec3(1, 0, 0),
			vec3(0, 1, 0),
			vec2(0, 1)
		)
	};

	indices = {
		 0,  1,  3,		 1,  2,  3,
		 4,  5,  7,		 5,  6,  7,
		 8,  9, 11,		 9, 10, 11,
		12, 13, 15,		13, 14, 15,
		16, 17, 19,		17, 18, 19,
		20, 21, 23,		21, 22, 23
	};

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLES);
}

Mesh* O3D::CreatePocket(const char *name, vec3 pocketCenter) 
{
	unsigned short counter = 0;
	float step = (2 * M_PI) / O3D::discretization_factor;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	
	vertices.push_back(
		VertexFormat(
			vec3(	pocketCenter.x,
					pocketCenter.y + O3D::epsilon,
					pocketCenter.z),
			vec3(0, 0, 0),
			vec3(0, -1, 0),
			vec2(0, 0)
		));
	indices.push_back(counter++);

	for (int index = 0; index <= O3D::discretization_factor; ++index) {
		indices.push_back(counter++);
		vertices.push_back(
			VertexFormat(
				vec3(	pocketCenter.x + sin(step * index) *  O3D::pocket_radius,
						pocketCenter.y + O3D::epsilon,
						pocketCenter.z + cos(step * index) *  O3D::pocket_radius),
				vec3(0, 0, 0),
				vec3(0, -1, 0),
				vec2(0.5f + sin(step * index) * 0.5f, 0.5f + cos(step * index) * 0.5f)
			));
	}
	indices.push_back(1);
	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_TRIANGLE_FAN);
}

vector<Mesh *> O3D::CreateRoom(const char *name, vec3 leftbottom, vec3 righttop)
{
	vector<Mesh *> room;
	
	{
		vector<VertexFormat> vertices;
		vector<unsigned short> indices;

		vertices = {
			// carpet
			VertexFormat(
				vec3(
					leftbottom.x,
					leftbottom.y,
					leftbottom.z),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(0, 0)
			),
			VertexFormat(
				vec3(
					righttop.x,
					leftbottom.y,
					leftbottom.z),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(1, 0)
			),
			VertexFormat(
				vec3(
					righttop.x,
					leftbottom.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(1, 1)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					leftbottom.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec2(0, 1)
			)
		};

		indices = { 0, 1, 3, 1, 2, 3};

		room.push_back(PoolGameUtilities::CreateMesh("CARPET", vertices, indices, GL_TRIANGLES));
	}

	{
		vector<VertexFormat> vertices;
		vector<unsigned short> indices;
		
		vertices = {
			// face 1
			VertexFormat(
			vec3(
				leftbottom.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(0, 0)
			),
			VertexFormat(
			vec3(
				righttop.x,
				leftbottom.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(1, 0)
			),
			VertexFormat(
			vec3(
				righttop.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(1, 1)
			),
			VertexFormat(
			vec3(
				leftbottom.x,
				righttop.y,
				leftbottom.z),
			vec3(1, 0, 0),
			vec3(0, 0, -1),
			vec2(0, 1)
			),
			// face 
			VertexFormat(
				vec3(
					righttop.x,
					leftbottom.y,
					leftbottom.z),
				vec3(1, 0, 0),
				vec3(-1, 0, 0),
				vec2(0, 0)
			),
			VertexFormat(
				vec3(
					righttop.x,
					leftbottom.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(-1, 0, 0),
				vec2(1, 0)
			),
			VertexFormat(
				vec3(
					righttop.x,
					righttop.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(-1, 0, 0),
				vec2(1, 1)
			),
			VertexFormat(
				vec3(
					righttop.x,
					righttop.y,
					leftbottom.z),
				vec3(1, 0, 0),
				vec3(-1, 0, 0),
				vec2(0, 1)
			),
			// face 3
			VertexFormat(
				vec3(
					righttop.x,
					leftbottom.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(0, 0, 1),
				vec2(0, 0)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					leftbottom.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(0, 0, 1),
				vec2(1, 0)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					righttop.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(0, 0, 1),
				vec2(1, 1)
			),
			VertexFormat(
				vec3(
					righttop.x,
					righttop.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(0, 0, 1),
				vec2(0, 1)
			),
			// faec 4
			VertexFormat(
				vec3(
					leftbottom.x,
					leftbottom.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(1, 0, 0),
				vec2(0, 0)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					leftbottom.y,
					leftbottom.z),
				vec3(1, 0, 0),
				vec3(1, 0, 0),
				vec2(1, 0)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					righttop.y,
					leftbottom.z),
				vec3(1, 0, 0),
				vec3(1, 0, 0),
				vec2(1, 1)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					righttop.y,
					righttop.z),
				vec3(1, 0, 0),
				vec3(1, 0, 0),
				vec2(0, 1)
			)
		};
		indices = { 
		 0,  1,  3,		 1,  2,  3,
		 4,  5,  7,		 5,  6,  7,
		 8,  9, 11,		 9, 10, 11,
		12, 13, 15,		13, 14, 15};

		room.push_back(PoolGameUtilities::CreateMesh("WALLS", vertices, indices, GL_TRIANGLES));
	}

	{
		vector<VertexFormat> vertices;
		vector<unsigned short> indices;

		vertices = {
			// ceiling
			VertexFormat(
				vec3(
					leftbottom.x,
					righttop.y,
					leftbottom.z),
				vec3(1, 1, 1),
				vec3(0, -1, 0),
				vec2(0, 0)
			),
			VertexFormat(
				vec3(
					righttop.x,
					righttop.y,
					leftbottom.z),
				vec3(1, 1, 1),
				vec3(0, -1, 0),
				vec2(1, 0)
			),
			VertexFormat(
				vec3(
					righttop.x,
					righttop.y,
					righttop.z),
				vec3(1, 1, 1),
				vec3(0, -1, 0),
				vec2(1, 1)
			),
			VertexFormat(
				vec3(
					leftbottom.x,
					righttop.y,
					righttop.z),
				vec3(1, 1, 1),
				vec3(0, -1, 0),
				vec2(0, 1)
			)
		};

		indices = { 0, 1, 3, 1, 2, 3 };

		room.push_back(PoolGameUtilities::CreateMesh("CEILING", vertices, indices, GL_TRIANGLES));
	}

	return room;
}

Mesh*  O3D::CreateLine(const char *name, vec3 firstpoint, vec3 secondpoint)
{
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	vertices = {
		VertexFormat(
				vec3(
					firstpoint.x,
					firstpoint.y,
					firstpoint.z),
				vec3(1, 1, 1),
				vec3(0, 1, 0),
				vec2(0, 0)
			),
		VertexFormat(
				vec3(
					secondpoint.x,
					secondpoint.y,
					secondpoint.z),
				vec3(1, 1, 1),
				vec3(0, 1, 0),
				vec2(1, 1)
			),
	};

	indices = {
		0, 1
	};

	return PoolGameUtilities::CreateMesh(name, vertices, indices, GL_LINE_STRIP);
}
