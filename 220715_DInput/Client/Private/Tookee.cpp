#include "stdafx.h"
#include "..\Public\Tookee.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Hong.h"
CTookee::CTookee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTookee::CTookee(const CTookee & rhs)
	: CGameObject(rhs)
{
}
HRESULT CTookee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTookee::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/* For.Toodee_Col */
	SetTag(L"Tookee");

	_float3 vPos = ObjInfo.vPos;
	vPos.z += 3.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	//Register Tookee
	CGameMgr::Get_Instance()->SetTookee(this);
	return S_OK;
}

void CTookee::Tick(_float fTimeDelta)
{
	//현재모드
	m_eCurMode = CGameMgr::Get_Instance()->GetMode();
	//현재모드와 이전모드를 비교해서 같냐
	if (m_eCurMode == m_ePreMode)
	{
		//모드가 안바뀜
	}
	else
	{
		//모드가 바뀐시점
		if (m_eCurMode == CGameMgr::TOPDEE)
		{
			//현재 바뀐모드가 탑디면 보정
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			vPos.x = _uint(vPos.x) + 0.5f;
			vPos.z = _uint(vPos.z) + 0.5f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		m_ePreMode = m_eCurMode;

		/* 투키상태의 초기화 */
		m_eCurState = TOOKEE_IDLE;
		//점프의 초기화
	}




	
}

void CTookee::LateTick(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case CTookee::TOOKEE_LEFT:
		m_fSpeed = 5.f;
		m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
		break;
	case CTookee::TOOKEE_RIGHT:
		m_fSpeed = 5.f;
		m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
		break;
	case CTookee::TOOKEE_UP:
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_DOWN:
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_JUMP:
		break;
	case CTookee::TOOKEE_IDLE:
		m_fSpeed = 0.f;
		break;
	default:
		break;
	}
	/* TOODEE */
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		//To do TOODEE
		Jump(fTimeDelta);
		m_pTransformCom->Set_TransformDesc_Speed(m_fSpeed);
		m_pTransformCom->Go_Straight_2D(fTimeDelta);
	}
	/* TOPDEE */
	else
	{
		//To do TOPDEE
	}

	m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, m_pBoxCom, m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTookee::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_bActive) {
		if (FAILED(m_pTextureCom->Bind_Texture(0)))
			return E_FAIL;
	}

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------

	return S_OK;
}

void CTookee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CTookee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDireciton)
{
	_float fBoxSize = 1.f;
	_float fMyLength = 1.5f;


	/* TOODEE */
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		if (other->CompareTag(L"Box")) {
			CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
			Safe_AddRef(TargetBox);

			_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);

			if (CCollider::DIR_UP == eDireciton) {
				if (0.2f < m_fJumpTime && m_bJump)
					m_bJump = false;
				m_fJumpTime = 0.f;
				m_eCurState = TOOKEE_IDLE;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + (fBoxSize * 0.5f)));
			}
			else if (CCollider::DIR_DOWN == eDireciton) {
				if (fMyLength > abs(vMyPos.z - vBoxPos.z))
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBoxPos.z) - 0.001f)));
			}
			else if (CCollider::DIR_LEFT == eDireciton) {
				if (m_eCurState == TOOKEE_RIGHT)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				//need fix
				else if (m_fSpeed > 0.f && m_eCurState == TOOKEE_JUMP)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				else if (m_eCurState == TOOKEE_IDLE)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			}
			else if (CCollider::DIR_RIGHT == eDireciton) {
				if (m_eCurState == TOOKEE_LEFT)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				//need fix
				else if (m_fSpeed > 0.f && m_eCurState == TOOKEE_JUMP)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				else if (m_eCurState == TOOKEE_IDLE)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			}

			Safe_Release(TargetBox);
		}
	}
	/* TOPDEE */
	else
	{
		//To do TOPDEE
	}
}

void CTookee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CTookee::Move(STATE _eState, _float fTimeDelta)
{
	switch (_eState)
	{
	case CTookee::TOOKEE_LEFT:
		break;
	case CTookee::TOOKEE_RIGHT:
		break;
	case CTookee::TOOKEE_UP:
		break;
	case CTookee::TOOKEE_DOWN:
		break;
	case CTookee::TOOKEE_JUMP:
		break;
	case CTookee::TOOKEE_IDLE:
		break;
	case CTookee::TOOKEE_END:
		break;
	default:
		MSG_BOX(L"잘못된 상태입니다 : TOOKEE");
		break;
	}
}

void CTookee::Jump(_float fTimeDelta)
{
	_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float vGravityPower = -1.63f * m_fJumpTime * 0.5f;

	if (m_fDrop_Endline + abs(vGravityPower) <= fPos.z) {
		if (m_bJump)
			fPos.z += m_fJumpPower * fTimeDelta;

		fPos.z += vGravityPower;

		if (m_fJumpTime > m_fMaxJumpTime)
			m_fJumpTime = m_fMaxJumpTime;
		else
			m_fJumpTime += fTimeDelta;
	}
	if (m_fDrop_Endline + abs(vGravityPower) > fPos.z)
	{
		m_bJump = false;
		m_fJumpTime = 0.f;
		fPos.z = m_fDrop_Endline;
		m_eCurState = TOOKEE_IDLE;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
}

void CTookee::SetScale(_float3 _vScale)
{
	m_pTransformCom->Set_Scale(_vScale);
}

void CTookee::SetPosition(_float fTimeDelta, _float3 vDir)
{
	m_pTransformCom->Translate(vDir * fTimeDelta * m_fSpeed);
}

void CTookee::CreateEffect()
{
	//Hong Edit For Effect
	for (int i = 0; i < 10; i++) {
		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(-.5f, .5f);
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
}

HRESULT CTookee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CTookee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CTookee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Toodee_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Tookee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.5f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 1.f);
	BoxColliderDesc.fRadius = 1.f;

	BoxColliderDesc.bIsTrigger = true;

	/* For.Com_BoxCollider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CTookee * CTookee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTookee* pInstance = new CTookee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTookee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTookee::Clone(void * pArg)
{
	CTookee* pInstance = new CTookee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CTookee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTookee::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


