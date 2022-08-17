#include "stdafx.h"
#include "..\Public\Leaf.h"
#include "Hong.h"

#include "GameInstance.h"

CLeaf::CLeaf(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLeaf::CLeaf(const CLeaf & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLeaf::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeaf::Initialize(void * pArg)
{
	if (pArg != nullptr)
	{
		CHong::ObjInfo ObjInfo;
		memcpy(&ObjInfo, pArg, sizeof(CHong::ObjInfo));
		m_iNumLevel = ObjInfo.iNumLevel;

	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.f, 1.5f, 10.f));
	m_Tag = L"Leaf";
	SetDirection();
	return S_OK;
}

void CLeaf::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	//x -5.5~35.5f
	//z  -0.5~20.5
	_float3 vLeafPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
	if (vLeafPos.x < -5.5f || vLeafPos.z < -0.5f)
		SetDirection();
	m_fAngle += 1.f;
	if (m_fAngle > 360.f)
		m_fAngle = 0.f;
	
	m_pTransformCom->Translate(m_vDir * fTimeDelta * m_fSpeed);
}

void CLeaf::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(m_fAngle));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CLeaf::Render()
{
	
	if (!m_bActive)
		return S_OK;
	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;

	WorldMatrix = m_pTransformCom ->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom ->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom ->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom ->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pTextureCom ->Bind_Texture(m_pShaderCom , "g_Texture", m_iTexture);


	m_pShaderCom ->Begin(0);

	m_pVIBufferCom ->Render();

	m_pShaderCom ->End();
	

	return S_OK;
}

HRESULT CLeaf::Set_RenderState()
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



HRESULT CLeaf::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	return S_OK;
}

HRESULT CLeaf::SetUp_Components()
{/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_LayDown::RECTDESC RectDesc;
	RectDesc.vSize = { 0.5f,0.5f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_LayDown"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Leaf"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Leaf"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;
	
	return S_OK;
}

CLeaf * CLeaf::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLeaf* pInstance = new CLeaf(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CLeaf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLeaf::Clone(void * pArg)
{
	CLeaf* pInstance = new CLeaf(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CLeaf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeaf::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

}

void CLeaf::SetDirection()
{
	//x -5.5~35.5f
	//z  -0.5~20.5
	random_device rdZ,rdX,rdDir,rdSpeed, rdTexture;

	default_random_engine engZ(rdZ());
	uniform_real_distribution<float> distZ(10.5f, 20.5f); //시작포인트
	default_random_engine engX(rdX());					  //시작포인트
	uniform_real_distribution<float> distX(25.5f, 35.5f); //시작포인트


	default_random_engine engDir(rdDir());					  //방향x랑z
	uniform_real_distribution<float> distDir(-1.0f, -0.1f);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(distX(engX), 1.5f, distZ(engZ)));
	
	m_vDir = { distDir(engDir) ,0.f ,distDir(engDir) };
	D3DXVec3Normalize(&m_vDir, &m_vDir);


	default_random_engine engSpeed(rdSpeed());
	uniform_real_distribution<float> distSpeed(3.5f, 9.5f);
	m_fSpeed = distSpeed(engSpeed);

	default_random_engine engTexture(rdTexture());
	uniform_real_distribution<float> distTexture(0.f, 2.9f);
	m_iTexture = (_uint)distTexture(engTexture);
}
