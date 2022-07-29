#include "stdafx.h"
#include "..\Public\Loading_BackGround.h"

#include "GameInstance.h"

CLoading_BackGround::CLoading_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLoading_BackGround::CLoading_BackGround(const CLoading_BackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoading_BackGround::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CLoading_BackGround::Initialize(void * pArg)
{
	if (g_iLoadingTexture == 0)
		g_iLoadingTexture = 1;
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0, 1);

	
	m_fX = g_iWinSizeX / 2;
	m_fY = g_iWinSizeY / 2;
	m_pTransform_BackGroundCom->Set_Scaled(_float3(g_iWinSizeX, g_iWinSizeY, 0.f));
	m_pTransform_BackGroundCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 1.f));

	m_fSizeX = 238.f;
	m_fSizeY = 238.f;
	m_pTransform_CloudCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransform_CloudCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.4f));

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_pTransform_InsideCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransform_InsideCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.5f));

	return S_OK;
}

void CLoading_BackGround::Tick(_float fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;
	if (m_fTimeDelta > 0.5f) {
		if (m_iCloudFrame >= 5)
			m_iCloudFrame = 0;
		++m_iCloudFrame;
		m_fTimeDelta = 0.f;
	}
}

void CLoading_BackGround::LateTick(_float fTimeDelta)
{
	m_pRenderer_BackGroundCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	m_pRenderer_CloudCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	m_pRenderer_InsideCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLoading_BackGround::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//이거 탑디랑 같이돌리는중
	return S_OK;
}

HRESULT CLoading_BackGround::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CLoading_BackGround::Render()
{
	_float4x4		Matrix;
	
	D3DXMatrixIdentity(&Matrix);

	if (FAILED(m_pTexture_BackGroundCom->Bind_Texture(0)))
		return E_FAIL;
	m_pTransform_BackGroundCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pVIBuffer_BackGroundCom->Render();


	D3DXMatrixIdentity(&Matrix);
	if (FAILED(m_pTexture_InsideCom->Bind_Texture(g_iLoadingTexture)))
		return E_FAIL;
	m_pTransform_InsideCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	m_pVIBuffer_InsideCom->Render();

	D3DXMatrixIdentity(&Matrix);
	if (FAILED(m_pTexture_CloudCom->Bind_Texture(m_iCloudFrame)))
		return E_FAIL;
	m_pTransform_CloudCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(Set_RenderState()))
		return E_FAIL;
	m_pVIBuffer_CloudCom->Render();
	if (FAILED(Reset_RenderState()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoading_BackGround::SetUp_Components()
{
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	//=======================================================================
	/* For.Com_Renderer_Loading_BackGround */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer_Loading_BackGround"), (CComponent**)&m_pRenderer_BackGroundCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Loading_BackGround */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Loading_BackGround"), (CComponent**)&m_pVIBuffer_BackGroundCom, this)))
		return E_FAIL;

	/* For.Com_Texture_Loading_BackGround */
	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_BackGround"), 
		TEXT("Com_Texture_Loading_BackGround"), (CComponent**)&m_pTexture_BackGroundCom, this)))
		return E_FAIL;
	
	/* For.Com_Transform_Loading_BackGround */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), 
		TEXT("Com_Transform_Loading_BackGround"), (CComponent**)&m_pTransform_BackGroundCom, this, &TransformDesc)))
		return E_FAIL;

	//=======================================================================
	/* For.Com_Renderer_Loading_Cloud */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer_Loading_Cloud"), (CComponent**)&m_pRenderer_CloudCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Loading_Cloud */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), 
		TEXT("Com_VIBuffer_Loading_Cloud"), (CComponent**)&m_pVIBuffer_CloudCom, this)))
		return E_FAIL;

	/* For.Com_Texture_Loading_Cloud */
	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_Cloud"), 
		TEXT("Com_Texture_Loading_Cloud"), (CComponent**)&m_pTexture_CloudCom, this)))
		return E_FAIL;
	
	/* For.Com_Transform_Loading_Cloud */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), 
		TEXT("Com_Transform_Loading_Cloud"), (CComponent**)&m_pTransform_CloudCom, this, &TransformDesc)))
		return E_FAIL;

	//=======================================================================
	/* For.Com_Renderer_Loading_Inside */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer_Loading_Inside"), (CComponent**)&m_pRenderer_InsideCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Loading_Inside */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), 
		TEXT("Com_VIBuffer_Loading_Inside"), (CComponent**)&m_pVIBuffer_InsideCom, this)))
		return E_FAIL;

	/* For.Com_Texture_Loading_Inside */
	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_Inside"), 
		TEXT("Com_Texture_Loading_Inside"), (CComponent**)&m_pTexture_InsideCom, this)))
		return E_FAIL;

	/* For.Com_Transform_Loading_Inside */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), 
		TEXT("Com_Transform_Loading_Inside"), (CComponent**)&m_pTransform_InsideCom, this, &TransformDesc)))
		return E_FAIL;
	//=======================================================================
	return S_OK;
}

CLoading_BackGround * CLoading_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_BackGround*		pInstance = new CLoading_BackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CLoading_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_BackGround::Clone(void* pArg)
{
	CLoading_BackGround*		pInstance = new CLoading_BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CLoading_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_BackGround::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_BackGroundCom);
	Safe_Release(m_pRenderer_BackGroundCom);
	Safe_Release(m_pTransform_BackGroundCom);
	Safe_Release(m_pVIBuffer_BackGroundCom);

	Safe_Release(m_pTexture_CloudCom);
	Safe_Release(m_pRenderer_CloudCom);
	Safe_Release(m_pTransform_CloudCom);
	Safe_Release(m_pVIBuffer_CloudCom);

	Safe_Release(m_pTexture_InsideCom);
	Safe_Release(m_pRenderer_InsideCom);
	Safe_Release(m_pTransform_InsideCom);
	Safe_Release(m_pVIBuffer_InsideCom);
}

