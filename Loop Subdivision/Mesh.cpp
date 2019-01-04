#include "Mesh.h"

// Created by Joshua Cook
// For Graphics and Computational Programming


// definitions of PI
#define PI 3.14159265359f
#define TWOPI 6.28318530718f

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadMesh(std::string filename)
{
	// basic OBJ loader
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<FaceData> faces;

	std::ifstream fileStream(filename);

	std::string line;
	while (std::getline(fileStream, line))
	{
		std::stringstream iss(line);
		std::string type;

		iss >> type;
		if (type == "v")
		{
			glm::vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (type == "vn")
		{
			glm::vec3 normal;
			iss >> normal.x >> normal.y >> normal.z;

			temp_normals.push_back(normal);
		}
		else if (type == "vt")
		{
			glm::vec2 uv;
			iss >> uv.x >> uv.y;

			temp_uvs.push_back(uv);
		}
		else if (type == "f")
		{
			FaceData newFace;
			char dummy;

			for (int i = 0; i < 3; i++)
			{
				iss >> newFace.vI[i] >> dummy >> newFace.uI[i] >> dummy >> newFace.nI[i];
			}

			faces.push_back(newFace);
		}
	}
	fileStream.close();

	int numFaces = faces.size();
	for (int i = 0; i < numFaces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices.push_back(temp_vertices[faces[i].vI[j] - 1]);
		}
	}

	numVertices = vertices.size();

	ComputeVAO();
}

// returns the magnitude of a vector
float Magnitude(glm::vec3 vector)
{
	return glm::sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

// sets up the winged edge table that makes the subdivision process much easier
void Mesh::ComputeWingedEdgeTable()
{
	// make sure our vectors are empty
	edges.clear();
	faces.clear();
	edgePairs.clear();
	wingedEdgeTable.clear();

	// we need to fill the vectors, so we loop through 3 edges at a time, which because of our OBJ loader, will make a single face
	for (unsigned int i = 0; i < numVertices; i += 3)
	{
		// we make a new edge
		Edge *e1;
		// assign two vertices to the edge
		e1 = new Edge(&vertices[i], &vertices[i + 1]);
		// add the edge to the edge vector
		edges.push_back(*e1);

		// repeat
		Edge *e2;
		e2 = new Edge(&vertices[i + 1], &vertices[i + 2]);
		edges.push_back(*e2);

		Edge *e3;
		e3 = new Edge(&vertices[i + 2], &vertices[i]);
		edges.push_back(*e3);
	}

	//setting up all the faces.
	int numEdges = edges.size();
	for (unsigned int i = 0; i < numEdges; i += 3)
	{
		// create the current face
		Face newFace;
		// add the edges to the current face
		newFace.edge[0] = &edges[i];
		newFace.edge[1] = &edges[i + 1];
		newFace.edge[2] = &edges[i + 2];
		glm::vec3 U, V;
		U = *edges[i].point[1] - *edges[i].point[0];
		V = *edges[i + 2].point[0] - *edges[i].point[1];
		newFace.normal = glm::vec3(((U.y * V.z) - (U.z * V.y)),
			((U.z * V.x) - (U.x * V.z)),
			((U.x * V.y) - (U.y * V.x)));
		newFace.normalMag = Magnitude(newFace.normal);
		// then add the face to the face vector
		faces.push_back(newFace);
	}
	//setting up all the edge pairs.
	numEdges = edges.size();
	int numPairs = edgePairs.size();
	for (unsigned int i = 0; i < numEdges; i++)
	{
		bool pairFound = false;
		EdgePair temp[2];
		temp[0].AB = &edges[i];
		temp[1].BA = &edges[i];
		for (int j = 0; j < numEdges; j++)
		{
			if (*temp[0].AB->point[0] == *edges[j].point[1] && *temp[0].AB->point[1] == *edges[j].point[0])
			{
				temp[0].BA = &edges[j];
				temp[1].AB = &edges[j];
				break;
			}
		}
		numPairs = edgePairs.size();
		for (int j = 0; j < numPairs; j++)
		{
			if ((*temp[0].AB == *edgePairs[j].AB && *temp[0].BA == *edgePairs[j].BA) || (*temp[0].BA == *edgePairs[j].AB && *temp[0].AB == *edgePairs[j].BA))
				pairFound = true;
		}
		if (!pairFound)
		{
			edgePairs.push_back(temp[0]);
			//edgePairs.push_back(temp[1]);
		}
	}

	// create the edge table
	numPairs = edgePairs.size();
	int numFaces = faces.size();
	for (unsigned int i = 0; i < numPairs; i++)
	{
		bool rightFound = false, leftFound = false;
		WingedEdge newEdge;

		newEdge.edgePair = &edgePairs[i];
		// to connect the edge table, we need to search through and link all the faces correctly
		for (int j = 0; j < numFaces; j++)
		{
			for (unsigned int l = 0; l < 3; l++)
			{
				// and check it with the 'right' face
				if (!rightFound)
				{
					if (newEdge.edgePair->AB == faces[j].edge[l])
					{
						newEdge.rightFace = &faces[j];

						(l == 0 ? newEdge.rightPre = faces[j].edge[2] : newEdge.rightPre = faces[j].edge[l - 1]);

						(l == 2 ? newEdge.rightSuc = faces[j].edge[0] : newEdge.rightSuc = faces[j].edge[l + 1]);

						rightFound = true;
					}
				}

				// and the 'left' face
				if (!leftFound)
				{
					if (newEdge.edgePair->BA == faces[j].edge[l])
					{
						newEdge.leftFace = &faces[j];

						(l == 0 ? newEdge.leftPre = faces[j].edge[2] : newEdge.leftPre = faces[j].edge[l - 1]);

						(l == 2 ? newEdge.leftSuc = faces[j].edge[0] : newEdge.leftSuc = faces[j].edge[l + 1]);

						leftFound = true;
					}
				}
			}
		}
		// add to the edge table
		wingedEdgeTable.push_back(newEdge);
	}
}

glm::vec3 NewVertPosition(WingedEdge edge)
{
	glm::vec3 point;

	// defines the hard edge
	float boundary = 90.f;

	// get dot product
	float dot_Product = glm::dot(edge.leftFace->normal, edge.rightFace->normal);
	// work out the angle between two edges
	float angle = ((acos(dot_Product / (edge.leftFace->normalMag * edge.rightFace->normalMag))) * 180.f) / PI;

	//if angle is less than the angle boundary then it is not a hard edge
	if (angle <= boundary)
	{
		point = ((*edge.edgePair->AB->point[0] * 0.375f) +
				 (*edge.edgePair->AB->point[1] * 0.375f) +
				 (*edge.rightSuc->point[1] * 0.125f) +
				 (*edge.leftPre->point[0] * 0.125f));
	}
	else
	{
		point = (*edge.edgePair->AB->point[0] * 0.5f) +
				(*edge.edgePair->AB->point[1] * 0.5f);
	}
	return point;
}

void Mesh::Subdivide()
{
	int index = 0;
	int indexStep = 0;

	ComputeWingedEdgeTable();
	// two steps of subdivision

	newVertices.clear();

	// adding in the new vertices
	glm::vec3 newVert[3];
	int numFaces = faces.size();
	int numWings = wingedEdgeTable.size();
	for (unsigned int i = 0; i < numFaces; i++)
	{
		for (unsigned int j = 0; j < numWings; j++)
		for (unsigned int l = 0; l < 3; l++)
		if (faces[i].edge[l] == wingedEdgeTable[j].edgePair->AB || faces[i].edge[l] == wingedEdgeTable[j].edgePair->BA)
			newVert[l] = NewVertPosition(wingedEdgeTable[j]);

		//New Face 1
		newVertices.push_back(*faces[i].edge[0]->point[0]);
		newVertices.push_back(newVert[0]);
		newVertices.push_back(newVert[2]);
		//New Face 2
		newVertices.push_back(newVert[0]);
		newVertices.push_back(*faces[i].edge[1]->point[0]);
		newVertices.push_back(newVert[1]);
		//New Face 3
		newVertices.push_back(newVert[0]);
		newVertices.push_back(newVert[1]);
		newVertices.push_back(newVert[2]);
		//New Face 4
		newVertices.push_back(newVert[2]);
		newVertices.push_back(newVert[1]);
		newVertices.push_back(*faces[i].edge[2]->point[0]);
	}

	// and moving the old vertices
	numVertices = vertices.size();
	int numEdgePairs = edgePairs.size();
	int numNewVerts = newVertices.size();
	for (unsigned int i = 0; i < numVertices; i++)
	{
		glm::vec3 vertexSum = glm::vec3(0.f, 0.f, 0.f);
		float N = 0.f;
		for (unsigned int j = 0; j < numEdgePairs; j++)
		{
			if (vertices[i] == *edgePairs[j].AB->point[0])
			{
				N += 1.f;
				vertexSum += *edgePairs[j].AB->point[1];
			}
			if (vertices[i] == *edgePairs[j].AB->point[1])
			{
				N += 1.f;
				vertexSum += *edgePairs[j].AB->point[0];
			}
		}

		float beta = (1.f / N) * ((5.f / 8.f) - (((3.f + (2.f * cos(2.f * PI / N))) * (3.f + (2.f * (cos(2.f * PI / N))))) / 64.f));
		glm::vec3 newVertPos = ((1.f - (N * beta)) * vertices[i]) + (beta * vertexSum);
		for (unsigned int j = 0; j < numNewVerts; j++)
			if (newVertices[j].x == vertices[i].x)
				if (newVertices[j] == vertices[i])
						newVertices[j] = newVertPos;
	}

	vertices.clear();
	vertices = newVertices;
	numVertices = vertices.size();

	ComputeVAO();
}

void Mesh::ComputeVAO()
{
	VAO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numVertices, &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}

void Mesh::Draw(Shader* shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Activate the VAO
	glBindVertexArray(VAO);

	// Draw vertices
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	//glDrawArrays(GL_POINTS, 0, numVertices);

	// Unbind VAO
	glBindVertexArray(0);
}