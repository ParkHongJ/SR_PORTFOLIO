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

HRESULT CCollider::Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CBoxCollider* pBoxCollider, class CTransform* pTransform)
{
	if (nullptr == pBoxCollider)
		return E_FAIL;

	//m_CollisionObjects[eCollisionGroup].push_back(pBoxCollider);

	m_pCollisionObjects[eCollisionGroup].push_back(make_pair(pBoxCollider, pTransform));

	Safe_AddRef(pBoxCollider);
	Safe_AddRef(pTransform);

	return S_OK;
}

HRESULT CCollider::Collision_RectEx(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta)
{
	if (m_pCollisionObjects[eSourGroup].empty() || m_pCollisionObjects[eDestGroup].empty())
		return S_OK;

	for (auto& pSour : m_pCollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_pCollisionObjects[eDestGroup])
		{
			if (pDest.first == pSour.first)
			{
				continue;
			}
			/* 상호 중점간의 거리가 2.f보다 짧거나 같은 경우에만 충돌계산 */
			if (2.f >= abs(pSour.second->Get_State(CTransform::STATE_POSITION).x - pDest.second->Get_State(CTransform::STATE_POSITION).x)
				&& 2.f >= abs(pSour.second->Get_State(CTransform::STATE_POSITION).y - pDest.second->Get_State(CTransform::STATE_POSITION).y)
				&& 2.f >= abs(pSour.second->Get_State(CTransform::STATE_POSITION).z - pDest.second->Get_State(CTransform::STATE_POSITION).z)) {
				if (pSour.first->GetOwner()->GetEnabled() && pDest.first->GetOwner()->GetEnabled())
				{
					float	fX = 0.f, fZ = 0.f;
					if (Check_RectEx(pSour.first, pSour.second, pDest.first, pDest.second, &fX, &fZ))
					{
						if ((pSour.first->GetBoxDesc().bIsTrigger))
						{
							// 상하 충돌
							if (fX > fZ)
							{
								// 상 충돌
								if (pDest.second->Get_State(CTransform::STATE_POSITION).z < pSour.second->Get_State(CTransform::STATE_POSITION).z)
								{
									//투디, 탑디가 상대방 위에 있을때
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_UP);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_UP);
								}
								else // 하 충돌
								{
									//투디, 탑디가 상대방 아래에 있을때
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_DOWN);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_DOWN);
								}
							}
							else
							{
								//투디, 탑디가 상대방 왼쪽에 있을때
								// 좌 충돌
								if (pDest.second->Get_State(CTransform::STATE_POSITION).x > pSour.second->Get_State(CTransform::STATE_POSITION).x)
								{
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_LEFT);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_LEFT);
								}

								//투디, 탑디가 상대방 오른쪽에 있을때
								else // 우 충돌
								{
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_RIGHT);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_RIGHT);
								}
							}
						}
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT CCollider::Collision_SphereEx(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta)
{
	if (m_pCollisionObjects[eSourGroup].empty() || m_pCollisionObjects[eDestGroup].empty())
		return S_OK;

	for (auto& pSour : m_pCollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_pCollisionObjects[eDestGroup])
		{
			if (pDest.first == pSour.first)
			{
				continue;
			}
			//First : BoxCollider
			//Second : Transform
			if (Check_SphereEx(pSour.first, pSour.second, pDest.first, pDest.second))
			{
				pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_END);
				pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_END);
			}
		}
	}
	return S_OK;
}
bool CCollider::Check_SphereEx(CBoxCollider* pSourCol, CTransform* pSourTrans,  CBoxCollider* pDestCol, CTransform* pDestTrans)
{
	_float3 vDiff = pDestTrans->Get_State(CTransform::STATE_POSITION) - pSourTrans->Get_State(CTransform::STATE_POSITION);
	_float fDistance = abs(D3DXVec3Length(&vDiff));
	if ((pSourCol->GetRadius() + pDestCol->GetRadius()) >= fDistance)
	{
		return true;
	}
	return false;
}

HRESULT CCollider::Collision_TriggerXXX(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta)
{
	if (m_pCollisionObjects[eSourGroup].empty() || m_pCollisionObjects[eDestGroup].empty())
		return S_OK;

	unordered_map<LONGLONG, bool>::iterator iter;

	for (auto& pSour : m_pCollisionObjects[eSourGroup])
	{
		for (auto& pDest : m_pCollisionObjects[eDestGroup])
		{
			if (pDest.first == pSour.first)
			{
				continue;
			}
			COLLIDER_ID ID;
			ID.Left_ID = pSour.first->GetID();
			ID.Right_ID = pDest.first->GetID();
			iter = m_ColInfo.find(ID.ID);

			//충돌 정보가 미등록 상태라면
			if (m_ColInfo.end() == iter)
			{
				//등록해주고 다시찾음
				m_ColInfo.insert(make_pair(ID.ID, false));
				iter = m_ColInfo.find(ID.ID);
			}

			//bActive = 객체가 Tick, Late_Tick, Render를 돌지 안돌지
			if (pSour.first->GetOwner()->GetEnabled() && pDest.first->GetOwner()->GetEnabled())
			{
				if (Check_SphereEx(pSour.first, pSour.second, pDest.first, pDest.second))
				{
					//현재 충돌 중이다
					if (iter->second)
					{
						//이전에도 충돌 중이다
						pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_END);
						pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_END);
					}
					else
					{
						//이전에는 충돌하지 않았다
						pSour.first->GetOwner()->OnTriggerEnter(pDest.first->GetOwner(), fTimeDelta);
						pDest.first->GetOwner()->OnTriggerEnter(pSour.first->GetOwner(), fTimeDelta);
						iter->second = true;
					}
				}
				else
				{
					//현재 충돌하고있지않다
					if (iter->second)
					{
						//이전에는 충돌하고 있었다.
						pSour.first->GetOwner()->OnTriggerExit(pDest.first->GetOwner(), fTimeDelta);
						pDest.first->GetOwner()->OnTriggerExit(pSour.first->GetOwner(), fTimeDelta);
						iter->second = false;
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT CCollider::EndEx()
{
	for (auto& List : m_pCollisionObjects)
	{
		for (auto& Pair : List)
		{
			Safe_Release(Pair.first);
			Safe_Release(Pair.second);
		}
		List.clear();
	}
	return S_OK;
}

bool CCollider::Check_RectEx(class CBoxCollider* pSourCol, class CTransform* pSourTrans, class CBoxCollider* pDestCol, class CTransform* pDestTrans, float *pX, float* pZ)
{
	//정보 불러오기
	_float3 vSourMin = pSourCol->GetMin();
	_float3 vSourMax = pSourCol->GetMax();

	_float3 vDestMin = pDestCol->GetMin();
	_float3 vDestMax = pDestCol->GetMax();

	_float4x4 SourWorld;
	_float4x4 DestWorld;

	D3DXMatrixIdentity(&SourWorld);
	D3DXMatrixIdentity(&DestWorld);

	_float3 vSourPos = pSourTrans->Get_State(CTransform::STATE_POSITION);
	_float3 vDestPos = pDestTrans->Get_State(CTransform::STATE_POSITION);
	_float3 vSourScale = pSourTrans->Get_Scaled();
	_float3 vDestScale = pDestTrans->Get_Scaled();

	D3DXMatrixScaling(&SourWorld, vSourScale.x, vSourScale.y, vSourScale.z);
	D3DXMatrixScaling(&DestWorld, vDestScale.x, vDestScale.y, vDestScale.z);

	memcpy(&SourWorld.m[3][0], &vSourPos, sizeof(_float3));
	memcpy(&DestWorld.m[3][0], &vDestPos, sizeof(_float3));

	//콜라이더간 거리
	_float3		temp1 = pSourCol->GetBoxDesc().vPos;
	_float3		temp2 = pDestCol->GetBoxDesc().vPos;

	D3DXVec3TransformCoord(&temp1, &temp1, &SourWorld);
	D3DXVec3TransformCoord(&temp2, &temp2, &DestWorld);
	D3DXVec3TransformCoord(&vSourMin, &vSourMin, &SourWorld);
	D3DXVec3TransformCoord(&vSourMax, &vSourMax, &SourWorld);
	D3DXVec3TransformCoord(&vDestMin, &vDestMin, &DestWorld);
	D3DXVec3TransformCoord(&vDestMax, &vDestMax, &DestWorld);

	_float		fWidth = abs(temp1.x - temp2.x);
	_float		fDepth = abs(temp1.z - temp2.z);

	/* 이거 수정해라 */
	_float fCX = (abs(vDestMax.x - vDestMin.x) + abs(vSourMax.x - vSourMin.x)) * 0.5f;
	_float fCZ = (abs(vDestMax.z - vDestMin.z) + abs(vSourMax.z - vSourMin.z)) * 0.5f;

	if ((fCX > fWidth) && (fCZ > fDepth))
	{
		*pX = fCX - fWidth;
		*pZ = fCZ - fDepth;
		return TRUE;
	}
	return FALSE;
}

void CCollider::AddRayList(const _float3 & _vRayPos, const _float3 & _vRayDir)
{
	m_RayList.push_back(make_pair(_vRayPos, _vRayDir));
}

bool CCollider::Collision_Ray_Top(COLLISIONGROUP eDestGroup, _bool bTurn_Topdee)
{	
	if (m_RayList.empty())
		return false;
	list<pair<CGameObject*, _float>> RayCastedList; //first ObjectOwner Second ZSorting
	for (auto& pDest : m_pCollisionObjects[eDestGroup])
	{			//First : BoxCollider //Second : Transform
		if (m_pCollisionObjects[eDestGroup].empty())
			return false;
		CBoxCollider::BOXDESC pBoxDesc =pDest.first->GetBoxDesc();
		_float3 pBoxHalfSize{ pBoxDesc.vSize * 0.5f };
		_float3 pDestPos{ pDest.second->Get_State(CTransform::STATE_POSITION) };
		_float3 pBox_Top_VB[4];
		if (bTurn_Topdee) {//탑디턴일땐 y축기준 +된 위치의 렉트를 잡아주어야하고   
			pBox_Top_VB[0] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[1] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[2] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z - pBoxHalfSize.z);
			pBox_Top_VB[3] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z - pBoxHalfSize.z);
		}
		else {//투디턴일땐 x축기준 +된위치에 렉트를 잡아주어야한다.
			pBox_Top_VB[0] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[1] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[2] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y - pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[3] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y - pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
		}
		float		fU, fV, fDist;
		for (auto& Pair : m_RayList) {//Pair first = Pos second Dir
			if (bTurn_Topdee)
			{//탑디턴인데 dir이 - z이면 컨티뉴.
				if (Pair.second.z < 0.f)
					continue;
			}
			else
			{//투디턴인데 dir이 -y이면 컨티뉴
				if (Pair.second.y < 0.f)
					continue;
			}
			if (true == D3DXIntersectTri(&pBox_Top_VB[0], &pBox_Top_VB[1], &pBox_Top_VB[2], &Pair.first, &Pair.second, &fU, &fV, &fDist))
			{
				RayCastedList.push_back(make_pair(pDest.first->GetOwner(), pDest.second->Get_State(CTransform::STATE_POSITION).z));
				if (bTurn_Topdee) {
					pDest.first->GetOwner()->Set_bRayCasted(true);
					return true;
				}
			}

			/* 왼쪽 하단. */
			if (true == D3DXIntersectTri(&pBox_Top_VB[0], &pBox_Top_VB[2], &pBox_Top_VB[3], &Pair.first, &Pair.second, &fU, &fV, &fDist))
			{
				RayCastedList.push_back(make_pair(pDest.first->GetOwner(), pDest.second->Get_State(CTransform::STATE_POSITION).z));
				if (bTurn_Topdee) {
					pDest.first->GetOwner()->Set_bRayCasted(true);
					return true;
				}
			}

			pDest.first->GetOwner()->Set_bRayCasted(false);
		}
	}

	
	if(bTurn_Topdee)
		return false;
	else
	{
		if (RayCastedList.empty())
			return false;
		_float fBestPosZ{ 0.f };
		for (auto& Pair : RayCastedList)
		{//소팅을해서
			if( fBestPosZ < Pair.second)
				fBestPosZ = Pair.second;
		}
		for (auto&Pair : RayCastedList)
		{//제일큰놈
			if (Pair.second == fBestPosZ) {
				Pair.first->Set_bRayCasted(true);
				continue;
			}
		}
	}
	return true;
}

void CCollider::Clear_RayList()
{

	//for (auto& Pair : m_RayInfo)
	//{
	//	Safe_Release(Pair.first);
	//	Safe_Release(Pair.second);
	//}
	m_RayList.clear();
	
	return;
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

	for (auto& List : m_pCollisionObjects)
	{
		for (auto& Pair : List)
		{
			Safe_Release(Pair.first);
			Safe_Release(Pair.second);
		}
		List.clear();
	}
}



