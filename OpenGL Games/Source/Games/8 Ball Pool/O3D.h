#pragma once

#include <climits>

#include "Utilities.h"

using namespace glm;

namespace O3D {
	
	constexpr float pocket_radius = 0.25f;
	constexpr float leg_radius = 0.9f;
	constexpr float epsilon = 0.001f;
	constexpr float offset = 0.3f;
	constexpr float offsetup = 0.03f;
	constexpr float offsetbank = 0.05f;
	constexpr float border_height = 0.15f;
	constexpr float discretization_factor = 1000.0f;

	Mesh* CreateCue(const char *name, const vec3 centerbase, float height);
	Mesh* CreateBank(const char *name, const vec3 leftbottom, const vec3 righttop, float offsetleft, float offsetright);
	Mesh* CreateCylinder(const char *name, const vec3 centerbase, float height);
	Mesh* CreateTable(const char *name, const vec3 cornertable, const float lengthtable, const float widthtable);
	Mesh* CreateFrustumTable(const char *name, const vec3 cornertable, const float lengthtable, const float widthtable);
	Mesh * CreatePocket(const char *name, vec3 pocketCenter);
	Mesh* CreatePocketCorner(const char *name, vec3 pocketcenter);
	Mesh* CreatePocketBound(const char *name, vec3 pocketcenter);
	Mesh* CreateBound(const char *name, vec3 leftbottom, vec3 righttop);
	Mesh* CreateLine(const char *name, vec3 firstpoint, vec3 secondpoint);
	vector<Mesh *> CreateRoom(const char *name, vec3 leftbottom, vec3 righttop);
}

