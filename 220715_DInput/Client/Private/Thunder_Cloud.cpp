#include "stdafx.h"
#include "..\Public\Thunder_Cloud.h"

#include "GameInstance.h"
#include "GameMgr.h"

CThunder_Cloud::CThunder_Cloud(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CThunder_Cloud::CThunder_Cloud(const CThunder_Cloud & rhs)
	: CGameObject(rhs)
{
}

HRESULT CThunder_Cloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CThunder_Cloud::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom_Cloud->Set_State(CTransform::STATE_POSITION,  _float3(vPos.x, vPos.y /*+ 1.5f*/, vPos.z-0.01f));
		m_pTransformCom_Rain->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y /*+ 1.5f*/, vPos.z - 2.f));
		m_vToodeePos = vPos;
		vPos.y += 5.f;
		m_vTopdeePos = vPos;
	}// 이니셜라이즈 할때 레이를 두방향으로쏠예정 
	_float3 vCloudPos{ m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION) };
	m_pColliderCom->AddRayList(_float3(vCloudPos.x, m_vTopdeePos.y, vCloudPos.z)		, _float3(0.f, -1.f, 0.f));	//탑디일때의 레이는 -z방향
	m_pColliderCom->AddRayList(_float3(vCloudPos.x - 1.f, m_vTopdeePos.y, vCloudPos.z)	, _float3(0.f, -1.f, 0.f));	//탑디일때의 레이는 
	m_pColliderCom->AddRayList(_float3(vCloudPos.x + 1.f, m_vTopdeePos.y, vCloudPos.z)	, _float3(0.f, -1.f, 0.f));
	m_pColliderCom->AddRayList(_float3(vCloudPos.x, m_vToodeePos.y, vCloudPos.z), _float3(0.f, 0.f, -1.f));
	return S_OK;
}

_float3 CThunder_Cloud::Lerp(_float3 vPos, _float3 vTargetPos, _float fTimeDelta)
{
	return vPos + (vTargetPos - vPos) * fTimeDelta;
}

void CThunder_Cloud::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	//애니메이션 돌리고
	m_fFrame_Cloud += 6.f * fTimeDelta;
	if (m_fFrame_Cloud >= 6.0f)
		m_fFrame_Cloud = 0.f;

	m_fFrame_Rain += 13.0f * fTimeDelta;
	if (m_fFrame_Rain >= 13.0f)
		m_fFrame_Rain = 0.f;

	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)
	{
		m_pTransformCom_Cloud->Set_State(
			CTransform::STATE_POSITION,
			Lerp(m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION),
				m_vTopdeePos,
				fTimeDelta * 5.f));

		m_pTransformCom_Rain->Set_State(
			CTransform::STATE_POSITION,
			Lerp(m_pTransformCom_Rain->Get_State(CTransform::STATE_POSITION),
				_float3(m_vTopdeePos.x, m_vTopdeePos.y - 3.f, m_vTopdeePos.z),
				fTimeDelta * 5.f));
		//이게 어디에 천둥이치고있는지가 정확하게 보이지않음
		m_bEnabled = false;
	}
	else
	{
		m_pTransformCom_Cloud->Set_State(
			CTransform::STATE_POSITION,
			Lerp(m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION),
				m_vToodeePos,
				fTimeDelta * 5.f));

		m_pTransformCom_Rain->Set_State(
			CTransform::STATE_POSITION,
			Lerp(m_pTransformCom_Rain->Get_State(CTransform::STATE_POSITION),
				_float3(m_vToodeePos.x, m_vToodeePos.y, m_vToodeePos.z - 2.f),
				fTimeDelta * 5.f));

		m_bEnabled = true;
	}

}

void CThunder_Cloud::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

#pragma region billboard
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom_Cloud->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom_Cloud->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom_Cloud->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pTransformCom_Rain->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom_Rain->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom_Rain->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

#pragma endregion billboard

	m_pRendererCom_Cloud->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	//m_pRendererCom_Cloud->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CThunder_Cloud::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom_Rain->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Rain->Bind_Texture((_uint)m_fFrame_Rain)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom_Rain->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
	//================================================================RainEnd
	if (FAILED(m_pTransformCom_Cloud->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Cloud->Bind_Texture((_uint)m_fFrame_Cloud)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom_Cloud->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
	//---------------------디버그일때 그리기-------------------------
	_float4x4 Matrix = m_pTransformCom_Cloud->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------
	//문제상황은 탑디일때 어디가 비맞는 위치인지 분간하기가어렵다
	return S_OK;
}

void CThunder_Cloud::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (!m_bActive)
		return;

}

HRESULT CThunder_Cloud::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CThunder_Cloud::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CThunder_Cloud::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom_Cloud, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 3.0f, 3.f, 0.f };

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rain"), (CComponent**)&m_pVIBufferCom_Rain, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Rain"), TEXT("Com_Texture_Rain"), (CComponent**)&m_pTextureCom_Rain, this)))
		return E_FAIL;


	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_Rain"), (CComponent**)&m_pTransformCom_Rain, this, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;
//==================================================================Cloud End
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud"), TEXT("Com_Texture_Cloud"), (CComponent**)&m_pTextureCom_Cloud, this)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_Cloud"), (CComponent**)&m_pTransformCom_Cloud, this, &TransformDesc)))
		return E_FAIL;
	
	CVIBuffer_Rect::RECTDESC RectDesc2;
	RectDesc2.vSize = { 3.0f, 1.f, 0.f };
	
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Cloud"), (CComponent**)&m_pVIBufferCom_Cloud, this, &RectDesc2)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(3.f, 0.5f, 1.f);
	BoxColliderDesc.bIsTrigger = true;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;
	

	return S_OK;
}

CThunder_Cloud * CThunder_Cloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CThunder_Cloud* pInstance = new CThunder_Cloud(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CThunder_Cloud"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CThunder_Cloud::Clone(void * pArg)
{
	CThunder_Cloud* pInstance = new CThunder_Cloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CThunder_Cloud"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThunder_Cloud::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom_Cloud);
	Safe_Release(m_pVIBufferCom_Cloud);
	Safe_Release(m_pRendererCom_Cloud);
	Safe_Release(m_pTransformCom_Cloud);
	Safe_Release(m_pColliderCom);
	//====================================CloudEnd
	Safe_Release(m_pTextureCom_Rain);
	Safe_Release(m_pTransformCom_Rain);
	Safe_Release(m_pVIBufferCom_Rain);
}
