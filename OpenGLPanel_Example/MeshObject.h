#pragma once

#include <string>
#include <vector>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "Texture.h"
//Include glew
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

//Include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef OpenMesh::TriMesh_ArrayKernelT<>  TriMesh;

class MyMesh : public TriMesh
{
public:
	MyMesh();
	~MyMesh();

	int FindVertex(MyMesh::Point pointToFind);
	void ClearMesh();
};

class GLMesh
{
public:
	GLMesh();
	~GLMesh();

	bool Init(std::string fileName);
	void Render();


	MyMesh mesh;
	GLuint vao;
	std::vector<GLuint> tVbos;
	GLuint ebo;
	GLuint vboVertices, vboNormal;

	bool LoadModel(std::string fileName);
	void LoadToShader();
};

class MeshObject
{
public:
	MeshObject();
	~MeshObject();

	bool Init(std::string fileName);
	void Render();
	void RenderSelectedFace();
	void RenderTexture(int id);
	bool AddSelectedFace(unsigned int faceID);
	void DeleteSelectedFace(unsigned int faceID);
	bool FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos);

	//hover method
	bool AddHoverSelectedFace(unsigned int faceID);
	void DeleteHoverSelectedFace(unsigned int faceID);
	void RenderHoverSelectedFace();

	GLMesh model;
	std::vector<Texture*> textures;
	std::vector<unsigned int> selectedFace;
	int ringNum = 0;
	
	//temp hover buffer
	std::vector<unsigned int> hoverSelectedFaces;

	//custom color
	glm::vec4 selectedFaceColor;
	glm::vec4 modelFaceColor;
	glm::vec4 hoverFaceColor;
	glm::vec4 wireFaceColor;
	glm::vec4 wireColor;
};

