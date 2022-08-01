#include "stdafx.h"
#include "..\Public\Monster_Pig.h"

#include "GameInstance.h"
#include "GameMgr.h"

CMonster_Pig::CMonster_Pig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{
}

CMonster_Pig::CMonster_Pig(const CMonster_Pig & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Pig::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Pig::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_Tag = L"Pig";
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 0.5f, 10.f));

	return S_OK;
}

void CMonster_Pig::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	m_fFrame += 9.0f * fTimeDelta;

	if (m_fFrame >= 9.0f)
		m_fFrame = 0.f;

	/*if ( TOODEE )
	중력 작용 / 블럭 위에서만 돌아다님*/
	CGameMgr* pGameMgr = CGameMgr::Get_Instance();
	Safe_AddRef(pGameMgr);
	if (pGameMgr->GetMode() == CGameMgr::TOODEE)
	{
		/*_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.z += -9.8f * fTimeDelta * 0.5f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);*/
	}
	else
	{
		//if ( TOPDEE )
		m_vTopdeePos = __super::SetUp_Topdee(m_pTransformCom, LEVEL_GYUH, L"Layer_topdee", 0, L"Com_Transform");
		m_pTransformCom->Chase(m_vTopdeePos, 0.5 * fTimeDelta);
	}
	Safe_Release(pGameMgr);
}

void CMonster_Pig::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pColliderCom->Add_CollisionGroup(CCollider::MONSTER, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	
}

HRESULT CMonster_Pig::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	
	
	if (FAILED(m_pTextureCom->Bind_Texture((_uint)m_fFrame)))
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

HRESULT CMonster_Pig::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CMonster_Pig::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CMonster_Pig::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_SENI, TEXT("Prototype_Component_Texture_Monster_Pig"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
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
	BoxColliderDesc.vSize = _float3(.5f, .5f, .5f);
	BoxColliderDesc.bIsTrigger = false;
	BoxColliderDesc.fRadius = 1.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CMonster_Pig * CMonster_Pig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster_Pig*		pInstance = new CMonster_Pig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMonster_Pig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Pig::Clone(void* pArg)
{
	CMonster_Pig*		pInstance = new CMonster_Pig(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CMonster_Pig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Pig::OnTriggerExit(CGameObject* other, _float fTimeDelta, _uint eDirection)
{
	int a = 10;
}

void CMonster_Pig::OnTriggerEnter(CGameObject* other, _float fTimeDelta, _uint eDirection)
{
	int a = 10;
}

void CMonster_Pig::OnTriggerStay(CGameObject* other, _float fTimeDelta, _uint eDirection)
{
	int a = 10;
}


void CMonster_Pig::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
