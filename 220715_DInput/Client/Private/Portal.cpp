#include "stdafx.h"
#include "..\Public\Portal.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPortal::CPortal(const CPortal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CPortal::Tick(_float fTimeDelta)
{
}

void CPortal::LateTick(_float fTimeDelta)
{
}

HRESULT CPortal::Render()
{
	return S_OK;
}

void CPortal::OnTriggerExit(CGameObject * other)
{
}

void CPortal::OnTriggerEnter(CGameObject * other)
{
}

void CPortal::OnTriggerStay(CGameObject * other)
{
}

HRESULT CPortal::Set_RenderState()
{
	return S_OK;
}

HRESULT CPortal::Reset_RenderState()
{
	return S_OK;
}

HRESULT CPortal::SetUp_Components()
{
	return S_OK;
}

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPortal* pInstance = new CPortal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();
}
