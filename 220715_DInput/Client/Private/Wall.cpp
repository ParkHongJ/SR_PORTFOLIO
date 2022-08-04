#include "stdafx.h"
#include "..\Public\Wall.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CWall::CWall(const CWall & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWall::Initialize_Prototype()
{
	return S_OK;
}


void CWall::TextureSelect(const _float3 & vPos)	// 현 벽들의 위치는 가로: 0.5f ~ 28.5f
{												//					세로: 0.5f ~ 14.5f 이다.
	if (vPos.x == 0.5f && vPos.z == 0.5f)		// 좌하
		m_iTextureNum = 2;
	else if (vPos.x == 28.5f && vPos.z == 0.5f)	//우하
		m_iTextureNum = 3;
	else if (vPos.x == 28.5f&&vPos.z == 14.5f)	//우상
		m_iTextureNum = 4;
	else if (vPos.x == 0.5f&&vPos.z == 14.5f)	//좌하
		m_iTextureNum = 5;
	else if (((vPos.x != 0.5f) && (vPos.x != 28.5f)) && ((vPos.z == 0.5f) || (vPos.z == 14.5)))//가로
		m_iTextureNum = 1;
	else if (((vPos.x == 0.5f) || (vPos.x == 28.5f)) && ((vPos.z != 0.5f) && (vPos.z != 14.5)))//세로
		m_iTextureNum = 0;
}

HRESULT CWall::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
		m_iTextureNum = ObjInfo.iTex;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//======================
	m_Tag = L"Box";
	//======================


	if (m_pTransformCom != nullptr && pArg != nullptr)
	{
		_float3 vPos;
		vPos = ObjInfo.vPos;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	return S_OK;
}

void CWall::Tick(_float fTimeDelta)
{
	
}

void CWall::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, m_pBoxCollider, m_pTransformCom);
}

HRESULT CWall::Render()
{

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
 
HRESULT CWall::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CWall::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
	
}

HRESULT CWall::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

CWall * CWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWall*		pInstance = new CWall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWall::Clone(void* pArg)
{
	CWall*		pInstance = new CWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

