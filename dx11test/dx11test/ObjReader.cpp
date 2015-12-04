#include "pch.h"
#include "ObjReader.h"

using namespace std;
ObjReader::ObjReader(void)
{
	m_vertices = 0;
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;
	v = 0;
	vn = 0;
	vt = 0;
	f = 0;
}


ObjReader::~ObjReader(void)
{
	delete[] m_vertices;
	m_vertices = 0;
}

bool ObjReader::ReadFileStructure(WCHAR * filename)
{
	char input, prev = 0;
	char *line;
	int location;
	ifstream file;

	// Initialize the counts.

	float a, b, c;
	Point3D poin;

	file.open(filename);

	if (file.fail())
	{
		file.close();
		return false;
	}

	v = new vector<XMFLOAT3>();
	vt = new vector<XMFLOAT2>();
	vn = new vector<XMFLOAT3>();
	f = new vector<vector<Point3D>>();


	// Read from the file and continue to read until the end of the file is reached.
	file.get(input);
	while (!file.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (prev == 'v' && input == 't') //
		{
			textureCount++;
			file >> a >> b;
			vt->push_back(XMFLOAT2(a, b));
		}
		else if (prev == 'v' && input == ' ') //
		{
			vertexCount++;
			file >> a >> b >> c;
			v->push_back(XMFLOAT3(a, b, c));
		}
		else if (prev == 'v' && input == 'n')
		{
			normalCount++;
			file >> a >> b >> c;
			vn->push_back(XMFLOAT3(a, b, c));
		}
		else if (prev == 'f' && input == ' ')
		{
			faceCount++;
			vector<Point3D> list;
			while (input != '\n')
			{
				file >> poin.v;
				file >> input;
				file >> poin.vt;
				file >> input;
				file >> poin.vn;

				file.get(input);
				list.push_back(poin);
			}
			f->push_back(list);
		}
		else if (input == '#')
		{
			while (input != '\n')
			{
				file.get(input);
			}
		}

		// Start reading the beginning of the next line.
		prev = input;
		file.get(input);
	}

	// Close the file.
	file.close();

	return true;
}

//ið turimø duomenø sukuria TrinagleList masyvà, ir gràþina pinteri á já.
//Jei masyvas jau sukurtas, tiesiog gràþina masyvà.
//jei "remake" true, masyvà perkruria
bool ObjReader::GetTriangeleList(mesh2d ** list, int & count, bool remake)
{
	mesh2d *o_list = nullptr;
	if (m_vertices == 0 || remake)
	{
		total = 0;
		vector<Vertices> invList;

		for (int i = 0; i< faceCount; i++)
		{
			if ((f->at(i)).size() > 3)
			{
				return false;
			}

			for (int j = (f->at(i)).size() - 1, size = 0; j >= size; j--)
			{
				Vertices vertice;

				total++;

				vertice.nx = vn->at((f->at(i)).at(j).vn - 1).x;
				vertice.ny = vn->at((f->at(i)).at(j).vn - 1).y;
				vertice.nz = vn->at((f->at(i)).at(j).vn - 1).z;

				vertice.x = v->at((f->at(i)).at(j).v - 1).x;
				vertice.y = v->at((f->at(i)).at(j).v - 1).y;
				vertice.z = v->at((f->at(i)).at(j).v - 1).z;

				vertice.tu = vt->at((f->at(i)).at(j).vt - 1).x;
				vertice.tv = vt->at((f->at(i)).at(j).vt - 1).y;
				invList.push_back(vertice);

				//jei ka majai :)
				//if(goNeg >= 3 && j+1 < size)
				//{
				//	j = j-2;
				//	goNeg = 0;
				//}

			}
		}

		if (!m_vertices)
		{
			delete[] m_vertices;
			m_vertices = 0;
		}

		o_list = new mesh2d[total];

		for (int i = 0; i < total; i++)
			o_list[i] = mesh2d(invList[i].x, invList[i].y, invList[i].z, invList[i].tu, invList[i].tv);
	}
	if(&count != nullptr)
		count = total;
	*list = o_list;
	return true;
}