#include "stdafx.h"
#include "..\Public\Button.h"

#include "GameInstance.h"
#include "ButtonBlock.h"
#include "ButtonBlock_Center.h"
#include "ParticleMgr.h"
#include "Hong.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_bPress(false)
	, m_bCheck(false)
{
}

CButton::CButton(const CButton & rhs)
	: CGameObject(rhs)
	, m_bPress(false)
	, m_bCheck(false)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
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
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos.y += 0.6f;
	vPos.z += 0.2f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	return S_OK;
}

void CButton::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

void CButton::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	_float4x4 ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3TransformCoord(&vPos, &vPos, &ViewMatrix);
	D3DXVec3TransformCoord(&vPos, &vPos, &ProjMatrix);

	if (vPos.x + 0.1f < -1.f)
	{
		return;
	}
	else if (vPos.x - 0.1f > 1.f)
	{
		return;
	}

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_CollisionGroup(CCollider::PORTAL, m_pBoxCom, m_pTransformCom);

}

HRESULT CButton::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (!m_bPress) {
		if (FAILED(m_pTextureCom->Bind_Texture(0)))
			return E_FAIL;
	}
	else if (m_bPress) {
		if (FAILED(m_pTextureCom->Bind_Texture(1)))
			return E_FAIL;
	}
		
	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
	return S_OK;
}

void CButton::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayEffect(TEXT("buttonPressSnd.wav"), C_FMOD::CHANNELID::EFFECT2, SOUND_DEFAULT);

	Safe_Release(pGameInstance);
}

void CButton::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (!m_bActive)
		return;

	if (other->CompareTag(L"Toodee") || other->CompareTag(L"Topdee") || other->CompareTag(L"Pig") || other->CompareTag(L"Tookee"))
	{
		m_bPress = true;

		if(!m_bCheck)
		{
			GetBoxList();
			if (m_pBoxList == nullptr)
			{
				MSG_BOX(L"ButtonBlock비어있음");
			}
			else
			{
				for (auto& iter : *m_pBoxList)
				{
					dynamic_cast<CButtonBlock*>(iter)->SetDead();
					//dynamic_cast<CButtonBlock_Center*>(iter)->SetDead();

					m_bCheck = true;
				}
			}
		}
	}
}

void CButton::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	if (m_bSnd) {
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlayEffect(TEXT("buttonReleaseSnd.wav"), C_FMOD::CHANNELID::EFFECT3, SOUND_DEFAULT);

		Safe_Release(pGameInstance);
		m_bSnd = false;
	}

	m_bPress = false;
	m_bCheck = false;

	GetBoxList();
	if (m_pBoxList == nullptr)
	{
		MSG_BOX(L"ButtonBlock비어있음");
	}

	for (auto& iter : *m_pBoxList)
	{
		iter->SetActive(true);
		CParticleMgr::Get_Instance()->EraseButton();
	}

}


HRESULT CButton::GetBoxList()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance); 
	m_pBoxList = pGameInstance->GetLayer(m_iNumLevel, L"Layer_ButtonBlock");
	if (m_pBoxList == nullptr)
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CButton::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CButton::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CButton::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;
	
	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 1.f,1.f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Button"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(1.8f, 0.5f, 1.f);
	BoxColliderDesc.bIsTrigger = true;
	BoxColliderDesc.fRadius = 1.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CButton * CButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton* pInstance = new CButton(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CButton::Clone(void * pArg)
{
	CButton* pInstance = new CButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
