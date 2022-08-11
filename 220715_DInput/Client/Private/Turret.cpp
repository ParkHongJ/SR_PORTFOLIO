#include "stdafx.h"
#include "..\Public\Turret.h"

#include "GameInstance.h"
#include "KeyMgr.h"
#include "ParticleMgr.h"
#include "Hong.h"

CTurret::CTurret(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTurret::CTurret(const CTurret & rhs)
	: CGameObject(rhs)
	, m_fCurrentTimer(0.f)
{

}

HRESULT CTurret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTurret::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	
	m_eDir = (DIRECTION)ObjInfo.iDirection;

	_float3 vPos = ObjInfo.vPos;
	switch (m_eDir)
	{
	case DIRECTION::UP:
		vPos.z -= m_fDistance;
		break;
	case DIRECTION::DOWN:
		vPos.z += m_fDistance;
		break;
	case DIRECTION::LEFT:
		vPos.x += m_fDistance;
		break;
	case DIRECTION::RIGHT:
		vPos.x -= m_fDistance;
		break;
	default:
		break;
	}
	vPos.y += 0.6f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	//======================
	SetTag(L"Turret");
	//======================

	return S_OK;
}

void CTurret::Tick(_float fTimeDelta)
{
	Fire(fTimeDelta);
}

void CTurret::LateTick(_float fTimeDelta)
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CTurret::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_eDir)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CTurret::Fire(_float fTimeDelta)
{
	m_fCurrentTimer += fTimeDelta;
	if (m_fCurrentTimer > .2f)
	{
		m_fCurrentTimer = 0.f;
		
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		switch (m_eDir)
		{
		case DIRECTION::UP:
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel, vPos, _float3(0.f, 0.f, 1.f), CParticleMgr::BULLET);
			break;
		case DIRECTION::DOWN:
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel, vPos, _float3(0.f, 0.f, -1.f), CParticleMgr::BULLET);
			break;
		case DIRECTION::LEFT:
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel, vPos, _float3(-1.f, 0.f, 0.f), CParticleMgr::BULLET);
			break;
		case DIRECTION::RIGHT:
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel, vPos, _float3(1.f, 0.f, 0.f), CParticleMgr::BULLET);
			break;
		default:
			break;
		}

		for (int i = 0; i < 3; i++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<float> distrX(.4f, .7f);
			uniform_real_distribution<float> distrZ(-.4f, .4f);
			//random float

			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos.x += distrX(eng);
			vPos.z += distrZ(eng);
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);
		}
	}
	
}

HRESULT CTurret::Set_RenderState()
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

HRESULT CTurret::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CTurret::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;
	/* 이거 수정해라 */
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Turret"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 0.5f,1.f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &RectDesc)))
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


CTurret * CTurret::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTurret*		pInstance = new CTurret(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTurret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTurret::Clone(void * pArg)
{
	CTurret*		pInstance = new CTurret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CTurret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTurret::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

