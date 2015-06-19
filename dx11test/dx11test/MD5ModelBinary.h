#pragma once

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;

#include "../cdxml/complex_types.h"
#include "../cdxml/math.h"

class MD5ModelBinary
{
protected:
	size_t *	meshStarLocations;
	size_t		meshSize;
	size_t		numJoints;
	size_t		numMeshes;
	Joint *		Joints;
	Mesh *		Meshes;
	string		file;

public:
	MD5ModelBinary(const string &file);
	~MD5ModelBinary();

	bool	PrepareMesh(mesh2d *& mesh, int *& list, int);
	void	SafeDelete();
	void	LoadModel(int modelPart);

	size_t	GetMeshSize();
	size_t	GetJointsSize();
	size_t	GetIntSize();

	static void	WriteModelToFile(const string &file, size_t numMeshes, size_t numJoints, Mesh * Meshes, Joint * Joints);

private:
	void CopyMeshInfo(mesh2d * mesh, int indice, int nr);
};

