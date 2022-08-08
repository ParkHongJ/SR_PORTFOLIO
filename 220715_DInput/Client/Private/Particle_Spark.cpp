#include "stdafx.h"
#include "..\Public\Particle_Spark.h"
#include "GameInstance.h"

CParticle_Spark::CParticle_Spark(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CParticle_Spark::CParticle_Spark(const CParticle_Spark & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticle_Spark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Spark::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_Tag = L"Particle";
	return S_OK;
}

void CParticle_Spark::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	if (m_bBazier) {
		m_fTimer += fTimeDelta;
		_float3 vScale = m_pTransformCom->Get_Scaled();
		vScale.x = vScale.x - fTimeDelta * 3.f;
		vScale.y = vScale.y - fTimeDelta * 3.f;
		vScale.z = vScale.z - fTimeDelta * 3.f;

		m_pTransformCom->Set_Scaled(vScale);
	}
}

void CParticle_Spark::Make_Bazier(const _float3 & vStartPoint, const _float3 & vEndPoint)
{
	m_vStartPoint = vStartPoint;
	m_vEndPoint = vEndPoint;
	m_vElsePoint[0] = Point_Setting(m_vStartPoint);
	m_vElsePoint[1] = Point_Setting(m_vEndPoint);
	m_bBazier = true;
}

_float3 CParticle_Spark::Point_Setting(_float3 vOriginPos)
{
	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distr(0.f, 360.f);
	_float x = m_fPos[0] * cosf(D3DXToRadian(distr(eng)))+vOriginPos.x;
	_float y = m_fPos[1] * sinf(D3DXToRadian(distr(eng))) + vOriginPos.y;
	return _float3(x, y, vOriginPos.z);
}

void CParticle_Spark::LateTick(_float fTimeDelta)
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

HRESULT CParticle_Spark::Render()
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

HRESULT CParticle_Spark::Set_RenderState()
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

HRESULT CParticle_Spark::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CParticle_Spark::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Spark"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

CParticle_Spark * CParticle_Spark::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle_Spark* pInstance = new CParticle_Spark(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CParticle_Spark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle_Spark::Clone(void * pArg)
{
	CParticle_Spark* pInstance = new CParticle_Spark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CParticle_Spark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Spark::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}