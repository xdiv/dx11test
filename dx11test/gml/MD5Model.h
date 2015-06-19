#include "..\cdxml\complex_types.h"
#include "..\cdxml\math.h"

using namespace std;

class GML_API MD5Model
{
public:
	MD5Model();
	~MD5Model();

	void SafeDelete();

	bool	LoadModel(const string& filename);
	bool	PrepareMesh(meshv1 *& mesh, UINT *& list, UINT);

	UINT	GetMeshSize(UINT);
	UINT	GetIntSize(UINT);
	UINT	GetMeshSize();
	UINT	GetJointsSize();

	Mesh*	GetMesh();
	Joint*	GetBones();
private:

	UINT  iMD5Version;
	UINT  iNumJoints;
	UINT  iNumMeshes;

	bool			bHasAnimation;

	Joint*			Joints;
	Mesh*			Meshes;

	void CopyMeshInfo(meshv1 *mesh, UINT indice, UINT nr);
};