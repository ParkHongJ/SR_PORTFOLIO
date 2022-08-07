#include "stdafx.h"
#include "..\Public\Tookee.h"

#include "ParticleMgr.h"
#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"
CTookee::CTookee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{
}

CTookee::CTookee(const CTookee & rhs)
	: CLandObject(rhs)
{
}
HRESULT CTookee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTookee::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/* For.Toodee_Col */
	SetTag(L"Tookee");

	_float3 vPos = ObjInfo.vPos;
	vPos.z += 3.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(2.f, 0.3f, 12.f));
	m_Toodee = CGameMgr::Get_Instance()->TempTrasnform();

	return S_OK;
}

void CTookee::Tick(_float fTimeDelta)
{
}

void CTookee::LateTick(_float fTimeDelta)
{
	
	/*_float3 vPos = *(_float3*)&test.m[3][0];
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);*/
	

	//_float4x4 toodeeLocal = m_Toodee->Get_WorldMatrix();
	//_float4x4 MyLocal = m_pTransformCom->Get_WorldMatrixInverse();
	//_float4x4 MyWorld = m_pTransformCom->Get_WorldMatrix();


	//_float4x4 test = MyLocal * toodeeLocal;// *MyWorld;

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&test.m[0][0]);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&test.m[1][0]);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&test.m[2][0]);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&test.m[3][0]);
	////m_pTransformCom->Set_WorldMatrix(test);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CTookee::Render()
{
	
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_bActive) {
		if (FAILED(m_pTextureCom->Bind_Texture(0)))
			return E_FAIL;
	}

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

void CTookee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CTookee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDireciton)
{
}

void CTookee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

HRESULT CTookee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CTookee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CTookee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Toodee_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Tookee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.5f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 1.f);
	BoxColliderDesc.fRadius = 1.f;

	BoxColliderDesc.bIsTrigger = true;

	/* For.Com_BoxCollider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CTookee * CTookee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTookee* pInstance = new CTookee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTookee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTookee::Clone(void * pArg)
{
	CTookee* pInstance = new CTookee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CTookee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTookee::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
