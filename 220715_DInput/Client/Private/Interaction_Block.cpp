#include "stdafx.h"
#include "..\Public\Interaction_Block.h"

#include "GameInstance.h"
#include "GameMgr.h"

CInteraction_Block::CInteraction_Block(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CInteraction_Block::CInteraction_Block(const CInteraction_Block & rhs)
	: CGameObject(rhs)
{
}

HRESULT CInteraction_Block::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteraction_Block::Initialize(void * pArg)
{
	return S_OK;
}

void CInteraction_Block::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	if (m_bDropFinish)
		return;
	if (m_bDropBox)
	{
		m_bAbility = false;
		Box_Drop_More(fTimeDelta);
	}
	if (m_bTopdeePush)
		Box_Push_More(fTimeDelta,m_vPushFinishPos,false);
}

void CInteraction_Block::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

HRESULT CInteraction_Block::Render()
{
	return S_OK;
}

#pragma region About_Topdee
bool CInteraction_Block::KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta, _float3 vPreLoaderPos)
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vDist = (vCurPosition - vFinalPos);
	_float fLength = D3DXVec3Length(&vDist);
	if (_int(fLength) <= 1)
	{
		if (((_int)vPreLoaderPos.x == (_int)vCurPosition.x) && ((_int)vPreLoaderPos.z == (_int)vCurPosition.z)) {
			vCurPosition = vCurPosition + (vFinalPos - vCurPosition) * (fTimeDelta * 5);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
			return true;
		}
		return false;
	}
	else
		return false;
}

void CInteraction_Block::KKK_Is_Raise(_float3 vTargetPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,vTargetPos);
}

_bool CInteraction_Block::KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta, _bool bHoleCall)
{
	_float fBoxSpeed{ m_pTransformCom->Get_Speed() };
	_float3 vCurPosition{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };//Box Cur Pos.
	if (!bHoleCall) {
		_float3 vHaveToReturnPos{ vFinalPos };
		vFinalPos = MoveTowards(vCurPosition, vFinalPos, fTimeDelta * fBoxSpeed*1.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFinalPos);
		if (vFinalPos == vHaveToReturnPos) {
			return true;
		}
		return false;
	}
	else {
		m_bDropBox = true;
		m_bEnabled = false;
	}
		return false;
}

void CInteraction_Block::Box_Drop_More(_float fTimeDelta)
{
	m_bEnabled = false;
	_float3 vBoxCurPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
	if (vBoxCurPos.y <= -0.45f) {//final Position is -0.45
		vBoxCurPos.y = -0.45f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vBoxCurPos));
		m_bDropBox = false;
		m_bDropFinish = true;
		return;
	}
	_float3 vBoxDir = { 0.f,-1.f,0.f };
	_float fBoxSpeed = m_pTransformCom->Get_Speed();
	m_pTransformCom->Translate(vBoxDir *fTimeDelta* fBoxSpeed * 2.f);
}

void CInteraction_Block::Box_Push_More(_float fTimeDelta, _float3 vPushFinishPos, _bool bFirstCall)
{
	if (!m_bEnabled)
		return;
	if (bFirstCall)
	{
		m_bTopdeePush = true;
		m_vPushFinishPos = vPushFinishPos;
	}
	_float3 vCurPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
	_float fBoxSpeed{ m_pTransformCom->Get_Speed() };
	vCurPos = MoveTowards(vCurPos, m_vPushFinishPos, fTimeDelta*fBoxSpeed);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
	if (vCurPos == m_vPushFinishPos)
	{//떨어질 위치는 아니고 도착했을때.
		m_bTopdeePush = false;
		if (CGameMgr::Get_Instance()->Check_PushBox_Exactly(vCurPos))
		{//밀리다가 떨어져야 할때.
			m_bDropBox = true;
		}
		return;
	}
}
#pragma endregion About_Topdee

void CInteraction_Block::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

