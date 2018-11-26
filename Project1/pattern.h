#ifndef _PATTERN_H
#define _PATTERN_H
#include <vector>
#include "shader.h"
#include "mesh.h"

class AbsPattern
{
protected:
	unsigned VAO_ID = 0;
	unsigned VBO_ID = 0;
	unsigned EBO_ID = 0;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
public:
	AbsPattern() = default;
	virtual void loadPattern() = 0;
	virtual void drawPattern() = 0;
	virtual ~AbsPattern() = default;
	static float *genVerticesArray(AbsPattern &pattern);
	static unsigned *genIndicesArray(AbsPattern &pattern);
};

class TrianglePattern : public AbsPattern
{
protected:
	virtual void setVerticesArrayAttrib() const;
	virtual void drawArrays() const;
	virtual  void drawElements() const;
	void genVAO();
	void genVBO();
	void genEBO();
public:
	TrianglePattern() = default;
	TrianglePattern(std::vector<float> vertices, std::vector<unsigned> indices)
	{
		this->vertices = std::move(vertices);
		this->indices = std::move(indices);
	}
	TrianglePattern(const TrianglePattern &rectPattern) = delete;
	void loadPattern() override;
	void drawPattern() override;
	void loadVertices(std::vector<float> vertices) { this->vertices = std::move(vertices); }
	void loadIndices(std::vector<unsigned> indices) { this->indices = std::move(indices); }
	~TrianglePattern() = default;
};

class SpherePattern : public TrianglePattern
{
protected:
	float radius;
	virtual void genVerticesFromSphere(int deltaAlphaDivision, int deltaBetaDivision);
	void setVerticesArrayAttrib() const override;
public:
	SpherePattern() = default;
	SpherePattern(float radius) :radius(radius) {}
	void loadPattern() override;
	void loadPattern(int alphaDivision, int betaDivision);
	void drawPattern() override;
};

class SpherePatternTextured : public SpherePattern
{
private:
	unsigned Texture_ID;
	void genVerticesFromSphere(int deltaAlphaDivision, int deltaBetaDivision) override;
	void setVerticesArrayAttrib() const override;
	void drawPattern() override {}
public:
	SpherePatternTextured() = default;
	SpherePatternTextured(float radius) :SpherePattern(radius) {}
	void loadTexture(const char* path);
	void drawPattern(ShaderProgram &shader);
};


Mesh genSphereMesh(int deltaAlphaDivision, int deltaBetaDivision, const char* texturePath=nullptr);

#endif