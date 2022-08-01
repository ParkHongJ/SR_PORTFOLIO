#include "stdafx.h"
#include "..\Public\Spike.h"

#include "GameInstance.h"
#include "GameMgr.h"

CSpike::CSpike(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSpike::CSpike(const CSpike & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSpike::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpike::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_Tag = L"Spike";
	
	if (pArg != nullptr)
	{
		
		_float3 vSetPos{ *(_float3*)pArg };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vSetPos);
	}

	return S_OK;
}

void CSpike::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (0.11f < m_fTime) {
		if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE) {
			if (0 < m_iFrame)
				--m_iFrame;
		}
		else if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE) {
			if (5 > m_iFrame)
				++m_iFrame;
		}
		m_fTime = 0.f;
	}
}

void CSpike::LateTick(_float fTimeDelta)
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);
	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 0.5f));
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	/* For.Spike_Col */
	m_pColliderCom->Add_CollisionGroup(CCollider::OBJECT, m_pBoxCom, m_pTransformCom);
}

HRESULT CSpike::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------

	return S_OK;
}

void CSpike::OnTriggerExit(CGameObject * other)
{
}

void CSpike::OnTriggerEnter(CGameObject * other)
{
}

void CSpike::OnTriggerStay(CGameObject * other)
{
}

HRESULT CSpike::Set_RenderState()
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

HRESULT CSpike::Reset_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

/* 규현이의 피와 땀이 담긴 작업물이라 안지웠음 */
//void CSpike::SpikeAngleMgr()
//{//카메라는 0~7.9
//	//0일때는 탑디모드 즉 프레임이5번
//	//7.9일때는 투디모드 즉 프레임이 0번
//	_float4x4		ViewMatrix;
//
//	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
//
//	_float4x4		CamWorldMatrix;
//	D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
//	_float fCameraZ = (*(_float3*)&CamWorldMatrix.m[3][0]).z;
//	_int iCameraZpos{ (_int)fCameraZ };
//	if ((iCameraZpos == -1) || (iCameraZpos == 0)) {
//		m_iFrame = 5;
//	}
//	else if((iCameraZpos == 1) || (iCameraZpos == 2)){
//		m_iFrame = 4;
//	}
//	else if ((iCameraZpos ==3) || (iCameraZpos == 4)) {
//		m_iFrame = 3;		
//	}						
//	else if (iCameraZpos ==5)  {
//		m_iFrame = 2;
//	}
//	else if (iCameraZpos == 6)  {
//		m_iFrame = 1;
//	}
//	else if ((iCameraZpos == 7) || (iCameraZpos == 8)) {
//		m_iFrame = 0;
//	}
//}

HRESULT CSpike::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 2.f,2.f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this,&RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Spike"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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
	BoxColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	BoxColliderDesc.bIsTrigger = true;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;
	return S_OK;
}

CSpike * CSpike::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpike* pInstance = new CSpike(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSpike"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpike::Clone(void * pArg)
{
	CSpike* pInstance = new CSpike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CSpike"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpike::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
