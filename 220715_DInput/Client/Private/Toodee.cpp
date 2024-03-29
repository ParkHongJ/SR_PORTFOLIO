#include "stdafx.h"
#include "Toodee.h"

#include "ParticleMgr.h"
#include "GameInstance.h"
#include "C_FMOD.h"

#include "WarpBlock.h"

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
	PLAYER_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(PLAYER_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/* For.Toodee_Col */
	SetTag(L"Toodee");

	/* For.Portal_Data */
	CGameMgr::Get_Instance()->Set_Object_Data(L"Toodee_Portal", &m_bPortal);

	m_fStartPos = ObjInfo.vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_fStartPos);
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
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_fStartPos);
			m_bActive = true;
			m_bJump = false;
			m_fJumpTime = 0.f;
			m_MoveSpeed = 0.f;
			m_bDiedEff = false;
			m_bDiedSnd = false;
		}
	}

	/* For.Toodee_Turn */
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).x,
			0.3f,
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).z));

		if (CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_Clear"))
			return;

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
				if (m_eCurruntDir != TOODEE_JUMP)
				{
					//SetJump for Tookee
					CGameMgr::Get_Instance()->SetJumpTookee();
				}
				if (TOODEE_PORTAL != m_eCurruntDir) {
					m_eToodeeDir = TOODEE_JUMP;

					if (!m_bJump) {
						//Hong Edit For Effect
						CreateEffect();
						MakeSound(TEXT("jumpSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					}
				}

				m_bJump = true;
			}

			if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT)) {
				if (TOODEE_PORTAL != m_eToodeeDir && TOODEE_JUMP != m_eToodeeDir)
					m_eToodeeDir = TOODEE_LEFT;

				m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
				CGameMgr::Get_Instance()->SetScaeTookee(_float3(-1.f, 1.f, 1.f));
				if (5.f > m_MoveSpeed)
				m_MoveSpeed += 0.5f;
				else
					m_MoveSpeed -= 1.f;
				if (!m_bPortal) {
					m_iMinFrame = 13;
					m_iMaxFrame = 24;
				}
				else {
					m_iMinFrame = 30;
					m_iMaxFrame = 37;
				}

				//Set State For Tookee
				//SetStateTookee();
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT)) {
				//점프 상태가 아니고 이동을 했다면
				if (TOODEE_PORTAL != m_eToodeeDir && TOODEE_JUMP != m_eToodeeDir)
					m_eToodeeDir = TOODEE_RIGHT; //현재 상태를 이동으로 바꿈

				m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
				CGameMgr::Get_Instance()->SetScaeTookee(_float3(1.f, 1.f, 1.f));
				if (5.f > m_MoveSpeed)
					m_MoveSpeed += 0.5f;
				else
					m_MoveSpeed -= 1.f;
				if (!m_bPortal) {
					m_iMinFrame = 13;
					m_iMaxFrame = 24;
				}
				else {
					m_iMinFrame = 30;
					m_iMaxFrame = 37;
				}

				//Set State For Tookee
				//이때는 상태가 변하는게아니라 이동만 해야함..
				//SetStateTookee();
			}
			else {
				if (TOODEE_PORTAL != m_eToodeeDir && TOODEE_JUMP != m_eToodeeDir)
					m_eToodeeDir = TOODEE_IDLE;
			}
		}
		else if (!m_bActive) {
			m_eToodeeDir = TOODEE_DEAD;
			if (!m_bDiedSnd) {
				MakeSound(TEXT("dieSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
				m_bDiedSnd = true;
			}
		}

		m_eCurruntDir = m_eToodeeDir;
	}
	else {
		//GameMode : Topdee pos
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
				m_fSoundTimeDelta += fTimeDelta;
				if (m_fSoundTimeDelta > 0.75f) {
					MakeSound(TEXT("footstepsSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					m_fSoundTimeDelta = 0.f;
				}
				break;

			case TOODEE_RIGHT:
				m_fSoundTimeDelta += fTimeDelta;
				if (m_fSoundTimeDelta > 0.75f) {
					MakeSound(TEXT("footstepsSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					m_fSoundTimeDelta = 0.f;
				}
				break;

			case TOODEE_JUMP:
				m_iMinFrame = 25;
				m_iMaxFrame = 29;
				break;

			case TOODEE_PORTAL:
				if (!(CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT))
					&& !(CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT))) {
					m_MoveSpeed = 0.f;
				}
				if (!CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_Clear")) {
					m_iMinFrame = 30;
					m_iMaxFrame = 37;
				}
				break;

			case TOODEE_IDLE:
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
			m_vGravityPower = -1.63f * m_fJumpTime * 0.5f;

			if (m_fDrop_Endline + abs(m_vGravityPower) <= fPos.z) {
				if (m_bJump)
					fPos.z += m_fJumpPower * fTimeDelta;

				fPos.z += m_vGravityPower;

				if (m_fJumpTime > m_fMaxJumpTime)
					m_fJumpTime = m_fMaxJumpTime;
				else
					m_fJumpTime += fTimeDelta;
			}

			if (0.f > ((m_fJumpPower * fTimeDelta) + m_vGravityPower)) {
				if (1.f > m_fWarpTimer) {
					if (m_bJump)
						m_fWarpTimer += fTimeDelta * m_fTempTimer;
					else
						m_fWarpTimer += fTimeDelta * 4.f * m_fTempTimer;

					m_fTempTimer += m_fTempTimer * 0.025f;

					m_fForWarpPower = 13.f;
					m_fForWarpTime = 0.39f;
				}
			}

			if (m_fDrop_Endline + abs(m_vGravityPower) > fPos.z)
			{
				m_bJump = false;
				m_fJumpTime = 0.f;
				m_fJumpPower = 17.f;
				m_fMaxJumpTime = 0.6f;
				m_fWarpTimer = 0.f;
				m_fTempTimer = 0.4f;
				fPos.z = m_fDrop_Endline;
				m_eToodeeDir = TOODEE_IDLE;
			}
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
#pragma endregion
			SetStateTookee();

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

					CGameMgr::Get_Instance()->SetDeadPos(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
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
						CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
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

	_float test = fTimeDelta;

	tick = GetTickCount64();
	tick = tick / 1000.0f;
	m_fTimedelta = fTimeDelta;
	m_pShaderCom->Set_RawValue("g_time", &tick, sizeof(_float));

	if (!CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_Clear"))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CToodee::Render()
{
	/*if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
	return E_FAIL;*/

	if (m_bActive) {
		if (FAILED(m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture", m_iTexIndex)))
			return E_FAIL;
	}
	else if (!m_bActive) {
		if (FAILED(m_pTextureCom_Died->Bind_Texture(m_pShaderCom, "g_Texture", m_iTexIndexDied)))
			return E_FAIL;
	}

	//월드던질때 전치
	_float4x4 matWorld;
	matWorld = m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixTranspose(&matWorld, &matWorld);

	_float4x4 matView;
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView)))
		MSG_BOX(L"실패");
	D3DXMatrixTranspose(&matView, &matView);

	_float4x4 matProj;
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj)))
		MSG_BOX(L"씰퍠");
	D3DXMatrixTranspose(&matProj, &matProj);

	m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_float4x4));

	if (m_bPortal)
		m_pShaderCom->Begin(1);
	else
		m_pShaderCom->Begin(0);

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	m_pShaderCom->End();
	return S_OK;
}

void CToodee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CToodee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDireciton)
{
	if (other->CompareTag(L"Wave"))
	{
		m_bActive = false;
		m_iTexIndexDied = 0;
	}
	if (other->CompareTag(L"Pig")) {
		m_bActive = false;
		m_iTexIndexDied = 0;
	}

	if (other->CompareTag(L"Spike")) {
		m_bActive = false;
		m_iTexIndexDied = 0;
	}
	if (other->CompareTag(L"Bullet")) {
		m_bActive = false;
		m_iTexIndexDied = 0;
	}
	if (other->CompareTag(L"Portal")) {
		if (!m_bPortal) {
			m_bPortal = true;
		}
	}

	_float fBoxSize = 1.f;
	_float fMyLength = 1.5f;

	//Edit Hong
	if (other->CompareTag(L"Box") || other->CompareTag(L"Wall")) {
		CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
		Safe_AddRef(TargetBox);

		_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);

		if (CCollider::DIR_UP == eDireciton) {
			if (0.f > ((m_fJumpPower * fTimeDelta) + m_vGravityPower)) {
				m_bJump = false;
			}
			m_fJumpTime = 0.f;
			m_fJumpPower = 17.f;
			m_fMaxJumpTime = 0.6f;
			m_fWarpTimer = 0.f;
			m_fTempTimer = 0.4f;
			m_eToodeeDir = TOODEE_IDLE;
			if ((fMyLength / 3) > abs(vMyPos.z - vBoxPos.z))
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z + ((fMyLength / 3) - abs(vMyPos.z - vBoxPos.z))));
			else
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + (fBoxSize * 0.5f)));
		}
		else if (CCollider::DIR_DOWN == eDireciton) {
			if (fMyLength > abs(vMyPos.z - vBoxPos.z))
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBoxPos.z) - 0.001f)));
		}
		else if (CCollider::DIR_LEFT == eDireciton) {
			if (m_eCurruntDir == TOODEE_RIGHT) {
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(-1.f, 0.f, 0.f));
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT) && m_eCurruntDir == TOODEE_JUMP) {
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(-1.f, 0.f, 0.f));
			}
			else if (m_eCurruntDir == TOODEE_IDLE) {
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(-1.f, 0.f, 0.f));
			}
		}
		else if (CCollider::DIR_RIGHT == eDireciton) {
			if (m_eCurruntDir == TOODEE_LEFT) {
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(1.f, 0.f, 0.f));
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT) && m_eCurruntDir == TOODEE_JUMP) {
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(1.f, 0.f, 0.f));
			}
			else if (m_eCurruntDir == TOODEE_IDLE) {
				m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(1.f, 0.f, 0.f));
			}
		}

		Safe_Release(TargetBox);
	}

	if (other->CompareTag(L"WarpBlock")) {
		CTransform* TargetBlock = (CTransform*)other->Get_Component(L"Com_Transform");
		Safe_AddRef(TargetBlock);

		_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vBlockPos = TargetBlock->Get_State(CTransform::STATE_POSITION);

		if (CCollider::DIR_UP == eDireciton) {
			if (CWarpBlock::DIR_UP != dynamic_cast<CWarpBlock*>(other)->GetDir()) {
				if (0.f > ((m_fJumpPower * fTimeDelta) + m_vGravityPower)) {
					m_bJump = false;
				}
				m_fJumpTime = 0.f;
				m_fJumpPower = 17.f;
				m_fMaxJumpTime = 0.6f;
				m_fWarpTimer = 0.f;
				m_fTempTimer = 0.4f;
				m_eToodeeDir = TOODEE_IDLE;
				if ((fMyLength / 3) > abs(vMyPos.z - vBlockPos.z))
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z + ((fMyLength / 3) - abs(vMyPos.z - vBlockPos.z))));
				else
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBlockPos.z + (fBoxSize * 0.5f)));
			}
			else {
				switch (dynamic_cast<CWarpBlock*>(other)->GetPartnerDir()) {
				case CWarpBlock::DIR_UP:
					if (!m_bJump) {
						m_bJump = true;
						m_fJumpPower = 17.f;
						m_fMaxJumpTime = 0.6f;
					}
					if (30.f > m_fJumpPower) {
						m_fJumpPower += (m_fForWarpPower * m_fWarpTimer);
						m_fMaxJumpTime += (m_fForWarpTime * m_fWarpTimer);
					}
					if (30.f < m_fJumpPower) {
						m_fJumpPower = 30.f;
						m_fMaxJumpTime = 1.0f;
					}
					m_MoveSpeed = 0.f;
					m_fJumpTime = 0.f;
					m_fWarpTimer = 0.f;
					m_fTempTimer = 0.4f;
					m_eToodeeDir = TOODEE_IDLE;
					break;
				case CWarpBlock::DIR_RIGHT:
					m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
					m_MoveSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
					break;
				case CWarpBlock::DIR_DOWN:
					break;
				case CWarpBlock::DIR_LEFT:
					m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
					m_MoveSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
					break;
				case CWarpBlock::DIR_END:
					break;
				}
			}
		}
		else if (CCollider::DIR_DOWN == eDireciton) {
			if (CWarpBlock::DIR_DOWN != dynamic_cast<CWarpBlock*>(other)->GetDir()) {
				if (fMyLength > abs(vMyPos.z - vBlockPos.z))
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBlockPos.z) - 0.001f)));
			}
			else {
				switch (dynamic_cast<CWarpBlock*>(other)->GetPartnerDir()) {
				case CWarpBlock::DIR_UP:
					break;
				case CWarpBlock::DIR_RIGHT:
					m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
					m_MoveSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
					break;
				case CWarpBlock::DIR_DOWN:
					m_fJumpTime = 0.6f;
					break;
				case CWarpBlock::DIR_LEFT:
					m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
					m_MoveSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
					break;
				case CWarpBlock::DIR_END:
					break;
				}
			}
		}
		else if (CCollider::DIR_LEFT == eDireciton) {
			if (CWarpBlock::DIR_LEFT != dynamic_cast<CWarpBlock*>(other)->GetDir()) {
				if (m_eCurruntDir == TOODEE_RIGHT) {
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				}
				else if (m_eCurruntDir == TOODEE_IDLE) {
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				}
				else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT) && m_eCurruntDir == TOODEE_JUMP) {
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				}
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(-1.f, 0.f, 0.f));
			}
			else {
				switch (dynamic_cast<CWarpBlock*>(other)->GetPartnerDir()) {
				case CWarpBlock::DIR_UP:
					m_fJumpTime = 0.f;
					break;
				case CWarpBlock::DIR_RIGHT:
					break;
				case CWarpBlock::DIR_DOWN:
					break;
				case CWarpBlock::DIR_LEFT:
					break;
				case CWarpBlock::DIR_END:
					break;
				}
			}
		}
		else if (CCollider::DIR_RIGHT == eDireciton) {
			if (CWarpBlock::DIR_RIGHT != dynamic_cast<CWarpBlock*>(other)->GetDir()) {
				if (m_eCurruntDir == TOODEE_LEFT) {
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				}
				else if (m_eCurruntDir == TOODEE_IDLE) {
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				}
				else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT) && m_eCurruntDir == TOODEE_JUMP) {
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				}
				CGameMgr::Get_Instance()->SetPosition(fTimeDelta, _float3(1.f, 0.f, 0.f));
			}
			else {
				switch (dynamic_cast<CWarpBlock*>(other)->GetPartnerDir()) {
				case CWarpBlock::DIR_UP:
					m_fJumpTime = 0.f;
					break;
				case CWarpBlock::DIR_RIGHT:
					break;
				case CWarpBlock::DIR_DOWN:
					break;
				case CWarpBlock::DIR_LEFT:
					break;
				case CWarpBlock::DIR_END:
					break;
				}
			}
		}

		Safe_Release(TargetBlock);
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
	if (m_bPortal) {
		m_bPortal = false;
	}
}

void CToodee::SetStateTookee()
{
	//SetState Tookee
	switch (m_eCurruntDir)
	{
	case CToodee::TOODEE_LEFT:
		CGameMgr::Get_Instance()->SetStateTooKee(CTookee::STATE::TOOKEE_LEFT);
		break;
	case CToodee::TOODEE_RIGHT:
		CGameMgr::Get_Instance()->SetStateTooKee(CTookee::STATE::TOOKEE_RIGHT);
		break;
	case CToodee::TOODEE_JUMP:
		CGameMgr::Get_Instance()->SetStateTooKee(CTookee::STATE::TOOKEE_JUMP);

		CGameMgr::Get_Instance()->SetMoveSpeedTookee(m_MoveSpeed);
		break;
	case CToodee::TOODEE_IDLE:
		CGameMgr::Get_Instance()->SetStateTooKee(CTookee::STATE::TOOKEE_IDLE);

		break;
	default:
		break;
	}
}

void CToodee::CreateEffect()
{
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

HRESULT CToodee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Toodee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Texture_Died */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"), TEXT("Com_Texture_Died"), (CComponent**)&m_pTextureCom_Died, this)))
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
	BoxColliderDesc.fRadius = .5f;

	BoxColliderDesc.bIsTrigger = true;

	/* For.Com_BoxCollider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;
	return S_OK;
}

void CToodee::MakeSound(_tchar * pTag, _uint ID, _float Volum)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayEffect(pTag, ID, Volum);

	Safe_Release(pGameInstance);
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
	Safe_Release(m_pShaderCom);
}



