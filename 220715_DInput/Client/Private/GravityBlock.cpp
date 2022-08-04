#include "stdafx.h"
#include "..\Public\GravityBlock.h"
#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"
CGravityBlock::CGravityBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteraction_Block(pGraphic_Device)
{
}

CGravityBlock::CGravityBlock(const CGravityBlock & rhs)
	: CInteraction_Block(rhs)
{
}

HRESULT CGravityBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGravityBlock::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//======================
	m_Tag = L"Box";
	//======================


	if (m_pTransformCom != nullptr && pArg != nullptr)
	{
		_float3 vPos;
		vPos = ObjInfo.vPos;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	m_bOnBlock = false;
	return S_OK;
}

void CGravityBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (!m_bActive)
		return;
}

void CGravityBlock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (!m_bActive)
		return;
	//현재모드
	CGameMgr::GAMEMODE eCurMode = CGameMgr::Get_Instance()->GetMode();
	//현재모드와 이전모드를 비교해서 같냐
	if (eCurMode != m_ePreMode)
	{
		//모드가 바뀐시점
		if (eCurMode == CGameMgr::TOODEE)
		{
			//현재 바뀐모드가 투디면 중력
			m_bOnBlock = false;
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

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);

			//탑디면 땅에 꺼질수있는지 체크
			if (CGameMgr::Get_Instance()->Check_PushBox_Exactly(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
			{
				//떨어져야 할때.
				m_bDropBox = true;
			}
		}
		m_ePreMode = eCurMode;
	}

	if (eCurMode == CGameMgr::TOODEE)
	{
		//현재 투디모드고 능력이 활성화라면 중력활성화
		if (m_bAbility)
		{
			UpdateGravitiy(fTimeDelta);
		}
	}
	
	m_pCollCom->Add_CollisionGroup(CCollider::INTEREACTION, m_pBoxCollider, m_pTransformCom);
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
	if (m_bAbility)
	{
		//밀어내기
		if (other->CompareTag(L"Box") && CCollider::DIR_UP == eDirection && m_bOnBlock == false)
		{
			_float fBoxSize = 1.f;

			CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
			Safe_AddRef(TargetBox);

			_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + fBoxSize));

			m_bOnBlock = true;
			Safe_Release(TargetBox);
		}
	}
	
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
		vPos.z += -9.8f * fTimeDelta * .7f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
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
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_GravityBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pCollCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxDesc;
	BoxDesc.vPos = { 0.f,0.f,0.f };
	BoxDesc.vSize = { 1.f,1.f,1.f };
	BoxDesc.bIsTrigger = true;
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
}
