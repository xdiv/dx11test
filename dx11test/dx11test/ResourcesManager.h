#pragma once

#include "DX_Global.h"
#include <vector>

using namespace std;

enum ResourceState
{
	Empty,
	Loading,
	Loaded
};

typedef struct StateContain
{
	StateContain() : state(ResourceState::Empty) {};
	ResourceState state;
} StateContain;

struct GameModel
{
	GameModel() : pVBuffer(nullptr), pIBuffer(nullptr), 
		vert_count(0), indexCount(0) {};
	ID3D11Buffer* pVBuffer;
	ID3D11Buffer* pIBuffer;
	UINT vert_count;
	UINT indexCount;
	UINT stride[2];
	UINT offset[2];
	inline void Release() { SAFE_RELEASE(pVBuffer) SAFE_RELEASE(pIBuffer) };
};

struct PixelShaderRes : public StateContain
{
	PixelShaderRes() : shader(nullptr) {};
	ID3D11VertexShader* shader;
};

template <class T>
struct ResourceContainer
{
	ResourceContainer() : state(ResourceState::Empty), resource(nullptr) {};
	//~ResourceContainer() { SAFE_RELEASE(resource); };
	T *resource;
	ResourceState state;
};

class ResourcesManager
{
private:
	//static ResourcesManager *singleton;
	vector<ResourceContainer<GameModel>> *gameModels;
	vector<ResourceContainer<ID3D11ShaderResourceView>> *textures;

public:
	ResourcesManager();
	~ResourcesManager();

	void Init(ID3D11Device * dev);
	GameModel* GetGameModelById(uint32_t id);
	ID3D11ShaderResourceView* GetTextureById(uint32_t id);

	//ResourcesManager* GetInstance();

private:
	void setBasicData(ID3D11Device * dev);
	UINT8 * GenerateTextureData1();
};

