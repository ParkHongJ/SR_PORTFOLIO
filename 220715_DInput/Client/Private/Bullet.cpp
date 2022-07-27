#include "stdafx.h"
#include "..\Public\Bullet.h"


#include "GameInstance.h"
CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void * pArg)
{
	return S_OK;
}

void CBullet::Tick(_float fTimeDelta)
{
	switch (m_eDir)
	{
	case DIRECTION::DOWN:
		m_pTransformCom->Translate(_float3(0.f, 0.f, -1.f) * fTimeDelta * m_fSpeed);
		break;
	case DIRECTION::UP:
		m_pTransformCom->Translate(_float3(0.f, 0.f, 1.f) * fTimeDelta * m_fSpeed);
		break;
	case DIRECTION::LEFT:
		m_pTransformCom->Translate(_float3(-1.f, 0.f, 0.f) * fTimeDelta * m_fSpeed);
		break;
	case DIRECTION::RIGHT:
		m_pTransformCom->Translate(_float3(1.f, 0.f, 0.f) * fTimeDelta * m_fSpeed);
		break;
	default:
		MSG_BOX(L"잘못된 방향입니다.");
		break;
	}
}

void CBullet::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollider->Add_CollisionGroup(CCollider::BULLET, this);

}

HRESULT CBullet::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::Set_RenderState()
{
	return S_OK;
}

HRESULT CBullet::Reset_RenderState()
{
	return S_OK;
}

HRESULT CBullet::SetUp_Components()
{
	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBullet*		pInstance = new CBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBullet::Clone(void * pArg)
{
	CBullet*		pInstance = new CBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
