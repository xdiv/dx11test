#include "../cdxml/complex_types.h"
#include "../cdxml/math.h"
#include "stdafx.h"

class GML_API MD5ModelBinary
{
protected:
	UINT *	meshStarLocations;
	UINT		meshSize;
	UINT		numJoints;
	UINT		numMeshes;
	Joint *		Joints;
	Mesh *		Meshes;
	char *		file;

public:
	MD5ModelBinary(char * file);
	~MD5ModelBinary();

	bool	PrepareMesh(mesh2d *& mesh, UINT *& list, UINT);
	void	SafeDelete();
	void	LoadModel(int modelPart);

	UINT	GetMeshSize();
	UINT	GetJointsSize();
	UINT	GetIntSize();

	static void	WriteModelToFile(const string &file, UINT numMeshes, UINT numJoints, Mesh * Meshes, Joint * Joints);

private:
	void CopyMeshInfo(mesh2d * mesh, UINT indice, UINT nr);
};