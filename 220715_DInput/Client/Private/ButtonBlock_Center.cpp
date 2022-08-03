#include "stdafx.h"
#include "..\Public\ButtonBlock_Center.h"

#include "GameInstance.h"
CButtonBlock_Center::CButtonBlock_Center(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CButtonBlock_Center::CButtonBlock_Center(const CButtonBlock_Center& rhs)
	: CGameObject(rhs)
{

}

HRESULT CButtonBlock_Center::Initialize_Prototype()
{
	return S_OK;

}

HRESULT CButtonBlock_Center::Initialize(void* pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//======================
	SetTag(L"Box");
	//======================
	if (pArg != nullptr)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 1.f, 3.f));
	return S_OK;

}

void CButtonBlock_Center::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

void CButtonBlock_Center::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, m_pBoxCollider, m_pTransformCom);
}

HRESULT CButtonBlock_Center::Render()
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

HRESULT CButtonBlock_Center::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_KeyBox"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

HRESULT CButtonBlock_Center::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CButtonBlock_Center::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

void CButtonBlock_Center::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

CGameObject* CButtonBlock_Center::Clone(void* pArg)
{
	CButtonBlock_Center*		pInstance = new CButtonBlock_Center(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CButtonBlock_Center"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CButtonBlock_Center* CButtonBlock_Center::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButtonBlock_Center*		pInstance = new CButtonBlock_Center(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CButtonBlock_Center"));
		Safe_Release(pInstance);
	}

	return pInstance;
}