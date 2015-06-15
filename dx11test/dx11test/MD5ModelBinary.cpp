#include "MD5ModelBinary.h"
#include "complex_types.h"
#include "PublicData.h"

MD5ModelBinary::MD5ModelBinary(const string &_file)
{
	file = _file;
	Joints = 0;
	Meshes = 0;
	meshStarLocations = 0;
}


MD5ModelBinary::~MD5ModelBinary()
{
	SAFE_DELETE(meshStarLocations);
	SafeDelete();
}

void MD5ModelBinary::SafeDelete()
{
	
	SAFE_DELETE(Joints);

	size_t i = 0;
	//for (size_t i = 0; i < numMeshes; i++)
	{
		SAFE_DELETE(Meshes[i].Triangles);
		SAFE_DELETE(Meshes[i].Vertices);
		SAFE_DELETE(Meshes[i].Weights);
	}

	SAFE_DELETE(Meshes);
}

void MD5ModelBinary::LoadModel(int modelPart)
{
	//ifstream myfile(file);

	const unsigned int length = 8192;
	char buffer[length];
	ifstream myfile;
	myfile.rdbuf()->pubsetbuf(buffer, length);
	//ofstream myfile(file, ios::out);
	myfile.open(file, ios::in | ios::binary);

	myfile.seekg(0, ios::end);
	__int64 fileLength = myfile.tellg();
	myfile.seekg(0, ios::beg);

	if (myfile.is_open())
	{
		/*if (meshStarLocations == NULL)
		{

		}*/
		numJoints = 0;
		numMeshes = 0;
		myfile.read((char *)&numJoints, sizeof(size_t));
		myfile.read((char *)&numMeshes, sizeof(size_t));

		assert(numMeshes != 0);
		assert(numJoints != 0);

		meshStarLocations = new size_t[numMeshes];
		Joints = new Joint[numJoints];

		for (size_t i = 0; i < numMeshes; i++)
		{
			myfile.read((char *)&meshStarLocations[i], sizeof(size_t));
		}

		for (size_t i = 0; i < numJoints; i++)
		{
			myfile.read((char *)&Joints[i], sizeof(Joint));
		}

		size_t j = 0;
		Meshes = new Mesh[1];

		myfile.seekg(meshStarLocations[modelPart]);
		
		myfile.read((char *)&Meshes[j].VerticesCount, sizeof(long));

		Meshes[j].Vertices = new Vertice[Meshes[j].VerticesCount];

		for (_Uint32t i = 0; i < Meshes[j].VerticesCount; i++)
		{
			myfile.read((char *)&Meshes[j].Vertices[i], sizeof(Vertice));
		}

		myfile.read((char *)&Meshes[j].TrianglesCount, sizeof(long));

		Meshes[j].Triangles = new int3[Meshes[j].TrianglesCount];
		for (_Uint32t i = 0; i < Meshes[j].TrianglesCount; i++)
		{
			myfile.read((char *)&Meshes[j].Triangles[i], sizeof(int3));
		}

		myfile.read((char *)&Meshes[j].WeightCount, sizeof(long));

		//__int64 poz = myfile.tellg();
		Meshes[j].Weights = new Weight[Meshes[j].WeightCount];
		for (_Uint32t i = 0; i < Meshes[j].WeightCount; i++)
		{
			myfile.read((char *)&Meshes[j].Weights[i], sizeof(Weight));
		}
	}
}

void MD5ModelBinary::WriteModelToFile(const string &file, size_t numMeshes, size_t numJoints, Mesh * Meshes, Joint * Joints)
{
	const unsigned int length = 2048;
	char buffer[length];
	ofstream myfile;
	myfile.rdbuf()->pubsetbuf(buffer, length);
	//ofstream myfile(file, ios::out);
	myfile.open(file, ios::out | ios::binary);

	if (myfile.is_open())
	{
		myfile.write((char *)&numJoints, sizeof(numJoints));
		myfile.write((char *)&numMeshes, sizeof(numJoints));

		//						numJoints + numMeshes			Joints						meshStarLocations
		size_t meshStarLocations = (sizeof(size_t) * 2) + (sizeof(Joint) * numJoints) + (sizeof(size_t) * numMeshes);
		myfile.write((char *)&meshStarLocations, sizeof(size_t));
		for (size_t i = 0; i < numMeshes - 1; i++)
		{
			meshStarLocations += sizeof(long) * 3 //TrianglesCount, VertCount, WeightCount
				+ sizeof(int3) * Meshes[i].TrianglesCount
				+ sizeof(Vertice) * Meshes[i].TrianglesCount
				+ sizeof(Weight) * Meshes[i].WeightCount;
			myfile.write((char *)&meshStarLocations, sizeof(size_t));
		}

		Joint jj;
		for (size_t i = 0; i < numJoints; i++)
		{
			jj = Joints[i];
			myfile.write((char *)&jj, sizeof(Joint));
		}

		for (size_t i = 0; i < numMeshes; i++)
		{
			myfile.write((char *)&Meshes[i].VerticesCount, sizeof(unsigned long));
			for (_Uint32t j = 0; j < Meshes[i].VerticesCount; j++)
			{
				myfile.write((char *)&Meshes[i].Vertices[j], sizeof(Vertice));
			}

			myfile.write((char *)&Meshes[i].TrianglesCount, sizeof(long));
			for (_Uint32t j = 0; j < Meshes[i].TrianglesCount; j++)
			{
				myfile.write((char *)&Meshes[i].Triangles[j], sizeof(int3));
			}

			myfile.write((char *)&Meshes[i].WeightCount, sizeof(long));
			for (_Uint32t j = 0; j < Meshes[i].WeightCount; j++)
			{
				myfile.write((char *)&Meshes[i].Weights[j], sizeof(Weight));
			}
		}
	}
}

size_t	MD5ModelBinary::GetMeshSize()
{
	return Meshes[0].VerticesCount;
}
size_t	MD5ModelBinary::GetJointsSize()
{
	return numJoints;
}

size_t	MD5ModelBinary::GetIntSize()
{
	return Meshes[0].TrianglesCount *3;
}

bool MD5ModelBinary::PrepareMesh(mesh2d *& mesh, int *& list, int nr)
{
	list = new int[Meshes[nr].TrianglesCount*3];
	mesh = new mesh2d[Meshes[nr].VerticesCount];

	bool * calc = new bool[Meshes[nr].VerticesCount];
	fill_n(calc, Meshes[nr].VerticesCount, false);
	for (_Uint32t i = 0; i < Meshes[nr].TrianglesCount; i++)
	{
		int in = i * 3;
		int a = Meshes[nr].Triangles[i].x;
		int b = Meshes[nr].Triangles[i].y;
		int c = Meshes[nr].Triangles[i].z;
		list[in] = c;
		list[in + 1] = b;
		list[in + 2] = a;

		if (!calc[a])
			CopyMeshInfo(mesh, a, nr);
		if (!calc[b])
			CopyMeshInfo(mesh, b, nr);
		if (!calc[c])
			CopyMeshInfo(mesh, c, nr);

		float3 normal = CalcNormals(mesh[a].Pos, mesh[b].Pos, mesh[c].Pos);

		/*if (!calc[a])
		{
			mesh[a].Normal = normal;
			calc[a] = true;
		}
		if (!calc[b])
		{
			mesh[b].Normal = normal;
			calc[b] = true;
		}
		if (!calc[c])
		{
			mesh[c].Normal = normal;
			calc[c] = true;
		}*/
	}
	delete calc;
	calc = 0;
	return true;
}

void MD5ModelBinary::CopyMeshInfo(mesh2d *mesh, int indice, int nr)
{
	mesh[indice].Tex0 = Meshes[nr].Vertices[indice].Tex;
	//mesh[indice].StartWeight = Meshes[nr].Vertices[indice].StartWeight;
	//mesh[indice].WeightCount = Meshes[nr].Vertices[indice].WeightCount;
	mesh[indice].Pos = float3(0, 0, 0);
	for (_Uint32t i = 0; i < Meshes[nr].Vertices[indice].WeightCount; i++)
	{
		int weigthId = Meshes[nr].Vertices[indice].StartWeight + i;
		int jointId = Meshes[nr].Weights[weigthId].JointId;
		float bias = Meshes[nr].Weights[weigthId].Bias;

		float3 root_pos = Multiply(Joints[jointId].Orient, Meshes[nr].Weights[weigthId].Pos);
		mesh[indice].Pos += bias * (root_pos + Joints[jointId].Pos);
	}
}