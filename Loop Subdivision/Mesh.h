#ifndef _MESH_H_
#define _MESH_H_

// Created by Joshua Cook
// For Graphics and Computational Programming

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>
#include "glew.h"

#include "Shader.h"

// For subdivision
// structure to hold edge data and comparison methods
struct Edge
{
	Edge(){};
	Edge(glm::vec3 *p1, glm::vec3 *p2) { point[0] = p1, point[1] = p2; };

	glm::vec3* point[2];
	const bool operator==(const Edge& other) { return (*point[0] == *other.point[0] && *point[1] == *other.point[1]); };
	const bool operator!=(const Edge& other) { return!(*this == other); };
};
// structure to hold edge pair data
// as edges are read in two ways (A->B, B->A), and storing both edges reduced computation time
struct EdgePair
{
	Edge* AB;
	Edge* BA;
};
// structure to hold face data
struct Face
{
	Edge* edge[3];

	glm::vec3 normal;
	float normalMag;
};
// structure to hold edge table data
struct WingedEdge
{
	EdgePair* edgePair;
	Face* leftFace;
	Face* rightFace;
	Edge* leftPre;
	Edge* leftSuc;
	Edge* rightPre;
	Edge* rightSuc;
};

// For model loader
struct FaceData
{
	int vI[3];
	int uI[3];
	int nI[3];
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadMesh(std::string filename);
	void Draw(Shader* shader);

	// call this function to initialise the subdivision
	void Subdivide();

	bool Null() { return (numVertices > 0 ? false : true); };

private:
	GLuint VAO;
	GLuint vertexBuffer;
	unsigned int numVertices;

	glm::mat4 modelMatrix;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;

	// face/edge data to perform the subdivision
	std::vector<Face> faces;
	std::vector<Edge> edges;
	std::vector<EdgePair> edgePairs;
	std::vector<WingedEdge> wingedEdgeTable;

	std::vector<glm::vec3> newVertices;

	void ComputeVAO();
	void ComputeWingedEdgeTable();
};

#endif