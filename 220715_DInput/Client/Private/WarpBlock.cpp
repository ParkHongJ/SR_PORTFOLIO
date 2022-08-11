#include "stdafx.h"
#include "..\Public\WarpBlock.h"

#include "GameInstance.h"
#include "GameMgr.h"

_uint CWarpBlock::m_iWBCount = 0;

CWarpBlock::CWarpBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteraction_Block(pGraphic_Device)
{
}

CWarpBlock::CWarpBlock(const CWarpBlock & rhs)
	: CInteraction_Block(rhs)
{
}

HRESULT CWarpBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWarpBlock::Initialize(void * pArg)
{// Warp Texture is 0: Up, 1: Right 2: Down, 3: Left    This GiJun is Warp Direction.
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetTag(L"WarpBlock");
	
	if (pArg != nullptr)
	{
		_float4 vInitStruct;
		memcpy(&vInitStruct, pArg, sizeof(_float4));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vInitStruct.x, vInitStruct.y, vInitStruct.z));
		m_iTextureNum = (_uint)vInitStruct.w;			// w means TextureNum 
	}

	switch (m_iTextureNum)
	{
	case 0:
		m_eWarpDir = WD_UP;
		break;

	case 1:
		m_eWarpDir = WD_RIGHT;
		break;

	case 2:
		m_eWarpDir = WD_DOWN;
		break;

	case 3:
		m_eWarpDir = WD_LEFT;
		break;
	}

	m_iWBNum = m_iWBCount;
	++m_iWBCount;

	return S_OK;
}

void CWarpBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWarpBlock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (!m_bActive)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, m_pBoxCollider, m_pTransformCom);
}

HRESULT CWarpBlock::Render()
{
	if (!m_bActive)
		S_OK;
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureNum)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	/*_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCollider->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
	//--------------------------------------------------------------

	
	return S_OK;
}

void CWarpBlock::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CWarpBlock::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CWarpBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta)
{
}

HRESULT CWarpBlock::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CWarpBlock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CWarpBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_WarpBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pCollCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxDesc;
	BoxDesc.vPos = { 0.f,0.f,0.f };
	BoxDesc.vSize = { 1.f,1.f,1.f };
	BoxDesc.bIsTrigger = false;
	BoxDesc.fRadius = 0.5f;
	/* For.Com_BoxCollider*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCollider, this, &BoxDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CWarpBlock * CWarpBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWarpBlock*		pInstance = new CWarpBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWarpBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWarpBlock::Clone(void* pArg)
{
	CWarpBlock*		pInstance = new CWarpBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CWarpBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWarpBlock::Free()
{
	__super::Free();
	
}

