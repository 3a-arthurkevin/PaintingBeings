#include "PrimitiveFactory.h"

TriMesh PrimitiveFactory::createTriMesh(std::vector<uint32_t> &indices, const std::vector<Vec3f> &vertices, const std::vector<Vec3f> &normals, const std::vector<Vec2f> &texCoords)
{
	TriMesh mesh;

	if (indices.size() > 0)
		mesh.appendIndices(&(indices[0]), indices.size());

	if (vertices.size() > 0)
		mesh.appendVertices(&(vertices[0]), vertices.size());

	if (normals.size() > 0)
		mesh.appendNormals(&(normals[0]), normals.size());

	if (texCoords.size() > 0)
		mesh.appendTexCoords(&(texCoords[0]), texCoords.size());

	return mesh;
}

TriMesh PrimitiveFactory::createSquare(const int resolution)
{
	std::vector<uint32_t> indices;
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texCoords;

	Vec3f normal(0.0f, 0.0f, 1.0f);

	float res = 1.0f/ static_cast<float>(resolution);

	uint32_t index = 0;

	for (int x = 0; x < resolution; ++x)
	{
		for (int y = 0; y < resolution; (++y, index++))
		{
			/*
			O (origin) = pA

			(x1;y1)
			 --------pB
			|         |
			|         |
			|		  |
			pA--------
			(x0;y0)
			*/

			float xA = static_cast<float>(x) * res;
			float yA = static_cast<float>(y) * res;
			float xB = static_cast<float>(x + 1) * res;
			float yB = static_cast<float>(y + 1) * res;

			/*
			OpenGL Triangle Strip
			Square ==> p2.p0.p1 and p1.p2.p3

			(xA;yB)      (xB;yB)
			p2-------p3
			| \       |
			|    \    |
			|	    \ |
			p0-------p1
			(xA;yA)      (xB;yA)
			*/

			// -0.5 --> to make the origin the gravity center of the cube

			Vec3f p0(xA - 0.5f, yA - 0.5f, 0.0f);
			Vec3f p1(xB - 0.5f, yA - 0.5f, 0.0f);
			Vec3f p2(xA - 0.5f, yB - 0.5f, 0.0f);
			Vec3f p3(xB - 0.5f, yB - 0.5f, 0.0f);

			/*
			Tex Coord
			
		  (0,0)     (0,1)
			-----------
			| \       |
			|    \    |
			|       \ |
		    -----------
	      (0,1)     (1,1)

			0 <= xA < xB <= 1
			0 <= yA < yB <= 1

			*/

			Vec2f texCoord0(xA, yA);
			Vec2f texCoord1(xB, yA);
			Vec2f texCoord2(xA, yB);
			Vec2f texCoord3(xB, yB);


			vertices.push_back(p2);
			vertices.push_back(p1);
			vertices.push_back(p0);
			//
			vertices.push_back(p1);
			vertices.push_back(p2);
			vertices.push_back(p3);

			//-----------------------

			texCoords.push_back(texCoord2);
			texCoords.push_back(texCoord1);
			texCoords.push_back(texCoord0);
			//
			texCoords.push_back(texCoord1);
			texCoords.push_back(texCoord2);
			texCoords.push_back(texCoord3);

			//-----------------------

			for (uint32_t i = 0; i < 6; ++i)
			{
				indices.push_back((index * 6) + i);

				normals.push_back(normal);
			}
		}
	}

	TriMesh mesh = PrimitiveFactory::createTriMesh(indices, vertices, normals, texCoords);

	indices.clear();
	vertices.clear();
	normals.clear();
	texCoords.clear();

	return mesh;
}

TriMesh PrimitiveFactory::createCube(const int resolution)
{
	std::vector<uint32_t> indices;
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texCoords;

	ci::TriMesh frontFace = createSquare(resolution);

	float pi = static_cast<float>(M_PI);

	Vec3f normal;
	Vec3f offset;
	Matrix44f transform;

	std::vector<Vec3f>::iterator iterPosition;
	std::vector<Vec2f>::iterator iterTexCoord;

	//Front Face
	normal = Vec3f(0.0f, 0.0f, 1.0f);
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate(offset);
	for (iterPosition = frontFace.getVertices().begin(); iterPosition != frontFace.getVertices().end(); ++iterPosition)
	{
		vertices.push_back(transform.transformPoint(*iterPosition));

		normals.push_back(normal);
	}
	for (iterTexCoord = frontFace.getTexCoords().begin(); iterTexCoord != frontFace.getTexCoords().end(); ++iterTexCoord)
	{
		texCoords.push_back(*iterTexCoord);
	}

	//Left Face
	normal = Vec3f(-1.0f, 0.0f, 0.0f);
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate(offset);
	transform.rotate(Vec3f(0.0f, -(pi * 0.5f), 0.0f));
	for (iterPosition = frontFace.getVertices().begin(); iterPosition != frontFace.getVertices().end(); ++iterPosition)
	{
		vertices.push_back(transform.transformPoint(*iterPosition));

		normals.push_back(normal);
	}
	for (iterTexCoord = frontFace.getTexCoords().begin(); iterTexCoord != frontFace.getTexCoords().end(); ++iterTexCoord)
	{
		texCoords.push_back(*iterTexCoord);
	}

	//Back Face
	normal = Vec3f(0.0f, 0.0f, -1.0f);
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate(offset);
	transform.rotate(Vec3f(0.0f, pi, 0.0f));
	for (iterPosition = frontFace.getVertices().begin(); iterPosition != frontFace.getVertices().end(); ++iterPosition)
	{
		vertices.push_back(transform.transformPoint(*iterPosition));

		normals.push_back(normal);
	}
	for (iterTexCoord = frontFace.getTexCoords().begin(); iterTexCoord != frontFace.getTexCoords().end(); ++iterTexCoord)
	{
		texCoords.push_back(*iterTexCoord);
	}

	//Right Face
	normal = Vec3f(1.0f, 0.0f, 0.0f);
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate(offset);
	transform.rotate(Vec3f(0.0f, (pi * 0.5f), 0.0f));
	for (iterPosition = frontFace.getVertices().begin(); iterPosition != frontFace.getVertices().end(); ++iterPosition)
	{
		vertices.push_back(transform.transformPoint(*iterPosition));

		normals.push_back(normal);
	}
	for (iterTexCoord = frontFace.getTexCoords().begin(); iterTexCoord != frontFace.getTexCoords().end(); ++iterTexCoord)
	{
		texCoords.push_back(*iterTexCoord);
	}

	//Top Face
	normal = Vec3f(0.0f, 1.0f, 0.0f);
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate(offset);
	transform.rotate(Vec3f((pi * 0.5f), 0.0f, 0.0f));
	for (iterPosition = frontFace.getVertices().begin(); iterPosition != frontFace.getVertices().end(); ++iterPosition)
	{
		vertices.push_back(transform.transformPoint(*iterPosition));

		normals.push_back(normal);
	}
	for (iterTexCoord = frontFace.getTexCoords().begin(); iterTexCoord != frontFace.getTexCoords().end(); ++iterTexCoord)
	{
		texCoords.push_back(*iterTexCoord);
	}

	//Bottom Face
	normal = Vec3f(0.0f, -1.0f, 0.0f);
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate(offset);
	transform.rotate(Vec3f(-(pi * 0.5f), 0.0f, 0.0f));
	for (iterPosition = frontFace.getVertices().begin(); iterPosition != frontFace.getVertices().end(); ++iterPosition)
	{
		vertices.push_back(transform.transformPoint(*iterPosition));

		normals.push_back(normal);
	}
	for (iterTexCoord = frontFace.getTexCoords().begin(); iterTexCoord != frontFace.getTexCoords().end(); ++iterTexCoord)
	{
		texCoords.push_back(*iterTexCoord);
	}


	for (uint32_t i = 0; i < vertices.size(); ++i) {
		indices.push_back(i);
	}

	TriMesh mesh = PrimitiveFactory::createTriMesh(indices, vertices, normals, texCoords);

	indices.clear();
	vertices.clear();
	normals.clear();
	texCoords.clear();

	return mesh;
}


