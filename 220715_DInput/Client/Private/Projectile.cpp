#include "stdafx.h"
#include "..\Public\Projectile.h"

#include "GameInstance.h"
CProjectile::CProjectile(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CProjectile::CProjectile(const CProjectile & rhs)
	: CGameObject(rhs)
{
}

HRESULT CProjectile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CProjectile::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT Client::CProjectile::SetUp_Components()
{
	return S_OK;
}

void CProjectile::Tick(_float fTimeDelta)
{

}

void Client::CProjectile::LateTick(_float fTimeDelta)
{

}

HRESULT Client::CProjectile::Render()
{
	return S_OK;
}

HRESULT Client::CProjectile::Set_RenderState()
{
	return S_OK;
}

HRESULT CProjectile::Reset_RenderState()
{
	return S_OK;
}

CProjectile * CProjectile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject* Client::CProjectile::Clone(void* pArg)
{
	return nullptr;
}

void Client::CProjectile::Free()
{

}