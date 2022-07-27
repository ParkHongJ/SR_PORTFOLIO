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
	map<LONGLONG, bool>::iterator iter;

	for (auto& pSour : m_CollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_CollisionObjects[eDestGroup])
		{
			COLLIDER_ID ID;
			ID.Left_ID = ((CBoxCollider*)pSour->Get_Component(L"Com_BoxCollider"))->GetID();
			ID.Right_ID = ((CBoxCollider*)pDest->Get_Component(L"Com_BoxCollider"))->GetID();
			iter = m_ColInfo.find(ID.ID);

			//충돌 정보가 미등록 상태라면
			if (m_ColInfo.end() == iter)
			{
				//등록해주고 다시찾음
				m_ColInfo.insert(make_pair(ID.ID, false));
				iter = m_ColInfo.find(ID.ID);
			}
			float	fX = 0.f, fZ = 0.f;
			//if (Check_Rect(pSour, pDest, &fX, &fZ))
			if (Check_Rect(pSour, pDest))
			{
				//현재 충돌 중이다
				if (iter->second)
				{
					//이전에도 충돌 중이다
					pSour->OnTriggerStay(pDest);
					pDest->OnTriggerStay(pSour);

					// 상하 충돌
					//if (fX > fZ)
					//{
					//	// 상 충돌
					//	if (((CTransform*)pDest->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION).z > ((CTransform*)pSour->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION).z)
					//	{
					//		((CTransform*)pSour->Get_Component(L"Com_Transform"))->Translate(_float3(0.f, 0.f, -fZ));
					//	}
					//	else // 하 충돌
					//	{
					//		((CTransform*)pSour->Get_Component(L"Com_Transform"))->Translate(_float3(0.f, 0.f, fZ));
					//	}
					//}
				}
				else
				{
					//이전에는 충돌하지 않았다
					pSour->OnTriggerEnter(pDest);
					pDest->OnTriggerEnter(pSour);
					iter->second = true;
				}
			}
			else
			{
				//현재 충돌하고있지않다
				if (iter->second)
				{
					//이전에는 충돌하고 있었다.
					pSour->OnTriggerExit(pDest);
					pDest->OnTriggerExit(pSour);
					iter->second = false;
				}
			}
		}
	}
	return S_OK;
}

HRESULT CCollider::End()
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
		return FALSE;
	}
	Safe_AddRef(SourCol);
	Safe_AddRef(DestCol);

	CTransform* SourTrans = (CTransform*)pSour->Get_Component(L"Com_Transform");
	CTransform* DestTrans = (CTransform*)pDest->Get_Component(L"Com_Transform");
	if (SourTrans == nullptr || DestTrans == nullptr)
	{
		return FALSE;
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
		return FALSE;
	}

	//y축에 대하여
	if (vSourMax.y < vDestMin.y ||
		vSourMin.y > vDestMax.y)
	{
		Safe_Release(SourCol);
		Safe_Release(DestCol);

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);
		return FALSE;
	}

	//z축에 대하여
	if (vSourMax.z < vDestMin.z ||
		vSourMin.z > vDestMax.z)
	{
		Safe_Release(SourCol);
		Safe_Release(DestCol);

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);
		return FALSE;
	}

	Safe_Release(SourCol);
	Safe_Release(DestCol);

	Safe_Release(SourTrans);
	Safe_Release(DestTrans);

	return TRUE;
}

bool CCollider::Check_RectEx(class CGameObject* pSour, class CGameObject* pDest, float *pX, float* pZ)
{
	CBoxCollider* SourCol = (CBoxCollider*)pSour->Get_Component(L"Com_BoxCollider");
	CBoxCollider* DestCol = (CBoxCollider*)pDest->Get_Component(L"Com_BoxCollider");
	if (SourCol == nullptr || DestCol == nullptr)
	{
		return FALSE;
	}
	Safe_AddRef(SourCol);
	Safe_AddRef(DestCol);

	CTransform* SourTrans = (CTransform*)pSour->Get_Component(L"Com_Transform");
	CTransform* DestTrans = (CTransform*)pDest->Get_Component(L"Com_Transform");
	if (SourTrans == nullptr || DestTrans == nullptr)
	{
		return FALSE;
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


	_float		fWidth = abs(vSourPos.x - vDestPos.x);
	_float		fDepth = abs(vSourPos.z - vDestPos.z);

	/* 이거 수정해라 */
	_float		fCX = 1.f;//(pDest->Get_Info().fCX + pSour->Get_Info().fCX) * 0.5f; // 1
	_float		fCZ = 1.f;//(pDest->Get_Info().fCY + pSour->Get_Info().fCY) * 0.5f; // 1

	if ((fCX > fWidth) && (fCZ > fDepth))
	{
		*pX = fCX - fWidth;
		*pZ = fCZ - fDepth;
		return true;
	}
	return false;
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



