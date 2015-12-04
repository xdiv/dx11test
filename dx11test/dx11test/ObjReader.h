#pragma once

#ifndef _OBJREADER_H_
#define _OBJREADER_H_

#include "pch.h"
using namespace std;
using namespace DirectX;

typedef struct Vertices
{
	float x, y, z;//v
	float tu, tv;//vt
	float nx, ny, nz;//vn
}Vertices;


class ObjReader
{
private:
	typedef struct Point3D
	{
		int v, vt, vn;
	}Point3D;
public:
	ObjReader(void);
	~ObjReader(void);
	//bool GetTriangeleList(Vertices **, int &, bool remake = false);
	bool GetTriangeleList(mesh2d **, int &, bool remake = false);
	bool ReadFileStructure(WCHAR *);

private:
	int vertexCount, textureCount, normalCount, faceCount,
		total;
	vector<XMFLOAT3> * v;
	vector<XMFLOAT2> * vt;
	vector<XMFLOAT3> * vn;
	vector<vector<Point3D>> * f;

	char garbage;
	Vertices *m_vertices;
};
#endif

