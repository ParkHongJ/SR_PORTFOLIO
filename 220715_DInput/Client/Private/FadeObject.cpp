#include "stdafx.h"
#include "..\Public\FadeObject.h"
#include "GameInstance.h"

CFadeObject::CFadeObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CFadeObject::CFadeObject(const CFadeObject & rhs)
	: CGameObject(rhs)
{
}
HRESULT CFadeObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFadeObject::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0, 1);

	m_fSizeX = 1280.f;
	m_fSizeY = 760.f;

	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CFadeObject::Tick(_float fTimeDelta)
{
}

void CFadeObject::LateTick(_float fTimeDelta)
{
	if (fRadian > 0.f)
	{
		fRadian -= fTimeDelta * 0.3f;
	}
	else
	{
		int a = 10;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CFadeObject::Render()
{
	//수정필요
	m_pShaderCom->Set_RawValue("g_fOffset", &m_fOffset, sizeof(_float2));

	m_pShaderCom->Set_RawValue("g_fRadian", &fRadian, sizeof(_float));
	_float4x4			WorldMatrix, Matrix;
	_float4x4			ViewMatrixInv;
	D3DXMatrixIdentity(&Matrix);

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	m_pShaderCom->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&Matrix, &Matrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&m_ProjMatrix, &m_ProjMatrix), sizeof(_float4x4));

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	m_pShaderCom->End();
	return S_OK;
}

void CFadeObject::SetOffset(_float2 _fOffset)
{

	m_fOffset = _fOffset;
	m_iPass = 4;
	//3번이 기능없는쉐이더
	//4번이 줌인

}

HRESULT CFadeObject::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Terrain"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;
	return S_OK;
}

CFadeObject * CFadeObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFadeObject*		pInstance = new CFadeObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CFadeObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFadeObject::Clone(void * pArg)
{
	CFadeObject*		pInstance = new CFadeObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CFadeObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFadeObject::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
