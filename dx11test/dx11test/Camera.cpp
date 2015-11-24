#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}


void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::MoveCamera(float dx, float dy, float dz)
{
	//XMVECTOR up;
	//XMMATRIX rotationMatrix;

	//up.x = dx;
	//up.y = dy;
	//up.z = dz;

	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, 0, m_rotationY, m_rotationX);
	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//m_positionX += up.x;
	//m_positionY += up.y;
	//m_positionZ += up.z;
}

void Camera::SetRotation(float x, float y, float z)
{
	//os_ << x << " " << y << "\n";
	//OutputDebugStringA(os_.str().c_str());

	m_rotationX += x * 0.0174532925f;
	m_rotationY += y * 0.0174532925f;
	m_rotationZ += z * 0.0174532925f;
	return;
}


XMVECTOR Camera::GetPosition()
{
	return DirectX::XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0f);
}


XMVECTOR Camera::GetRotation()
{
	return DirectX::XMVectorSet(m_rotationX, m_rotationY, m_rotationZ, 0.0f);
}


void Camera::Render3DCamera()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	//up.x = 0.0f;	up.y = 0.0f;	up.z = 1.0f;

	up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Setup the position of the camera in the world.
	//position.x = m_positionX; 	position.y = m_positionY;	position.z = m_positionZ;
	position = DirectX::XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0f);

	// Setup where the camera is looking by default.
	//lookAt.x = 0.0f;	lookAt.y = 1.0f;	lookAt.z = 0.0f;
	lookAt = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	//pitch = m_rotationX;
	//yaw = m_rotationY * 0.0174532925f;
	//roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(0, m_rotationY, m_rotationX);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}

void Camera::Render2DCamera()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	//up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;

	up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	//position.x = m_positionX; position.y = m_positionY; position.z = m_positionZ;
	position = DirectX::XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0f);

	// Setup where the camera is looking by default.
	//lookAt.x = 0.0f; lookAt.y = 0.0f;	lookAt.z = 1.0f;
	lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;//// Create the rotation matrix from the yaw, pitch, and roll values.

	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	rotationMatrix = XMMatrixRotationRollPitchYaw(0, m_rotationY, m_rotationX);
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationY, m_rotationX, m_rotationZ);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.

	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&view2dMatrix, &position, &lookAt, &up);
	view2dMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void Camera::GetView3DMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::GetView2DMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = view2dMatrix;
}
