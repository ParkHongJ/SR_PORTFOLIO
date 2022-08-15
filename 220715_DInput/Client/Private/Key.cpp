#include "stdafx.h"
#include "..\Public\Key.h"

#include "GameInstance.h"
#include "KeyBlock.h"
#include "ParticleMgr.h"
#include "Hong.h"
#include "GameMgr.h"

CKey::CKey(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CKey::CKey(const CKey & rhs)
	: CGameObject(rhs)
{
}

HRESULT CKey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKey::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(15.f, 0.5f, 3.f));

	CGameMgr::Get_Instance()->AddKey();
	return S_OK;
}

void CKey::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	//애니메이션 돌리고
	m_fFrame += 12.0f * fTimeDelta;
	if (m_fFrame >= 12.0f)
		m_fFrame = 0.f;

}

void CKey::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_CollisionGroup(CCollider::OBJECT, m_pBoxCom, m_pTransformCom);

}

HRESULT CKey::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture((_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
	return S_OK;
}

void CKey::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (!m_bActive)
		return;

	if (other->CompareTag(L"Toodee") || other->CompareTag(L"Topdee") || other->CompareTag(L"Pig"))
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlayEffect(TEXT("pickKeySnd.wav"), C_FMOD::CHANNELID::EFFECT2, SOUND_DEFAULT);

		Safe_Release(pGameInstance);

		//키는 사라지고
		m_bActive = false;

		//Hong Edit For Effect
		for (int i = 0; i < 7; i++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<float> distr(-.8f, .8f);
			//random float

			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos.x += distr(eng);
			vPos.z += distr(eng);
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);
		}

		CGameMgr::Get_Instance()->DeleteKey();




		//GetBoxList();

		//if (m_pBoxList == nullptr)
		//{
		//	MSG_BOX(L"KeyBlock비어있음");
		//}
		//else
		//{
		//	for (auto& iter : *m_pBoxList)
		//	{
		//		dynamic_cast<CKeyBlock*>(iter)->SetDead();
		//	}
		////박스 사라지게 하는함수
		//}
	}
}


HRESULT CKey::GetBoxList()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance); 
	m_pBoxList = pGameInstance->GetLayer(m_iNumLevel, L"Layer_KeyBox");
	if (m_pBoxList == nullptr)
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CKey::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CKey::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CKey::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 0.6f, 0.6f, 0.f };

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Key"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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
	BoxColliderDesc.fRadius = 0.2f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CKey * CKey::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKey* pInstance = new CKey(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKey::Clone(void * pArg)
{
	CKey* pInstance = new CKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKey::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
