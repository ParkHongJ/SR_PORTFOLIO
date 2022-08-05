#include "stdafx.h"
#include "..\Public\Monster_Pig.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Hong.h"

CMonster_Pig::CMonster_Pig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
	, m_bOnBlock(false)
	, m_bOnAir(true)
{
}

CMonster_Pig::CMonster_Pig(const CMonster_Pig & rhs)
	: CLandObject(rhs)
	, m_bOnBlock(false)
	, m_bOnAir(true)
{
}

HRESULT CMonster_Pig::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Pig::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_Tag = L"Pig";
	
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

void CMonster_Pig::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_fFrame += 9.0f * fTimeDelta;

	if (m_fFrame >= 9.0f)
		m_fFrame = 0.f;

}

void CMonster_Pig::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	//현재모드
	CGameMgr::GAMEMODE eCurMode = CGameMgr::Get_Instance()->GetMode();
	//현재모드와 이전모드를 비교해서 같냐
	if (eCurMode == m_ePreMode)
	{
		//모드가 안바뀜
	}
	else
	{
		//모드가 바뀐시점
		if (eCurMode == CGameMgr::TOPDEE)
		{
			////현재 바뀐모드가 탑디면 보정
			//_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			//vPos.x = _uint(vPos.x) + 0.5f;
			//vPos.z = _uint(vPos.z) + 0.5f;

			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			/* 이거 수정해라*/
			m_eCurDir = DIR_RIGHT;
		}
		m_ePreMode = eCurMode;
	}
	/* TOODEE */
	if (eCurMode == CGameMgr::TOODEE)
	{
 		UpdateGravitiy(fTimeDelta);
		_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float vGravityPower = -1.63f * fTimeDelta * 0.8f;
		if (m_fDrop_Endline + abs(vGravityPower) > fPos.z)
		{
			m_bOnBlock = true;
			fPos.z = m_fDrop_Endline;
		}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

	}
	/* TOPDEE */
	else
	{
		/* 이거 수정해라 */ 
		m_vTopdeePos = __super::SetUp_Topdee(m_pTransformCom, m_iNumLevel, L"Layer_topdee", 0, L"Com_Transform");
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fCollisionDist;

		_float3 vDir = m_vTopdeePos - vPos;
		if (CGameMgr::Get_Instance()->Check_Not_Go_Monster(vPos, &fCollisionDist, false))
		{
			if (m_eCurDir == DIR_LEFT)
				vPos.x += fCollisionDist + .0001f;
			else if (m_eCurDir == DIR_RIGHT)
				vPos.x -= fCollisionDist + .0001f;
			else if (m_eCurDir == DIR_UP)
				vPos.z -= fCollisionDist + .0001f;
			else if (m_eCurDir == DIR_DOWN)
				vPos.z += fCollisionDist + .0001f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		else
		{

			if (vDir.x > vDir.z && vDir.x > 0.f)
			{
				m_eCurDir = DIR_RIGHT;
			}
			else if (vDir.x < vDir.z && vDir.x < 0.f)
			{
				m_eCurDir = DIR_LEFT;
			}
			else if (vDir.z > vDir.x && vDir.z > 0.f)
			{
				m_eCurDir = DIR_UP;
			}
			else if (vDir.z < vDir.x && vDir.z < 0.f)
			{
				m_eCurDir = DIR_DOWN;
			}

			m_pTransformCom->Chase(m_vTopdeePos, 0.3 * fTimeDelta);
			
		}
		m_bOnBlock = false;
		m_fDrop_Endline = 0.f; 
	}

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pColliderCom->Add_CollisionGroup(CCollider::MONSTER, m_pBoxCom, m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMonster_Pig::Render()
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

	//---------------------디버그일때만 그리기------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------


	return S_OK;
}

void CMonster_Pig::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CMonster_Pig::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CMonster_Pig::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (other->CompareTag(L"Spike"))
	{
		m_bActive = false;
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
			CParticleMgr::Get_Instance()->ReuseObj(LEVEL_STAGE1,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);
		}
	}
	_float fBoxSize = 1.f;
	_float fMyLength = .5f;
	if (other->CompareTag(L"Box")) {
		CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
		Safe_AddRef(TargetBox);

		_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);

		if (CCollider::DIR_UP == eDirection) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + fBoxSize));
			m_bOnBlock = true;
		}
		else if (CCollider::DIR_DOWN == eDirection) {
			if (fMyLength > abs(vMyPos.z - TargetBox->Get_State(CTransform::STATE_POSITION).z))
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBoxPos.z))));
		}
		else if (CCollider::DIR_LEFT == eDirection) {
			m_pTransformCom->Go_Straight_2D(-fTimeDelta);
		}
		else if (CCollider::DIR_RIGHT == eDirection) {
			m_pTransformCom->Go_Straight_2D(-fTimeDelta);
		}
		Safe_Release(TargetBox);
	}
}

HRESULT CMonster_Pig::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CMonster_Pig::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CMonster_Pig::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Monster_Pig"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
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
	BoxColliderDesc.vSize = _float3(.5f, .5f, .5f);
	BoxColliderDesc.bIsTrigger = true;
	BoxColliderDesc.fRadius = .1f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Pig::UpdateGravitiy(_float fTimeDelta)
{
	_float3 vPigPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_bOnBlock)
	{
		m_bOnAir = false;
	}
	else
	{
		m_bOnAir = true;
	}

	if (m_bOnAir)
	{
		vPigPos.z += -9.8f * fTimeDelta * 0.8f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPigPos);
	}

}

CMonster_Pig * CMonster_Pig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster_Pig*		pInstance = new CMonster_Pig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMonster_Pig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Pig::Clone(void* pArg)
{
	CMonster_Pig*		pInstance = new CMonster_Pig(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CMonster_Pig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Pig::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
