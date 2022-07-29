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

//HRESULT CCollider::Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CBoxCollider* pBoxCollider)
//{
//	if (nullptr == pBoxCollider)
//		return E_FAIL;
//
//	m_CollisionObjects[eCollisionGroup].push_back(pBoxCollider);
//
//	Safe_AddRef(pBoxCollider);
//
//	return S_OK;
//}

HRESULT CCollider::Collision_Rect(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta)
{
	unordered_map<LONGLONG, bool>::iterator iter;

	for (auto& pSour : m_CollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_CollisionObjects[eDestGroup])
		{
			if (pSour->GetEnabled() && pDest->GetEnabled())
			{
				float	fX = 0.f, fZ = 0.f;
				if (Check_RectEx(pSour, pDest, &fX, &fZ))
				{
					CTransform* DestTrans = ((CTransform*)pDest->Get_Component(L"Com_Transform"));
					CTransform* SourTrans = ((CTransform*)pSour->Get_Component(L"Com_Transform"));

					Safe_AddRef(DestTrans);
					Safe_AddRef(SourTrans);
					if (((CBoxCollider*)pSour->Get_Component(L"Com_BoxCollider"))->GetBoxDesc().bIsTrigger)
					{
						// 상하 충돌
						if (fX > fZ)
						{
							// 상 충돌
							if (DestTrans->Get_State(CTransform::STATE_POSITION).z > SourTrans->Get_State(CTransform::STATE_POSITION).z)
							{
								//투디, 탑디가 상대방 위에 있을때
								pSour->OnTriggerStay(pDest, fTimeDelta, DIR_UP);
								pDest->OnTriggerStay(pSour, fTimeDelta, DIR_UP);
							}
							else // 하 충돌
							{
								//투디, 탑디가 상대방 아래에 있을때
								pSour->OnTriggerStay(pDest, fTimeDelta, DIR_DOWN);
								pDest->OnTriggerStay(pSour, fTimeDelta, DIR_DOWN);
							}
						}
						else
						{

							//투디, 탑디가 상대방 왼쪽에 있을때
							// 좌 충돌
							if (DestTrans->Get_State(CTransform::STATE_POSITION).x > SourTrans->Get_State(CTransform::STATE_POSITION).x)
							{
								pSour->OnTriggerStay(pDest, fTimeDelta, DIR_LEFT);
								pDest->OnTriggerStay(pSour, fTimeDelta, DIR_LEFT);
							}

							//투디, 탑디가 상대방 오른쪽에 있을때
							else // 우 충돌
							{
								pSour->OnTriggerStay(pDest, fTimeDelta, DIR_RIGHT);
								pDest->OnTriggerStay(pSour, fTimeDelta, DIR_RIGHT);
							}
						}
						Safe_Release(DestTrans);
						Safe_Release(SourTrans);
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT CCollider::Collision_Sphere(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta)
{
	for (auto& pSour : m_CollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_CollisionObjects[eDestGroup])
		{
			if (pSour->GetEnabled() && pDest->GetEnabled())
			{
				if (Check_Sphere(pSour, pDest))
				{
					pSour->OnTriggerStay(pDest, fTimeDelta, DIR_END);
					pDest->OnTriggerStay(pSour, fTimeDelta, DIR_END);
				}
			}
		}
	}
	return S_OK;
}

HRESULT CCollider::Collision_TriggerXXX(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta)
{
	unordered_map<LONGLONG, bool>::iterator iter;

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

			//bActive = 객체가 Tick, Late_Tick, Render를 돌지 안돌지
			if (pSour->GetEnabled() && pDest->GetEnabled())
			{
				if (Check_Sphere(pSour, pDest))
				{
					//현재 충돌 중이다
					if (iter->second)
					{
						//이전에도 충돌 중이다
						pSour->OnTriggerStay(pDest, fTimeDelta, DIR_END);
						pDest->OnTriggerStay(pSour, fTimeDelta, DIR_END);
					}
					else
					{
						//이전에는 충돌하지 않았다
						pSour->OnTriggerEnter(pDest, fTimeDelta);
						pDest->OnTriggerEnter(pSour, fTimeDelta);
						iter->second = true;
					}
				}
				else
				{
					//현재 충돌하고있지않다
					if (iter->second)
					{
						//이전에는 충돌하고 있었다.
						pSour->OnTriggerExit(pDest, fTimeDelta);
						pDest->OnTriggerExit(pSour, fTimeDelta);
						iter->second = false;
					}

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

bool CCollider::Check_Sphere(CGameObject * pSour, CGameObject * pDest)
{
	CTransform*	SourTrans = (CTransform*)pSour->Get_Component(L"Com_Transform");
	CTransform*	DestTrans = (CTransform*)pDest->Get_Component(L"Com_Transform");

	Safe_AddRef(SourTrans);
	Safe_AddRef(DestTrans);

	CBoxCollider* SourCol = (CBoxCollider*)pSour->Get_Component(L"Com_BoxCollider");
	CBoxCollider* DestCol = (CBoxCollider*)pDest->Get_Component(L"Com_BoxCollider");

	Safe_AddRef(SourCol);
	Safe_AddRef(DestCol);
	_float3 vDiff = DestTrans->Get_State(CTransform::STATE_POSITION) - SourTrans->Get_State(CTransform::STATE_POSITION);
	_float fDistance = D3DXVec3Length(&vDiff);
	if ((SourCol->GetRadius() + DestCol->GetRadius()) >= fDistance)
	{

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);

		Safe_Release(SourCol);
		Safe_Release(DestCol);
		return true;
	}

	Safe_Release(SourTrans);
	Safe_Release(DestTrans);

	Safe_Release(SourCol);
	Safe_Release(DestCol);
	return false;
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
	_float3 vSourScale = SourTrans->Get_Scaled();
	_float3 vDestScale = DestTrans->Get_Scaled();

	memcpy(&SourWorld.m[3][0], &vSourPos, sizeof(_float3));
	memcpy(&DestWorld.m[3][0], &vDestPos, sizeof(_float3));
	D3DXMatrixScaling(&SourWorld, vSourScale.x, vSourScale.y, vSourScale.z);
	D3DXMatrixScaling(&DestWorld, vDestScale.x, vDestScale.y, vDestScale.z);

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
	_float3 vSourScale = SourTrans->Get_Scaled();
	_float3 vDestScale = DestTrans->Get_Scaled();

	D3DXMatrixScaling(&SourWorld, vSourScale.x, vSourScale.y, vSourScale.z);
	D3DXMatrixScaling(&DestWorld, vDestScale.x, vDestScale.y, vDestScale.z);

	memcpy(&SourWorld.m[3][0], &vSourPos, sizeof(_float3));
	memcpy(&DestWorld.m[3][0], &vDestPos, sizeof(_float3));

	/*D3DXVec3TransformCoord(&vSourMin, &vSourMin, &SourWorld);
	D3DXVec3TransformCoord(&vSourMax, &vSourMax, &SourWorld);
	D3DXVec3TransformCoord(&vDestMin, &vDestMin, &DestWorld);
	D3DXVec3TransformCoord(&vDestMax, &vDestMax, &DestWorld);*/

	_float3		temp1 = SourCol->GetBoxDesc().vPos;
	_float3		temp2 = DestCol->GetBoxDesc().vPos;
	D3DXVec3TransformCoord(&temp1, &temp1, &SourWorld);
	D3DXVec3TransformCoord(&temp2, &temp2, &DestWorld);

	_float		fWidth = abs(temp1.x - temp2.x);
	_float		fDepth = abs(temp1.z - temp2.z);

	//_float		fWidth = abs(vSourPos.x - vDestPos.x);
	//_float		fDepth = abs(vSourPos.z - vDestPos.z);

	/* 이거 수정해라 */
	_float fCX = (abs(vDestMax.x - vDestMin.x) + abs(vSourMax.x - vSourMin.x)) * 0.5f;
	_float fCZ = (abs(vDestMax.z - vDestMin.z) + abs(vSourMax.z - vSourMin.z)) * 0.5f;

	if ((fCX > fWidth) && (fCZ > fDepth))
	{
		*pX = fCX - fWidth;
		*pZ = fCZ - fDepth;

		Safe_Release(SourCol);
		Safe_Release(DestCol);

		Safe_Release(SourTrans);
		Safe_Release(DestTrans);
		return TRUE;
	}

	Safe_Release(SourCol);
	Safe_Release(DestCol);

	Safe_Release(SourTrans);
	Safe_Release(DestTrans);

	return FALSE;
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



