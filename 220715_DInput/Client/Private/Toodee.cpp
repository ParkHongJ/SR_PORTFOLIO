#include "stdafx.h"
#include "Toodee.h"

#include "ParticleMgr.h"
#include "GameInstance.h"

CToodee::CToodee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CToodee::CToodee(const CToodee & rhs)
	: CGameObject(rhs)
{
}

HRESULT CToodee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CToodee::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/* For.Toodee_Col */
	SetTag(L"Toodee");

	/* For.Portal_Data */
	CGameMgr::Get_Instance()->Set_Object_Data(L"Toodee_Portal", &m_bPortal);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(2.f, 0.3f, 12.f));

	return S_OK;
}

void CToodee::Tick(_float fTimeDelta)
{
	if (CGameMgr::Get_Instance()->Key_Down(DIK_T)) {
		if (m_bActive) {
			m_bActive = false;
			m_iTexIndexDied = 0;
		}
		else {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(2.f, 0.3f, 2.f));
			m_bActive = true;
			m_bJump = false;
			m_fJumpTime = 0.f;
			m_MoveSpeed = 0.f;
			m_bDiedEff = false;
		}
	}

	/* For.Toodee_Turn */
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).x,
			0.3f,
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).z));

		if (m_bActive) {
			if (m_bPortal) {
				m_eToodeeDir = TOODEE_PORTAL;
				if ((CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_Clear"))) {
					m_eCurruntDir = m_eToodeeDir;
					return;
				}
				m_eCurruntDir = m_eToodeeDir;
			}

			if (CGameMgr::Get_Instance()->Key_Down(DIK_Z)) {
				if (TOODEE_PORTAL != m_eCurruntDir) {
					m_eToodeeDir = TOODEE_JUMP;

					if (!m_bJump) {
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
							CParticleMgr::Get_Instance()->ReuseObj(LEVEL_STAGE1,
								vPos,
								vPos - vPos2,
								CParticleMgr::PARTICLE);
						}
					}
				}
				m_bJump = true;
			}

			if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT)) {
				if (TOODEE_PORTAL != m_eToodeeDir && TOODEE_JUMP != m_eToodeeDir)
					m_eToodeeDir = TOODEE_LEFT;

				m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
				if (5.f > m_MoveSpeed)
					m_MoveSpeed += 0.1f;
				else if (5.f < m_MoveSpeed)
					m_MoveSpeed -= 1.f;
				if (!m_bPortal) {
					m_iMinFrame = 13;
					m_iMaxFrame = 24;
				}
				else {
					m_iMinFrame = 30;
					m_iMaxFrame = 37;
				}
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT)) {
				if (TOODEE_PORTAL != m_eToodeeDir && TOODEE_JUMP != m_eToodeeDir)
					m_eToodeeDir = TOODEE_RIGHT;

				m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
				if (5.f > m_MoveSpeed)
					m_MoveSpeed += 0.1f;
				else if (5.f < m_MoveSpeed)
					m_MoveSpeed -= 1.f;
				if (!m_bPortal) {
					m_iMinFrame = 13;
					m_iMaxFrame = 24;
				}
				else {
					m_iMinFrame = 30;
					m_iMaxFrame = 37;
				}
			}
			else {
				if (TOODEE_PORTAL != m_eToodeeDir && TOODEE_JUMP != m_eToodeeDir)
					m_eToodeeDir = TOODEE_IDLE;
			}
		}
		else if (!m_bActive) {
			m_eToodeeDir = TOODEE_DEAD;
		}

		m_eCurruntDir = m_eToodeeDir;
	}
	else {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).x,
			0.01f, 
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).z));
	}
}

void CToodee::LateTick(_float fTimeDelta)
{
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE) {
		if (m_bActive) {
#pragma region Toodee_Speed_and_Frame
			switch (m_eCurruntDir)
			{
			case TOODEE_LEFT:
				break;

			case TOODEE_RIGHT:
				break;

			case TOODEE_JUMP:
				m_iMinFrame = 25;
				m_iMaxFrame = 29;
				break;

			case TOODEE_PORTAL:
				if (!(CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT))
					&& !(CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT))) {
					if (0.f < m_MoveSpeed)
						m_MoveSpeed -= 0.1f;
					else
						m_MoveSpeed = 0.f;
				}
				if (!CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_Clear")) {
					m_iMinFrame = 30;
					m_iMaxFrame = 37;
				}
				break;

			case TOODEE_IDLE:
				if (0.f < m_MoveSpeed)
					m_MoveSpeed -= 1.f;
				else
					m_MoveSpeed = 0.f;
				m_iMinFrame = 0;
				m_iMaxFrame = 12;
				break;
			}

			if (m_bJump && m_iMaxFrame - 1 < m_iTexIndex)
				m_iTexIndex = m_iMaxFrame;
			else {
				if (m_iMinFrame > m_iTexIndex || m_iMaxFrame - 1 < m_iTexIndex)
					m_iTexIndex = m_iMinFrame;
				else {
					m_fFrame += m_iMaxFrame * fTimeDelta;

					if (m_fFrame > (m_iMaxFrame / 8)) {
						++m_iTexIndex;
						m_fFrame = 0.f;
					}
				}
			}
#pragma endregion

#pragma region Toodee_Jump_and_Drop
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
				m_eToodeeDir = TOODEE_IDLE;
			}
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
#pragma endregion

			if (CGameMgr::Get_Instance()->Key_Down(DIK_P)) { /* Test WarpBox for Up to Left/Right */
				m_MoveSpeed += 100.f * abs((m_fJumpPower * fTimeDelta) + vGravityPower);
			}

			m_pTransformCom->Set_TransformDesc_Speed(m_MoveSpeed);
			m_pTransformCom->Go_Straight_2D(fTimeDelta);
			//Edit Hong
			m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, m_pBoxCom, m_pTransformCom);
		}
		else if (!m_bActive) {
			if (5 > m_iTexIndexDied) {
				m_fFrame += 5.f * fTimeDelta;

				if (m_fFrame > 0.8f) {
					++m_iTexIndexDied;
					m_fFrame = 0.f;
				}
			}
			else {
				if (!m_bDiedEff) {
					//Hong Edit For Effect
					for (int i = 0; i < 50; i++) {
						random_device rd;
						default_random_engine eng(rd());
						uniform_real_distribution<float> distr(-.5f, .5f);

						//random float
						_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
						vPos.z += 0.5f; // toodee size / 2
						_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
						vPos2.z += 0.5f; // toodee size / 2

						vPos.x += distr(eng);
						vPos.z += distr(eng);
						CParticleMgr::Get_Instance()->ReuseObj(LEVEL_STAGE1,
							vPos,
							vPos - vPos2,
							CParticleMgr::PARTICLE);
					}
					m_bDiedEff = true;
				}
			}
			
		}
	}
	else { m_MoveSpeed = 0.f; }

	if (!CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_Clear"))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CToodee::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_bActive) {
		if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
			return E_FAIL;
	}
	else if (!m_bActive) {
		if (FAILED(m_pTextureCom_Died->Bind_Texture(m_iTexIndexDied)))
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

void CToodee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CToodee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDireciton)
{
	if (other->CompareTag(L"Pig")) {
		m_bActive = false;
		m_iTexIndexDied = 0;
	}

	if (other->CompareTag(L"Spike")) {
		m_bActive = false;
		m_iTexIndexDied = 0;
	}

	if (other->CompareTag(L"Portal"))
		m_bPortal = true;

	_float fBoxSize = 1.f;
	_float fMyLength = 1.5f;
	if (other->CompareTag(L"Box")) {
		CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
		Safe_AddRef(TargetBox);

		_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);

		if (CCollider::DIR_UP == eDireciton) {
			if(0.2f < m_fJumpTime && m_bJump)
				m_bJump = false;
			m_fJumpTime = 0.f;
			m_eToodeeDir = TOODEE_IDLE;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + (fBoxSize * 0.5f)));
		}
		else if (CCollider::DIR_DOWN == eDireciton) {
			if (fMyLength > abs(vMyPos.z - vBoxPos.z))
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBoxPos.z) - 0.001f)));
		}
		else if (CCollider::DIR_LEFT == eDireciton) {
			if (m_eCurruntDir == TOODEE_RIGHT)
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT) && m_eCurruntDir == TOODEE_JUMP)
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			else if (m_eCurruntDir == TOODEE_IDLE)
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
		}
		else if (CCollider::DIR_RIGHT == eDireciton) {
			if (m_eCurruntDir == TOODEE_LEFT)
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			else if(CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT) && m_eCurruntDir == TOODEE_JUMP)
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			else if(m_eCurruntDir == TOODEE_IDLE)
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
		}

		if (other->CompareTag(L"WarpBlock")) {
			_uint i = 0;
		}

		Safe_Release(TargetBox);
	}

	/*
	Up-Up
	if (m_bJump) {
		m_fJumpPower += 1.f;
		m_fMaxJumpTime += 0.03f;
	}
	else if (!m_bJump) {
		m_fJumpPower = 17.f;
		m_fMaxJumpTime = 0.6;
	}

	Down-Down
	Set_State(Position);
	m_bJump = false;

	Up-Left/Right
	maybe m_fMoveSpeed = (vGravityPower) + (m_fJumpPower * fTimeDelta);
	*/
}

void CToodee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	m_bPortal = false;
}

HRESULT CToodee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CToodee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CToodee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Toodee_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Texture_Died */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee_Died"), TEXT("Com_Texture_Died"), (CComponent**)&m_pTextureCom_Died, this)))
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

CToodee * CToodee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CToodee* pInstance = new CToodee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CToodee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CToodee::Clone(void* pArg)
{
	CToodee* pInstance = new CToodee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CToodee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToodee::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Died);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}