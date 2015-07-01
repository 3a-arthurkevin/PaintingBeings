#pragma once

#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"

#include "cinder/TriMesh.h";

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h";

using namespace ci;

class PrimitiveFactory
{
	public:
		static TriMesh createTriMesh(std::vector<uint32_t> &indices, const std::vector<Vec3f> &vertices, const std::vector<Vec3f> &normals, const std::vector<Vec2f> &texCoords);

		static TriMesh createSquare(const int resolution = 1);

		static TriMesh createCube(const int resolution = 1);
};

