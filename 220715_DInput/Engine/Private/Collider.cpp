#include "..\Public\Collider.h"
#include "GameObject.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
{

}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	//컴포넌트 받아오기

	return S_OK;
}

HRESULT CCollider::OnCollision()
{
	for (auto& Sour : m_CollisionObjects)
	{
		for (auto& Dest : m_CollisionObjects)
		{
			if ((nullptr != Dest) && (nullptr != Sour) && (Dest != Sour))
			{
				//Enter, Stay, Exit
			}

			Safe_Release(Dest);
		}
		

		Safe_Release(Sour);
	}
	m_CollisionObjects.clear();
	return S_OK;
}

HRESULT CCollider::Add_CollisionGroup(class CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_CollisionObjects.push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CCollider::Render()
{
	return S_OK;
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider*			pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider*			pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();
}



