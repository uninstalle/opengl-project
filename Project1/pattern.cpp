#include "pattern.h"

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>

void TrianglePattern::loadPattern(float* vertices, int verticesArraySize)
{
	try
	{
		loadVertices(vertices, verticesArraySize);
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
	}

	initializePattern();
}

void TrianglePattern::loadVertices(float* vertices, int verticesArraySize)
{
	if (vertices == nullptr)
		throw std::runtime_error("Vertices array null error.\n");
	this->vertices = vertices;
	this->verticesArraySize = verticesArraySize;
}

void TrianglePattern::initializePattern()
{
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, verticesArraySize, vertices, GL_STATIC_DRAW);

	//load position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	//load color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void TrianglePattern::drawPattern()
{
	glBindVertexArray(VAO_ID);
	glDrawArrays(GL_TRIANGLES, 0, verticesArraySize / sizeof(float) / 2);
}

void TrianglePattern::drawPattern(unsigned *indexArray, int indexArraySize)
{
	glBindVertexArray(VAO_ID);
	if (EBO_ID == 0)
		glGenBuffers(1, &EBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize, indexArray, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(int), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
}

void TrianglePatternTextured::initializePattern()
{
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, verticesArraySize, vertices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//tex coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
