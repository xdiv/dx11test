#pragma once

#include "DataModelBase.h"
#include "ShaderBase.h"
#include "../cdxml/math.h"
#include "ResourcesManager.h"

class NormalShader : public ShaderBase
{
public:
	NormalShader(ID3D* d3d) : ShaderBase(d3d) {};
	~NormalShader() {};

	void Init();
	void Render(DMBdata* data);
};

