#include "stdafx.h"
#include "..\Public\Wave.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"

CWave::CWave(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CWave::CWave(const CWave & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWave::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWave::Initialize(void * pArg)
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
	m_Tag = L"Wave";
	//======================


	if (m_pTransformCom != nullptr && pArg != nullptr)
	{
		_float3 vPos;
		vPos = _float3(-15.f, 5.2f, 9.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pTransformCom->Rotation(_float3(1.0f, 0.f, 0.f), D3DXToRadian(90.f));
	}
	else
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	return S_OK;
}

void CWave::Tick(_float fTimeDelta)
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos.x += fTimeDelta * .5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CWave::LateTick(_float fTimeDelta)
{
	m_pColliderCom->Add_CollisionGroup(CCollider::OBJECT, m_pBoxCom, m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CWave::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;

	//월드던질때 전치
	_float4x4 matWorld;
	matWorld = m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixTranspose(&matWorld, &matWorld);

	_float4x4 matView;
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView)))
		MSG_BOX(L"실패");
	D3DXMatrixTranspose(&matView, &matView);

	_float4x4 matProj;
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj)))
		MSG_BOX(L"씰퍠");
	D3DXMatrixTranspose(&matProj, &matProj);

	m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_float4x4));

	m_pShaderCom->Begin(2);

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	m_pShaderCom->End();
	return S_OK;
}

void CWave::OnTriggerExit(CGameObject * other, float fTimeDelta)
{
}

void CWave::OnTriggerEnter(CGameObject * other, float fTimeDelta)
{
}

void CWave::OnTriggerStay(CGameObject * other, float fTimeDelta, _uint eDireciton)
{
}

HRESULT CWave::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CWave::Reset_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CWave::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 30.f,20.f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Wave"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(10.f, 1.f, 10.f);
	BoxColliderDesc.bIsTrigger = true;
	BoxColliderDesc.fRadius = 12.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;

	return S_OK;
}

CWave * CWave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWave* pInstance = new CWave(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWave"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWave::Clone(void * pArg)
{
	CWave* pInstance = new CWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CWave"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWave::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
