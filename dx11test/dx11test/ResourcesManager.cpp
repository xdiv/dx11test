#include "pch.h"
#include "ResourcesManager.h"
#include "ShaderBase.h"
#include "../gml/MD5ModelBinary.h"

ResourcesManager::ResourcesManager(): gameModels(nullptr), textures(nullptr)
{
}


ResourcesManager::~ResourcesManager()
{
}

void ResourcesManager::Init(ID3D11Device2 * dev)
{
	if (gameModels != 0)
		return;

	gameModels = new vector<ResourceContainer<GameModel>>();
	textures = new vector<ResourceContainer<ID3D11ShaderResourceView>>();
	setBasicData(dev);
}

GameModel* ResourcesManager::GetGameModelById(uint32_t id)
{
	ResourceContainer<GameModel> container = gameModels->at(id);
	if (container.state == ResourceState::Loading)
	{
		// TODO kanors protingiau
		// Daba žukraunam standartini modeli
		container = gameModels->at(0);
	}

	if (container.resource)
	{
		// TODO užkrauti modeli
	}

	return container.resource;
}

ID3D11ShaderResourceView* ResourcesManager::GetTextureById(uint32_t id)
{
	ResourceContainer<ID3D11ShaderResourceView> container = textures->at(id);
	if (container.state == ResourceState::Loading)
	{
		// TODO kanors protingiau
		// Daba žukraunam standartini modeli
		container = textures->at(0);
	}

	if (container.resource)
	{
		// TODO užkrauti modeli
	}

	return container.resource;
}

//ResourcesManager * ResourcesManager::GetInstance()
//{
//	if (singleton)
//		singleton = new ResourcesManager();
//	return singleton;
//}

void ResourcesManager::setBasicData(ID3D11Device2 * dev)
{
	auto x = ResourceContainer<GameModel>();
	x.resource = new GameModel();

	auto y = ResourceContainer<ID3D11ShaderResourceView>();
	//y.resource = new ID3D11ShaderResourceView();
	{ //plane
		mesh2d mesh[] =
		{
			mesh2d(-0.5, -0.5, 0, 0, 0),
			mesh2d(-0.5,  0.5, 0, 0, 1),
			mesh2d(0.5,  0.5, 0, 1, 1),
			mesh2d(0.5, -0.5, 0, 1, 0)
		};

		UINT index[] = { 0, 1, 2, 2, 3, 0 };
		//UINT index[] = { 0, 3, 1, 3, 2, 1 };

		//UINT index[] = { 0, 1, 3, 2, 1, 3 };
		x.resource->vert_count = sizeof(mesh) / sizeof(mesh2d);
		x.resource->indexCount = sizeof(index) / sizeof(UINT);

		//x.instances = new InstanceType_B[data->maxInstanceCount];

		x.resource->stride[0] = sizeof(mesh2d);
		//x.resource->stride[1] = sizeof(InstanceType_B);

		x.resource->pVBuffer = CreateVertexBufferHelp(dev, (sizeof(mesh2d) * x.resource->vert_count), mesh);
		x.resource->pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * x.resource->indexCount, index);
	}
	
	gameModels->push_back(x); //0 plane

	{ //cube
		mesh2d mesh[] =
		{
			mesh2d( 0.5, -0.5, -0.5,  0,  0),
			mesh2d( 0.5, -0.5,  0.5,  0, -1),
			mesh2d(-0.5, -0.5,  0.5, -1, -1),

			mesh2d(-0.5, 0.5, -0.5, -1, 1),
			mesh2d(-0.5, 0.5,  0.5, -1, 0),
			mesh2d(-0.5, 0.5,  0.5,  0, 0),

			mesh2d(0.5,  0.5, -0.5,  0, 1),
			mesh2d(0.5,  0.5,  0.5, -1, 1),
			mesh2d(0.5, -0.5,  0.5, -1, 0),

			mesh2d( 0.5,  0.5, 0.5,  0, 1),
			mesh2d(-0.5,  0.5, 0.5, -1, 1),
			mesh2d(-0.5, -0.5, 0.5, -1, 0),

			mesh2d(-0.5,  0.5,  0.5, 0, 1),
			mesh2d(-0.5,  0.5, -0.5, 1, 1),
			mesh2d(-0.5, -0.5, -0.5, 1, 0),

			mesh2d( 0.5, -0.5, -0.5,  0, -1),
			mesh2d(-0.5, -0.5, -0.5, -1, -1),
			mesh2d(-0.5,  0.5, -0.5, -1,  0),

			mesh2d(-0.5, -0.5, -0.5, -1, 0),
			mesh2d(0.5, -0.5, -0.5, 0, 0),
			mesh2d(-0.5, -0.5, 0.5, -1, -1),

			mesh2d( 0.5, 0.5, -0.5,  0, 1),
			mesh2d(-0.5, 0.5, -0.5, -1, 1),
			mesh2d( 0.5, 0.5,  0.5,  0, 0),

			mesh2d(0.5, -0.5, -0.5,  0, 0),
			mesh2d(0.5,  0.5, -0.5,  0, 1),
			mesh2d(0.5, -0.5,  0.5, -1, 0),

			mesh2d( 0.5, -0.5, 0.5,  0, 0),
			mesh2d( 0.5,  0.5, 0.5,  0, 1),
			mesh2d(-0.5, -0.5, 0.5, -1, 0),

			mesh2d(-0.5, -0.5,  0.5, 0, 0),
			mesh2d(-0.5,  0.5,  0.5, 0, 1),
			mesh2d(-0.5, -0.5, -0.5, 1, 0),

			mesh2d(0.5, 0.5, -0.5, 0, 0),
			mesh2d(0.5, -0.5, -0.5, 0, -1),
			mesh2d(-0.5, 0.5, -0.5, -1, 0),
		};

		UINT index[] = { 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

		x.resource->vert_count = sizeof(mesh) / sizeof(mesh2d);
		x.resource->indexCount = sizeof(index) / sizeof(UINT);

		//x.instances = new InstanceType_B[data->maxInstanceCount];

		x.resource->stride[0] = sizeof(mesh2d);
		x.resource->stride[1] = sizeof(InstanceType_B);

		x.resource->pVBuffer = CreateVertexBufferHelp(dev, (sizeof(mesh2d) * x.resource->vert_count), mesh);
		x.resource->pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * x.resource->indexCount, index);
	}

	gameModels->push_back(x); //1 cube

	{ //binary model
		mesh2d * mesh = nullptr;
		UINT * list = nullptr;

		MD5ModelBinary * mod = new MD5ModelBinary("test.txt");

		mod->LoadModel(0);
		mod->PrepareMesh(mesh, list, 0);

		x.resource->vert_count = mod->GetMeshSize();
		x.resource->indexCount = mod->GetIntSize();

		//x.resource->instances = new InstanceType_B[data->maxInstanceCount];

		x.resource->stride[0] = sizeof(mesh2d);
		x.resource->stride[1] = sizeof(InstanceType_B);

		x.resource->pVBuffer = CreateVertexBufferHelp(dev, (sizeof(mesh2d) * x.resource->vert_count), mesh);
		x.resource->pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * x.resource->indexCount, list);
		//x.resource->pInsBuffer = CreateInstanceBufferHelp(dev, sizeof(InstanceType_B) * data->maxInstanceCount, data->instances);

		//D3DCreateShaderResourceViewFromFile(dev, L"bob_body.dds", nullptr, nullptr, &(data->texture), nullptr);
		//D3DCreateShaderResourceViewFromFile(dev, L"bob_body.dds", nullptr, nullptr, &(data->texture), nullptr);

		SAFE_DELETE(mesh);
		SAFE_DELETE(list);
		SAFE_DELETE(mod);
	}

	gameModels->push_back(x); //2 model

	{
		//ID3D11Buffer *textureBuffer = nullptr;
		ID3D11ShaderResourceView *textureBuffer;
		auto texture = GenerateTextureData1();
		textureBuffer = CreateD3D11TextureResourceView(dev, D3D11_USAGE_DYNAMIC, 256, 256, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, texture, 256, 256*256);
		//CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, texture.size(), D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, &texture);
		y.resource = textureBuffer;
		//dev->CreateShaderResourceView()
	}

	textures->push_back(y); //0 texture
}

//TextureWidth and TextureHeight must be same
UINT8 * ResourcesManager::GenerateTextureData1()
{
	const UINT TextureWidth = 256;
	const UINT TextureHeight = 256;
	const UINT TexturePixelSize = 4;
	UINT rowPitch = TextureWidth * TexturePixelSize;
	UINT cellPitch = rowPitch >> 3;		// The width of a cell in the checkboard texture.
	UINT cellHeight = TextureWidth >> 3;	// The height of a cell in the checkerboard texture.
	UINT textureSize = rowPitch * TextureHeight;
	UINT8* data = new UINT8[textureSize];

	for (UINT n = 0; n < textureSize; n += TexturePixelSize)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

		if (i % 2 == j % 2)
		{
			data[n] = 0xff;		// R
			data[n + 1] = 0x11;	// G
			data[n + 2] = 0x11;	// B
			data[n + 3] = 0xff;	// A
		}
		else
		{
			data[n] = 0xff;		// R
			data[n + 1] = 0xff;	// G
			data[n + 2] = 0xff;	// B
			data[n + 3] = 0xff;	// A
		}
	}

	return data;
}
