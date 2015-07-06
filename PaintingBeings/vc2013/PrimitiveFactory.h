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
		// Cr�ation d'un TriMesh � partir des vertices, normales, coordonn�es textures
		static TriMesh createTriMesh(std::vector<uint32_t> &indices, const std::vector<Vec3f> &vertices, const std::vector<Vec3f> &normals, const std::vector<Vec2f> &texCoords);
		// Cr�ation d'un face avec la r�solution en param�tre
		static TriMesh createSquare(const int resolution = 1);
		// Cr�ation d'un cube avec la r�solution en param�tre
		static TriMesh createCube(const int resolution = 1);
};

