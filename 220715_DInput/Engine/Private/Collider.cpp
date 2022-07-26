#include "..\Public\Collider.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"


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
	return S_OK;
}

HRESULT CCollider::Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_CollisionObjects[eCollisionGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}


HRESULT CCollider::Collision_Rect(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup)
{
	//현재 렌더러는 모든 리스트를 순회함
	//콜라이더는 다르게해야함
	//pDest->OnCollisionEnter(pSour)
	//pSour->OnCollisionEnter(pDest)

	for (auto& pSour : m_CollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_CollisionObjects[eDestGroup])
		{
			if (Check_Rect(pSour, pDest))
			{
				pSour->OnTriggerStay(pDest);
				pDest->OnTriggerStay(pSour);
			}
		}
	}
	return S_OK;
}

HRESULT Engine::CCollider::End()
{
	for (auto& List : m_CollisionObjects)
	{
		for (auto& pGameObject : List)
		{
			Safe_Release(pGameObject);
		}
		List.clear();
	}
	return S_OK;
}

bool CCollider::Check_Rect(class CGameObject* pSour, class CGameObject* pDest)
{
	CBoxCollider* SourCol = (CBoxCollider*)pSour->Get_Component(L"Com_BoxCollider");
	CBoxCollider* DestCol = (CBoxCollider*)pDest->Get_Component(L"Com_BoxCollider");
	if (SourCol == nullptr || DestCol == nullptr)
	{
		return false;
	}
	Safe_AddRef(SourCol);
	Safe_AddRef(DestCol);

	CTransform* SourTrans = (CTransform*)pSour->Get_Component(L"Com_Transform");
	CTransform* DestTrans = (CTransform*)pDest->Get_Component(L"Com_Transform");
	if (SourTrans == nullptr || DestTrans == nullptr)
	{
		return false;
	}
	Safe_AddRef(SourTrans);
	Safe_AddRef(DestTrans);

	//정보 불러오기
	_float3 vSourMin = SourCol->GetMin();
	_float3 vSourMax = SourCol->GetMax();

	_float3 vDestMin = DestCol->GetMin();
	_float3 vDestMax = DestCol->GetMax();

	_float4x4 SourWorld;
	_float4x4 DestWorld;

	D3DXMatrixIdentity(&SourWorld);
	D3DXMatrixIdentity(&DestWorld);
	_float3 vSourPos = SourTrans->Get_State(CTransform::STATE_POSITION);
	_float3 vDestPos = DestTrans->Get_State(CTransform::STATE_POSITION);
	memcpy(&SourWorld.m[3][0], &vSourPos, sizeof(_float3));
	memcpy(&DestWorld.m[3][0], &vDestPos, sizeof(_float3));

	D3DXVec3TransformCoord(&vSourMin, &vSourMin, &SourWorld);
	D3DXVec3TransformCoord(&vSourMax, &vSourMax, &SourWorld);
	D3DXVec3TransformCoord(&vDestMin, &vDestMin, &DestWorld);
	D3DXVec3TransformCoord(&vDestMax, &vDestMax, &DestWorld);

	//x축에 대하여
	if (vSourMax.x < vDestMin.x ||
		vSourMin.x > vDestMax.x)
	{
		Safe_Release(SourCol);
		Safe_Release(DestCol);

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);
		return false;
	}

	//y축에 대하여
	if (vSourMax.y < vDestMin.y ||
		vSourMin.y > vDestMax.y)
	{
		Safe_Release(SourCol);
		Safe_Release(DestCol);

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);
		return false;
	}

	//z축에 대하여
	if (vSourMax.z < vDestMin.z ||
		vSourMin.z > vDestMax.z)
	{
		Safe_Release(SourCol);
		Safe_Release(DestCol);

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);
		return false;
	}

	Safe_Release(SourCol);
	Safe_Release(DestCol);

	Safe_Release(SourTrans);
	Safe_Release(DestTrans);

	return TRUE;
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
	AddRef();

	return this;
}

void CCollider::Free()
{
	__super::Free();

	for (auto& List : m_CollisionObjects)
	{
		for (auto& pGameObject : List)
		{
			Safe_Release(pGameObject);
		}
		List.clear();
	}
}



