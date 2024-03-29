#include "stdafx.h"
#include "..\Public\Hole.h"

#include "GameInstance.h"
#include "Hong.h"

CHole::CHole(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CHole::CHole(const CHole & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHole::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHole::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float3 vPos;
	vPos = ObjInfo.vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	if (pArg != nullptr)
	{
		_float3* vInitPos= (_float3*)pArg;
		vInitPos->y -= 0.49f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vInitPos);
	}
	return S_OK;
}

void CHole::Tick(_float fTimeDelta)
{

}

void CHole::LateTick(_float fTimeDelta)
{	
	if (m_bActive) {
		_float4x4	ViewMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
}

HRESULT CHole::Render()
{
	if (m_bActive) {
		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_Texture(m_iFrameNum)))
			return E_FAIL;

		if (FAILED(Set_RenderState()))
			return E_FAIL;

		m_pVIBufferCom->Render();

		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}
	
	return S_OK;
}

void CHole::OnTriggerExit(CGameObject * other, float fTimeDelta)
{
}

void CHole::OnTriggerEnter(CGameObject * other, float fTimeDelta)
{

}

void CHole::OnTriggerStay(CGameObject * other, float fTimeDelta, _uint eDirection)
{
	
}


HRESULT CHole::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CHole::Reset_RenderState()
{
	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);*/

	return S_OK;
}

HRESULT CHole::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Hole"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this)))
		return E_FAIL;
	return S_OK;

}

CHole * CHole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHole*		pInstance = new CHole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHole::Clone(void* pArg)
{
	CHole*		pInstance = new CHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CHole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHole::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pBoxCom);
}

