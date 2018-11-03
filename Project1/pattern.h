#ifndef _PATTERN_H
#define _PATTERN_H
#include <vector>
#include <glm/vec3.hpp>

class AbsPattern
{
protected:
	unsigned VAO_ID = 0;
	unsigned VBO_ID = 0;
	unsigned EBO_ID = 0;
	float *vertices = nullptr;
	int verticesArraySize = 0;
	virtual void initializePattern() = 0;
public:
	AbsPattern() = default;
	virtual void loadPattern(float *vertices, int verticesArraySize) = 0;
	virtual void drawPattern() = 0;
	virtual ~AbsPattern() = default;
};

class TrianglePattern : public AbsPattern
{
private:
	void loadVertices(float *vertices, int verticesArraySize);
	virtual void initializePattern() override;
public:
	TrianglePattern() = default;
	TrianglePattern(const TrianglePattern &rectPattern) = delete;
	virtual void loadPattern(float* vertices, int verticesArraySize) override;
	virtual void drawPattern() override;
	void drawPattern(unsigned *indexArray, int indexArraySize); //using EBO
	~TrianglePattern() = default;
};


#endif