#include "MD5Model.h"
#include "DXMath.h"

MD5Model::MD5Model()
{
	Joints = 0;
	Meshes = 0;
}

MD5Model::~MD5Model()
{
	SafeDelete();
}

void MD5Model::SafeDelete()
{
	if (Joints != NULL)
		SAFE_DELETE(Joints);
	if (Meshes == NULL)
		return;
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		SAFE_DELETE(Meshes[i].Triangles);
		SAFE_DELETE(Meshes[i].Vertices);
		SAFE_DELETE(Meshes[i].Weights);
	}

	SAFE_DELETE(Meshes);
}


bool MD5Model::LoadModel(const std::string& filename)
{
	ifstream file;
	file.open(filename);

	short meshid = 0;

	string param;
	string junk;   // Read junk from the file

	//fs::ifstream file(filename);
	file.seekg(0, ios::end);
	__int64 fileLength = file.tellg();
	file.seekg(0, ios::beg);
	assert(fileLength > 0);

	SafeDelete();

	file >> param;

	while (!file.eof())
	{
		if (param == "MD5Version")
		{
			file >> iMD5Version;
			assert(iMD5Version == 10);
		}
		else if (param == "commandline")
		{
			file.ignore(1, '\n');  // Ignore the contents of the line
		}
		else if (param == "numJoints")
		{
			file >> iNumJoints;
			Joints = new Joint[iNumJoints];
		}
		else if (param == "numMeshes")
		{
			file >> iNumMeshes;

			Meshes = new Mesh[iNumMeshes];
		}
		else if (param == "joints")
		{
		
			string name;
			Joint joint;

			file >> junk; // Read the '{' character
			for (size_t i = 0; i < iNumJoints; ++i)
			{
				file >> name >> Joints[i].Parent >> junk;
				file >> Joints[i].Pos.x >> Joints[i].Pos.y >> Joints[i].Pos.z >> junk >> junk
					>> Joints[i].Orient.x >> Joints[i].Orient.y >> Joints[i].Orient.z >> junk;

				ComputeQuatW(Joints[i].Orient);
				file.ignore(20, '\n');
				name.clear();
			}
			file >> junk; // Read the '}' character
		}
		else if (param == "mesh")
		{
			file >> junk; // Read the '{' character
			file >> param;
			while (param != "}")  // Read until we get to the '}' character
			{
				//mesh pavadininimas
				//if (param == "shader")
				//{
				//	file >> mesh.Shader;
				//	RemoveQuotes(mesh.Shader);
				//	fs::path shaderPath(mesh.m_Shader);
				//	fs::path texturePath;
				//	if (shaderPath.has_parent_path())
				//	{
				//		texturePath = shaderPath;
				//	}
				//	else
				//	{
				//		texturePath = parent_path / shaderPath;
				//	}
				//	if (!texturePath.has_extension())
				//	{
				//		texturePath.replace_extension(".tga");
				//	}
				//	mesh.TexID = SOIL_load_OGL_texture(texturePath.string().c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				//	file.ignore(fileLength, '\n'); // Ignore everything else on the line
				//}else
				if (param == "numverts")
				{
					file >> Meshes[meshid].VerticesCount;               // Read in the vertices

					Meshes[meshid].Vertices = new Vertice[Meshes[meshid].VerticesCount];

					file.ignore(1, '\n');
					for (_Uint32t i = 0; i < Meshes[meshid].VerticesCount; ++i)
					{
						file >> junk >> junk >> junk                    // vert vertIndex (
							>> Meshes[meshid].Vertices[i].Tex.x >> Meshes[meshid].Vertices[i].Tex.y >> junk  //  s t )
							>> Meshes[meshid].Vertices[i].StartWeight >> Meshes[meshid].Vertices[i].WeightCount;

						file.ignore(1, '\n');
					}
				}
				else if (param == "numtris")
				{
					file >> Meshes[meshid].TrianglesCount;
					//file.ignore(1, '\n');
					Meshes[meshid].Triangles = new int3[Meshes[meshid].TrianglesCount];
					for (_Uint32t i = 0; i < Meshes[meshid].TrianglesCount; ++i)
					{
						file >> junk >> junk >> Meshes[meshid].Triangles[i].x >> Meshes[meshid].Triangles[i].y >> Meshes[meshid].Triangles[i].z;

						file.ignore(1, '\n');
					}
				}
				else if (param == "numweights")
				{
					file >> Meshes[meshid].WeightCount;
					Meshes[meshid].Weights = new Weight[Meshes[meshid].WeightCount];
					for (_Uint32t i = 0; i < Meshes[meshid].WeightCount; ++i)
					{
						file >> junk >> junk
							>> Meshes[meshid].Weights[i].JointId
							>> Meshes[meshid].Weights[i].Bias >> junk
							>> Meshes[meshid].Weights[i].Pos.x
							>> Meshes[meshid].Weights[i].Pos.y
							>> Meshes[meshid].Weights[i].Pos.z
							>> junk;

						file.ignore(1, '\n');
					}
				}
				else
				{
					file.ignore(1, '\n');
				}

				file >> param;
			}
			meshid++;
		}

		file >> param;
	}

	file.close();
	return true;
}

size_t MD5Model::GetMeshSize(size_t nr)
{
	return Meshes[nr].VerticesCount;
}

size_t MD5Model::GetIntSize(size_t nr)
{
	return  Meshes[nr].TrianglesCount * 3;
}

size_t  MD5Model::GetMeshSize()
{
	return iNumMeshes;
}

size_t MD5Model::GetJointsSize()
{
	return iNumJoints;
}

Mesh*  MD5Model::GetMesh()
{
	return Meshes;
}
Joint*  MD5Model::GetBones()
{
	return Joints;
}

bool MD5Model::PrepareMesh(meshv1 *& mesh, int *& list, int nr)
{
	list = new int[GetIntSize(nr)];
	mesh = new meshv1[GetMeshSize(nr)];

	bool * calc = new bool[Meshes[nr].VerticesCount];
	fill_n(calc, Meshes[nr].VerticesCount, false);
	for (_Uint32t i = 0; i < Meshes[nr].TrianglesCount; i++)
	{
		int in = i * 3;
		int a = Meshes[nr].Triangles[i].x;
		int b = Meshes[nr].Triangles[i].y;
		int c = Meshes[nr].Triangles[i].z;
		list[in] = c;
		list[in+1] = b;
		list[in+2] = a;

		if (!calc[a])
			CopyMeshInfo(mesh, a, nr);
		if (!calc[b])
			CopyMeshInfo(mesh, b, nr);
		if (!calc[c])
			CopyMeshInfo(mesh, c, nr);

		float3 normal = CalcNormals(mesh[a].Pos, mesh[b].Pos, mesh[c].Pos);

		if (!calc[a])
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
		}
	}
	delete calc;
	calc = 0;
	return true;
}

void MD5Model::CopyMeshInfo(meshv1 *mesh, int indice, int nr)
{
	mesh[indice].Tex0 = Meshes[nr].Vertices[indice].Tex;
	mesh[indice].StartWeight = Meshes[nr].Vertices[indice].StartWeight;
	mesh[indice].WeightCount = Meshes[nr].Vertices[indice].WeightCount;
	mesh[indice].Pos = float3(0, 0, 0);
	for (_Uint32t i = 0; i < Meshes[nr].Vertices[indice].WeightCount; i++)
	{
		int weigthId = Meshes[nr].Vertices[indice].StartWeight + i;
		int jointId = Meshes[nr].Weights[weigthId].JointId;
		float bias = Meshes[nr].Weights[weigthId].Bias;
		//D3DXVECTOR3 root_pos = Multiply(Joints[jointId].Orient, Joints[jointId].Pos);
		//mesh[indice].Pos +=  (root_pos + Weights[weigthId].Pos);

		float3 root_pos = Multiply(Joints[jointId].Orient, Meshes[nr].Weights[weigthId].Pos);
		mesh[indice].Pos += bias * (root_pos + Joints[jointId].Pos);
	}
}