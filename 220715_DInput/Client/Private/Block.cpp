#include "stdafx.h"
#include "..\Public\Block.h"

#include "GameInstance.h"
#include "Hong.h"
#include "ParticleMgr.h"

CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteraction_Block(pGraphic_Device)
{
}

CBlock::CBlock(const CBlock & rhs)
	: CInteraction_Block(rhs)
{
}

HRESULT CBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlock::Initialize(void * pArg)
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
	return S_OK;
}

void CBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
}

void CBlock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (!m_bActive)
		return;
	if (m_bRayCasted)
	{
		for (int i = 0; i < 10; i++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<float> distr(-1.5f, 1.5f);
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vPos2 = vPos;
			vPos.x += distr(eng);
			vPos.z += distr(eng);
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);
		}
		m_bActive = false;
	}


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::INTEREACTION, m_pBoxCollider, m_pTransformCom);
}

HRESULT CBlock::Render()
{
	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture", m_iTextureNum);


	m_pShaderCom->Begin(m_eShaderSelect);//0 default, 1 WarpOn

	m_pVIBufferCom->Render();

	m_pShaderCom->End();
	
	return S_OK;
}

void CBlock::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	int a = 10;
}

void CBlock::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
	int a = 10;
}

void CBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta)
{
	int a = 10;
}

HRESULT CBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */ 
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;

	return S_OK;
}

CBlock * CBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock*		pInstance = new CBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlock::Clone(void* pArg)
{
	CBlock*		pInstance = new CBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock::Free()
{
	__super::Free();
	
}

