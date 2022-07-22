#include "stdafx.h"
#include "..\Public\Monster_Pig.h"

#include "GameInstance.h"

CMonster_Pig::CMonster_Pig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMonster_Pig::CMonster_Pig(const CMonster_Pig & rhs)
	: CGameObject(rhs)
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 0.5f, 0.0f));

	return S_OK;
}

void CMonster_Pig::Tick(_float fTimeDelta)
{
	m_vTargetPos = _float3(1.f, 50.f, 0.f);

	// m_pTransformCom->Chase(m_vTargetPos, fTimeDelta);
	//m_pTransformCom->LookAtForLandObject(m_vTargetPos);
	
}

void CMonster_Pig::LateTick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMonster_Pig::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	
	
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	_float3 temp = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_uint Sour = 1; // 객체에 넘버링 할 수 있음.

	char Sour1[256] = { "Monster1" };

	ImGui::Begin("Inspector");

	ImGui::Text("Monster%d", Sour);

	ImGui::SliderFloat(Sour1, &temp.x, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat(Sour1, &temp.y, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat(Sour1, &temp.z, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);

	ImGui::Spacing();

	ImGui::End();

	return S_OK;
}

HRESULT CMonster_Pig::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_Pig"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
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

void CMonster_Pig::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

