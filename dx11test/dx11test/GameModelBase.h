#pragma once

#include "pch.h"
#include "..\cdxml\complex_types.h"
#include "DataModelBase.h"


class GameModelBase
{
private:
	float3 position;
	float3 rotation;
	float3 scale;
	float3 boundbox;

	DataModelBase * renderer;

	bool visible;
public:
	GameModelBase();
	~GameModelBase();

	void Render();

	void SetPosition(float3*);
	void SetRotation(float3*);
	void SetScale(float3*);
	void SetBoundbox(float3*);

	void GetPosition(float3*);
	void GetRotation(float3*);
	void GetScale(float3*);
	void GetBoundbox(float3*);
};

