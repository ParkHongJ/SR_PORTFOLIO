#include "stdafx.h"
#include "..\Public\GravityBlock.h"
#include "GameInstance.h"
#include "GameMgr.h"
CGravityBlock::CGravityBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CGravityBlock::CGravityBlock(const CGravityBlock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGravityBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGravityBlock::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_Tag = L"Box";

	if (pArg != nullptr)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.5f, .5f, 14.f));
	
	m_bOnBlock = false;
	m_bOnAir = true;
	return S_OK;
}

void CGravityBlock::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

}

void CGravityBlock::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE)
	{
		UpdateGravitiy(fTimeDelta);
	}
	else
	{
		//현재위치
		_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vFinalPosition; //도착위치

		//z값만 보정 (중력이니까)
		vFinalPosition.x = vCurPosition.x;
		vFinalPosition.y = vCurPosition.y;
		vFinalPosition.z = _int(vCurPosition.z) + 0.5f;

		//러프시작
		vCurPosition = Lerp(vCurPosition, vFinalPosition, fTimeDelta * 5);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
	}
	
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, m_pBoxCollider, m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CGravityBlock::Render()
{
	if (!m_bActive)
		S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CGravityBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	//밀어내기
	_float fBoxSize = 1.f;

	/*if (CCollider::DIR_DOWN == eDirection) {
		if (TargetBox->Get_State(CTransform::STATE_POSITION).x - (fBoxSize * 0.45f) < m_pTransformCom->Get_State(CTransform::STATE_POSITION).x
			&& TargetBox->Get_State(CTransform::STATE_POSITION).x + (fBoxSize * 0.45f) > m_pTransformCom->Get_State(CTransform::STATE_POSITION).x)
			m_bOnBlock = true;
	}*/
	m_bOnBlock = true;
}

void CGravityBlock::UpdateGravitiy(_float fTimeDelta)
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_bOnBlock)
	{
		m_bOnAir = false;
	}
	else
	{
		m_bOnAir = true;
	}

	if (m_bOnAir)
	{
		vPos.z += -9.8f * fTimeDelta * 1.5f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

_bool CGravityBlock::KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta, _float3 vPreLoaderPos)
{
	return _bool();
}

void CGravityBlock::KKK_Is_Raise(_float3 vTargetPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);
}

_bool CGravityBlock::KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta, _bool bHoleCall)
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (!bHoleCall) {
		vCurPosition = vCurPosition + (vFinalPos - vCurPosition) * (fTimeDelta * 5);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);

		if (vCurPosition.y <= 0.55f)
			return true;
		return false;
	}
	else
		m_bDropBox = true;
	return false;
}

void CGravityBlock::Box_Drop_More(_float fTimeDelta)
{
}

void CGravityBlock::Box_Push_More(_float fTimeDelta, _float3 vPushFinishPos, _float3 vPushDir)
{
}

void CGravityBlock::Box_Push_Find_A_Place()
{
}

HRESULT CGravityBlock::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CGravityBlock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CGravityBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_GravityBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pCollCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxDesc;
	BoxDesc.vPos = { 0.f,0.f,0.f };
	BoxDesc.vSize = { 1.f,1.f,1.f };
	BoxDesc.bIsTrigger = false;
	BoxDesc.fRadius = 0.5f;
	/* For.Com_BoxCollider*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCollider, this, &BoxDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CGravityBlock * CGravityBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGravityBlock*		pInstance = new CGravityBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGravityBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGravityBlock::Clone(void * pArg)
{
	CGravityBlock*		pInstance = new CGravityBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CGravityBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGravityBlock::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
