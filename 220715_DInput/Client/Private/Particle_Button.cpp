#include "stdafx.h"
#include "..\Public\Particle_Button.h"


#include "GameInstance.h"
#include "Hong.h"

CParticle_Button::CParticle_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_bCheck(false)
{
}

CParticle_Button::CParticle_Button(const CParticle_Button & rhs)
	: CGameObject(rhs)
	, m_bCheck(false)
{
}

HRESULT CParticle_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Button::Initialize(void * pArg)
{
	if (pArg != nullptr)
	{
		memcpy(&m_iNumLevel, pArg, sizeof(_uint));
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_Tag = L"Particle";

	return S_OK;
}

void CParticle_Button::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	
	m_fTimer += fTimeDelta;
	if (m_fTimer > 1.6f)
	{
		m_fTimer = 0.f;
		return;
	}
	
	if (0.8f < m_fTimer)
		m_pTransformCom->Translate(m_vDir * fTimeDelta * 0.2f);
	else
		m_pTransformCom->Translate(m_vDir * fTimeDelta * -0.2f);

	if (!m_bCheck)
	{
		//Scale Random
		_float3 vScale = m_pTransformCom->Get_Scaled();

		random_device random;
		mt19937 rd(random());
		uniform_real_distribution<float> range(.2f, .8f);

		vScale = vScale * range(rd);
		m_pTransformCom->Set_Scaled(vScale);
		
		//Render Random
		default_random_engine eng(random());
		uniform_int_distribution<int> distr(0, 3);

		m_iRand = distr(eng);
		m_bCheck = true;
	}
}

void CParticle_Button::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3 vScale = m_pTransformCom->Get_Scaled();
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pTransformCom->Set_Scaled(vScale);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CParticle_Button::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iRand)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Button::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CParticle_Button::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CParticle_Button::SetUp_Components()
{/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 0.5f,0.5f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &RectDesc)))
		return E_FAIL;

	//이거 수정해라
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

CParticle_Button * CParticle_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle_Button* pInstance = new CParticle_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CParticle_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle_Button::Clone(void * pArg)
{
	CParticle_Button* pInstance = new CParticle_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CParticle_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Button::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

void CParticle_Button::SetDirection(const _float3& vDir)
{
	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}
