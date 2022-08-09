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
			/* ��ȣ �������� �Ÿ��� 2.f���� ª�ų� ���� ��쿡�� �浹��� */
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
							// ���� �浹
							if (fX > fZ)
							{
								// �� �浹
								if (pDest.second->Get_State(CTransform::STATE_POSITION).z < pSour.second->Get_State(CTransform::STATE_POSITION).z)
								{
									//����, ž�� ���� ���� ������
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_UP);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_UP);
								}
								else // �� �浹
								{
									//����, ž�� ���� �Ʒ��� ������
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_DOWN);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_DOWN);
								}
							}
							else
							{
								//����, ž�� ���� ���ʿ� ������
								// �� �浹
								if (pDest.second->Get_State(CTransform::STATE_POSITION).x > pSour.second->Get_State(CTransform::STATE_POSITION).x)
								{
									pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_LEFT);
									pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_LEFT);
								}

								//����, ž�� ���� �����ʿ� ������
								else // �� �浹
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

			//�浹 ������ �̵�� ���¶��
			if (m_ColInfo.end() == iter)
			{
				//������ְ� �ٽ�ã��
				m_ColInfo.insert(make_pair(ID.ID, false));
				iter = m_ColInfo.find(ID.ID);
			}

			//bActive = ��ü�� Tick, Late_Tick, Render�� ���� �ȵ���
			if (pSour.first->GetOwner()->GetEnabled() && pDest.first->GetOwner()->GetEnabled())
			{
				if (Check_SphereEx(pSour.first, pSour.second, pDest.first, pDest.second))
				{
					//���� �浹 ���̴�
					if (iter->second)
					{
						//�������� �浹 ���̴�
						pSour.first->GetOwner()->OnTriggerStay(pDest.first->GetOwner(), fTimeDelta, DIR_END);
						pDest.first->GetOwner()->OnTriggerStay(pSour.first->GetOwner(), fTimeDelta, DIR_END);
					}
					else
					{
						//�������� �浹���� �ʾҴ�
						pSour.first->GetOwner()->OnTriggerEnter(pDest.first->GetOwner(), fTimeDelta);
						pDest.first->GetOwner()->OnTriggerEnter(pSour.first->GetOwner(), fTimeDelta);
						iter->second = true;
					}
				}
				else
				{
					//���� �浹�ϰ������ʴ�
					if (iter->second)
					{
						//�������� �浹�ϰ� �־���.
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
	//���� �ҷ�����
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

	//�ݶ��̴��� �Ÿ�
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

	/* �̰� �����ض� */
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
		if (bTurn_Topdee) {//ž�����϶� y����� +�� ��ġ�� ��Ʈ�� ����־���ϰ�   
			pBox_Top_VB[0] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[1] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[2] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z - pBoxHalfSize.z);
			pBox_Top_VB[3] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z - pBoxHalfSize.z);
		}
		else {//�������϶� x����� +����ġ�� ��Ʈ�� ����־���Ѵ�.
			pBox_Top_VB[0] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[1] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y + pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[2] = _float3(pDestPos.x + pBoxHalfSize.x, pDestPos.y - pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
			pBox_Top_VB[3] = _float3(pDestPos.x - pBoxHalfSize.x, pDestPos.y - pBoxHalfSize.y, pDestPos.z + pBoxHalfSize.z);
		}
		float		fU, fV, fDist;
		for (auto& Pair : m_RayList) {//Pair first = Pos second Dir
			if (bTurn_Topdee)
			{//ž�����ε� dir�� - z�̸� ��Ƽ��.
				if (Pair.second.z < 0.f)
					continue;
			}
			else
			{//�������ε� dir�� -y�̸� ��Ƽ��
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

			/* ���� �ϴ�. */
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
		{//�������ؼ�
			if( fBestPosZ < Pair.second)
				fBestPosZ = Pair.second;
		}
		for (auto&Pair : RayCastedList)
		{//����ū��
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



