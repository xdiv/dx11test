#include "..\cdxml\complex_types.h"
#include "MD5Animation.h"
#include "PublicData.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class MD5Model
{
public:
	MD5Model();
	~MD5Model();

	void SafeDelete();

	bool	LoadModel(const string& filename);
	bool	PrepareMesh(meshv1 *& mesh, int *& list, int);

	size_t	GetMeshSize(size_t);
	size_t	GetIntSize(size_t);
	size_t	GetMeshSize();
	size_t	GetJointsSize();

	Mesh*	GetMesh();
	Joint*	GetBones();
private:

	size_t 	iMD5Version;
	size_t  iNumJoints;
	size_t  iNumMeshes;

	bool			bHasAnimation;

	Joint*			Joints;
	Mesh*			Meshes;

	void CopyMeshInfo(meshv1 *mesh, int indice, int nr);
};
