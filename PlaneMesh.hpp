#ifndef PLANEMESH_HPP
#define PLANEMESH_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderUtils.hpp"

class PlaneMesh
{
public:
	std::vector<float> verts;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	float min, max;
	int numVerts;
	int numIndices;
	GLuint VAO, VBO, EBO;
	GLuint programID;

	PlaneMesh(float min, float max, float stepsize) : min(min), max(max)
	{
		planeMeshQuads(min, max, stepsize);
		numVerts = verts.size() / 3;
		numIndices = indices.size();
	}

	void planeMeshQuads(float min, float max, float stepsize)
	{
		float x = min;
		float y = 0;

		for (float z = min; z <= max; z += stepsize)
		{
			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}

		for (float x = min + stepsize; x <= max; x += stepsize)
		{
			for (float z = min; z <= max; z += stepsize)
			{
				verts.push_back(x);
				verts.push_back(y);
				verts.push_back(z);
				normals.push_back(0);
				normals.push_back(1);
				normals.push_back(0);
			}
		}

		int nCols = (int)((max - min) / stepsize) + 1;
		int i = 0, j = 0;
		for (float x = min; x < max; x += stepsize)
		{
			j = 0;
			for (float z = min; z < max; z += stepsize)
			{
				indices.push_back(i * nCols + j);
				indices.push_back(i * nCols + j + 1);
				indices.push_back((i + 1) * nCols + j + 1);
				indices.push_back((i + 1) * nCols + j);
				++j;
			}
			++i;
		}
	}

	void init()
	{
		programID = LoadShaders("WaterShader.vertexshader", "WaterShader.tcs", "WaterShader.tes", "WaterShader.geoshader", "WaterShader.fragmentshader");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

		glBindVertexArray(0);
	}

	void useShader()
	{
		glUseProgram(programID);
		glBindVertexArray(VAO);
	}

	void draw()
	{
		glDrawElements(GL_PATCHES, numIndices, GL_UNSIGNED_INT, 0);
	}
};

#endif