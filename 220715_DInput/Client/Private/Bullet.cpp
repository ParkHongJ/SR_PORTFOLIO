#include "stdafx.h"
#include "..\Public\Bullet.h"


#include "GameInstance.h"
#include "ParticleMgr.h"
#include "Interaction_Block.h"
#include "WarpBlock.h"
CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs),
	m_fFrame(0.f)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void * pArg)
{
	if (pArg != nullptr)
	{
		memcpy(&m_iNumLevel, pArg, sizeof(_uint));
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	/*if (nullptr != pArg)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_eDir = RIGHT;
	}*/
	
	m_Tag = L"Bullet";
	return S_OK;
}

void CBullet::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
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
	m_fFrame += 10.f * fTimeDelta;

	if (m_fFrame >= 10.f)
		m_fFrame = 0.f;
}

void CBullet::LateTick(_float fTimeDelta)
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
		m_bActive = false;
		return;
	}
	else if (vPos.x - 0.1f > 1.f)
	{
		m_bActive = false;
		return;
	}

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	switch (m_eDir)
	{
	case DOWN:
		break;
	case UP:
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), 1.f);
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), 1.f);
		break;
	case LEFT:
		m_pTransformCom->Turn(-m_pTransformCom->Get_State(CTransform::STATE_LOOK), 1.f);
		break;
	case RIGHT:
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), 1.f);
		break;
	}
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollider->Add_CollisionGroup(CCollider::BULLET, m_pBoxCollider, m_pTransformCom);
}

HRESULT CBullet::Render()
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

HRESULT CBullet::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CBullet::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CBullet::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;
	/* 이거 수정해라 */
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Bullet"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 1.f,0.8f,0.f };

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

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	BoxColliderDesc.fRadius = 0.5f;
	BoxColliderDesc.bIsTrigger = false;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCollider, this, &BoxColliderDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pCollider, this)))
		return E_FAIL;
	return S_OK;
}

void CBullet::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (other->CompareTag(L"Box") || other->CompareTag(L"Topdee") ||
		other->CompareTag(L"Toodee") || other->CompareTag(L"Wall"))
	{
		m_bActive = false;
		for (int i = 0; i < 3; i++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<float> distrX(.4f, .7f);
			uniform_real_distribution<float> distrZ(-.4f, .4f);
			//random float

			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			vPos.x -= distrX(eng);
			vPos.z += distrZ(eng);

			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);
		}
	}
	if (other->CompareTag(L"WarpBlock"))
	{
		CWarpBlock* pBlockWarp = dynamic_cast<CWarpBlock*>(other);
		//만약 워프블럭이라면.
		//여기사 나오는 디렉션은 밀려 나가는 디렉션임.
		if (pBlockWarp->GetDir() != eDirection)
		{
			m_bActive = false;
			for (int i = 0; i < 3; i++)
			{
				random_device rd;
				default_random_engine eng(rd());
				uniform_real_distribution<float> distrX(.4f, .7f);
				uniform_real_distribution<float> distrZ(-.4f, .4f);
				//random float

				_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				vPos.x -= distrX(eng);
				vPos.z += distrZ(eng);

				CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
					vPos,
					vPos - vPos2,
					CParticleMgr::PARTICLE);
			}
			return;
		}
	}
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBullet* pInstance = new CBullet(pGraphic_Device);

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

void CBullet::SetUp(void* pArg)
{
	if (nullptr != pArg)
	{
		BULLET_DESC BulletDesc;
		memcpy(&BulletDesc, pArg, sizeof(BULLET_DESC));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, BulletDesc.vPos);
		m_eDir = BulletDesc.eDir;
		m_vDir = BulletDesc.vDir;
	}
	
}

void CBullet::SetDirection(_float3 _vDir)
{
	m_vDir = _vDir;
	if ((_uint)_vDir.x > 0)
	{
		m_eDir = DIRECTION::RIGHT;
	}
	else if ((_uint)_vDir.x < 0)
	{
		m_eDir = DIRECTION::LEFT;
	}
	else if ((_uint)_vDir.z > 0)
	{
		m_eDir = DIRECTION::UP;
	}
	else if ((_uint)_vDir.z < 0)
	{
		m_eDir = DIRECTION::DOWN;
	}
}
