#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinSizeX / 2;
	m_fY = g_iWinSizeY / 2;

	return S_OK;
}

void CBackGround::Tick(_float fTimeDelta)
{
	fSizeAddMgr(fTimeDelta);
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)g_iWinSizeX, (_float)g_iWinSizeY, 0, 1);

	m_fSizeX = _float(g_iWinSizeX + 1.f + m_fSizeAdd);
	m_fSizeY = _float(g_iWinSizeY + 1.f + m_fSizeAdd);
	if (m_fSizeX < g_iWinSizeX || m_fSizeY < g_iWinSizeY)
	{
		m_fSizeX = g_iWinSizeX;
		m_fSizeY = g_iWinSizeY;
	}
	
	m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

void CBackGround::LateTick(_float fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcUI;
	SetRect(&rcUI, _uint(m_fX - m_fSizeX * 0.5f), _uint(m_fY - m_fSizeY * 0.5f), _uint(m_fX + m_fSizeX * 0.5f), _uint(m_fY + m_fSizeY * 0.5f));

	/*if (PtInRect(&rcUI, ptMouse))
	{
		MSG_BOX(TEXT("충돌"));
	}
*/
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}
HRESULT CBackGround::Render()
{
	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);

	if (FAILED(m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Matrix);
	/*m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	*/
	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &Matrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->End();
	return S_OK;
}

void CBackGround::fSizeAddMgr(_float fTimeDelta)
{
	m_fSizeAddTimer += fTimeDelta;
	_int iTimer = (_int)m_fSizeAddTimer;    
	if (m_iPreTime != iTimer) {
		m_iPreTime = iTimer;
		m_fAddAdd *= -1.f;
		m_fSizeAdd += m_fAddAdd;
	}
	/*if (fTimeDelta > 0.5f) {
		m_fAddAdd *= -1.f;
	}
	else
		m_fAddAdd *= -1.f;*/
	/*if (fTimeDelta > 0.5f) {
		m_fAddAdd += -1.f;
	}
	else
		m_fAddAdd = 5.f;
	m_fSizeAdd += m_fAddAdd;*/
	
	
}

HRESULT CBackGround::SetUp_Components()
{
	/* 1. 내일 이거 수정해라. */
	/* 2. 월드행렬을ㅇ 관리하는 CTransform 제ㅈ닥. */
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Default"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*		pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void* pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	int i = 10;
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

