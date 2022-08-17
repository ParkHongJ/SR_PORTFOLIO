#include "stdafx.h"
#include "..\Public\FadeObject.h"
#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"

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
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
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
	if (CGameMgr::Get_Instance()->Key_Down(DIK_Y))
	{
		m_iPass = 4;
	}
	if (m_iPass != 3)
	{
		if (fRadian > 0.f)
		{
			fRadian -= fTimeDelta * 0.8f;
		}
		else
		{
			int a = 10;
		}
	}
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CFadeObject::Render()
{
	//수정필요
	/*m_fOffset = _float2(0.5f, 0.5f);*/
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

void CFadeObject::SetOffset(_float3 _fOffset)
{
	////인자로 들어오는값은 World포지션
	//_float4x4 matView, matProj;
	//D3DXMatrixIdentity(&matView);

	//D3DXMatrixIdentity(&matProj);
	////
	////D3DXMatrixIdentity(&matWorld);
	////memcpy(&matWorld.m[3][0], &_fOffset, sizeof(_float3));

	//_float3 vPos = _fOffset;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	////D3DXMatrixInverse(&matView, nullptr, &matView);
	//m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	////D3DXMatrixInverse(&matProj, nullptr, &matProj);

	//int a = 10;
	//D3DXVec3TransformCoord(&vPos, &vPos, &matView);
	//D3DXVec3TransformCoord(&vPos, &vPos, &matProj);

	//m_pGraphic_Device->GetTransform();
	m_fOffset = { 0.5f, 0.5f };
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
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
