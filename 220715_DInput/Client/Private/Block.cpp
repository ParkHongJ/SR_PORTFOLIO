#include "stdafx.h"
#include "..\Public\Block.h"

#include "GameInstance.h"
CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBlock::CBlock(const CBlock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlock::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_float3* vTemp = (_float3*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vTemp);
	}
	return S_OK;
}

void CBlock::Tick(_float fTimeDelta)
{
	/*if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Translate(_float3(0.f, 0.f, 1.f) * fTimeDelta * 3.f);
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Translate(_float3(0.f, 0.f, -1.f) * fTimeDelta * 3.f);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Translate(_float3(-1.f, 0.f, 0.f) * fTimeDelta * 3.f);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Translate(_float3(1.f, 0.f, 0.f) * fTimeDelta * 3.f);
	}*/
}

void CBlock::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, this);
}

HRESULT CBlock::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	/*if (FAILED(m_pTextureCom->Bind_Texture(2)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(1)))
		return E_FAIL;*/

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	//m_pColliderCom->Render();
	//--------------------------------------------------------------

	
	return S_OK;
}

HRESULT CBlock::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CBlock::Reset_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_HONG, TEXT("Prototype_Component_Texture_ElectricBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;
	/* For.Com_Coll */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Coll"), (CComponent**)&m_pCollCom, this)))
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
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

