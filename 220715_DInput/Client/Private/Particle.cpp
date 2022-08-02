#include "stdafx.h"
#include "..\Public\Particle.h"


#include "GameInstance.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CParticle::CParticle(const CParticle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(5.f, 0.5f, 2.f));
	m_Tag = L"Particle";
	return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	_float3 vScale = m_pTransformCom->Get_Scaled();
	m_fTimer += fTimeDelta;
	if (m_fTimer > 1.5f)
	{
		m_bActive = false;
		m_fTimer = 0.f;
		return;
	}
	vScale.x = vScale.x - fTimeDelta * 3.f;
	vScale.y = vScale.y - fTimeDelta * 3.f;
	vScale.z = vScale.z - fTimeDelta * 3.f;
	
	m_pTransformCom->Set_Scaled(vScale);

	m_pTransformCom->Translate(m_vDir * fTimeDelta);
}

void CParticle::LateTick(_float fTimeDelta)
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

HRESULT CParticle::Render()
{
	if (!m_bActive)
		return S_OK;

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

HRESULT CParticle::Set_RenderState()
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

HRESULT CParticle::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CParticle::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle* pInstance = new CParticle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CParticle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle::Clone(void * pArg)
{
	CParticle* pInstance = new CParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CParticle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

void CParticle::SetDirection(const _float3& vDir)
{
	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}
