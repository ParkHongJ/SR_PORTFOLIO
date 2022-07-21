#include "..\Public\Component.h"
#include "..\Public\GameObject.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{

	//Safe_Release(m_pOwner);

	Safe_Release(m_pGraphic_Device);
}

void CComponent::SetOwner(CGameObject * _pOwner)
{
	m_pOwner = _pOwner;
}
