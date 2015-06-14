#pragma once
#include "DX_Global.h"

class Camera
{
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX view2dMatrix;

public:
	Camera();
	Camera(const Camera&);
	~Camera();

	/*
	3d camera:
	-x/x kaire desine
	-y/y atgal gylyn
	-z/z i zemyn ivirsu
	2d camera:
	-x/x kaire desinė;
	-y/y virsun zemyn;
	*/
	void SetPosition(float x, float y, float z);

	/*
		3d ir 2d cameros posukiai yra sumaišyti
	*/
	void SetRotation(float x, float y, float z);

	void MoveCamera(float dx, float dy, float dz);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render3DCamera();
	void Render2DCamera();
	void GetView3DMatrix(D3DXMATRIX&);
	void GetView2DMatrix(D3DXMATRIX&);
};

