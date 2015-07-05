#include "GameModelBase.h"


GameModelBase::GameModelBase()
{
	position = float3::Zero();
	rotation = float3::Zero();
	scale = float3::Zero();
	boundbox = float3::Zero();
	renderer = 0;
}


GameModelBase::~GameModelBase()
{
}

void GameModelBase::Render()
{
	renderer->AddInstance(InstanceType_B(position, color_rgba::Zero(), scale, rotation));
}

void GameModelBase::SetPosition(float3* p)
{
	position.x = p->x;
	position.y = p->y;
	position.z = p->z;
}

void GameModelBase::SetRotation(float3*r)
{
	rotation.x = r->x;
	rotation.y = r->y;
	rotation.z = r->z;
}

void GameModelBase::SetScale(float3*s)
{
	scale.x = s->x;
	scale.y = s->y;
	scale.z = s->z;
}

void GameModelBase::SetBoundbox(float3*b)
{
	boundbox.x = b->x;
	boundbox.y = b->y;
	boundbox.z = b->z;
}

void GameModelBase::GetPosition(float3* p)
{
	p->x = position.x;
	p->y = position.y;
	p->z = position.z;
}

void GameModelBase::GetRotation(float3* r)
{
	r->x = rotation.x;
	r->y = rotation.y;
	r->z = rotation.z;
}

void GameModelBase::GetScale(float3* s)
{
	s->x = scale.x;
	s->y = scale.y;
	s->z = scale.z;
}

void GameModelBase::GetBoundbox(float3* b)
{
	b->x = boundbox.x;
	b->z = boundbox.z;
	b->y = boundbox.y;
}