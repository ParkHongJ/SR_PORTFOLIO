#include "stdafx.h"
#include "..\Public\BreakingBlock.h"

#include "GameInstance.h"
#include "ParticleMgr.h"
#include "GameMgr.h"
#include "Hong.h"

CBreakingBlock::CBreakingBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_fOnBlock(0.f)
	, m_fRemember(0.f)
	, m_bCheck(false)
{
}

CBreakingBlock::CBreakingBlock(const CBreakingBlock & rhs)
	: CGameObject(rhs)
	, m_fOnBlock(0.f)
	, m_fRemember(0.f)
	, m_bCheck(false)
{
}

HRESULT CBreakingBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBreakingBlock::Initialize(void * pArg)
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


	if (pArg != nullptr)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	else
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	return S_OK;
}

void CBreakingBlock::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	CGameMgr::GAMEMODE eCurMode = CGameMgr::Get_Instance()->GetMode();

	if (m_bCheck)
	{
		m_fOnBlock += fTimeDelta;
		m_fRemember = m_fOnBlock;

		if (eCurMode == CGameMgr::TOPDEE)
		{
			m_fOnBlock = 0;
		}
		else
		{
			if (3.f <= m_fRemember)
			{
				m_bActive = false;
				int a = 10;
				for (int i = 0; i < 7; i++)
				{
					random_device rd;
					default_random_engine eng(rd());
					uniform_real_distribution<float> distr(-.8f, .8f);
					//random float

					_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					vPos.x += distr(eng);
					vPos.z += distr(eng);
					CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
						vPos,
						vPos - vPos2,
						CParticleMgr::PARTICLE);
				}
			}
		}
	}
}

void CBreakingBlock::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, m_pBoxCollider, m_pTransformCom);
}

HRESULT CBreakingBlock::Render()
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

void CBreakingBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (!m_bActive)
		return;

	if (other->CompareTag(L"Toodee") || other->CompareTag(L"Pig") || other->CompareTag(L"Tookee") || other->CompareTag(L"Box"))
	{
		if (CCollider::DIR_UP == eDirection) 
		{
			m_bCheck = true;

			// 블럭 상하좌우 살펴서 같은 블럭 찾기
		}
		//m_bCheck = true;
	}
}

HRESULT CBreakingBlock::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CBreakingBlock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBreakingBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_BreakingBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

CBreakingBlock * CBreakingBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBreakingBlock*		pInstance = new CBreakingBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBreakingBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBreakingBlock::Clone(void* pArg)
{
	CBreakingBlock*		pInstance = new CBreakingBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CBreakingBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBreakingBlock::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

