#pragma once
#include "DX_Global.h"

#include <Windows.h>
#include <iostream>
#include <sstream>

using namespace DirectX;

class Camera
{
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMVECTOR position; //v3
	XMMATRIX m_viewMatrix;
	XMMATRIX view2dMatrix;

	std::ostringstream os_;

public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float x, float y, float z);

	void SetRotation(float x, float y, float z);

	void MoveCamera(float dx, float dy, float dz);

	XMVECTOR GetPosition(); //v3
	XMVECTOR GetRotation(); //v3

	void Render3DCamera();
	void Render2DCamera();
	void GetView3DMatrix(XMMATRIX&);
	void GetView2DMatrix(XMMATRIX&);
};

