#pragma once
#include "DX_Global.h"

#include <Windows.h>
#include <iostream>
#include <sstream>

class Camera
{
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXVECTOR3 position;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX view2dMatrix;

	std::ostringstream os_;

public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float x, float y, float z);

	void SetRotation(float x, float y, float z);

	void MoveCamera(float dx, float dy, float dz);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render3DCamera();
	void Render2DCamera();
	void GetView3DMatrix(D3DXMATRIX&);
	void GetView2DMatrix(D3DXMATRIX&);
};

