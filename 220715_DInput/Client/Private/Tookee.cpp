#include "stdafx.h"
#include "..\Public\Tookee.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Hong.h"
#include "Interaction_Block.h"
#include "WarpBlock.h"

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
	CGameMgr::Get_Instance()->Set_Object_Data(L"Tookee_Dead", &m_bActive);
	_float3 vPos = ObjInfo.vPos;
	vPos.z += 3.f;
	vPos.y = 0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	//Register Tookee
	CGameMgr::Get_Instance()->SetTookee(this);
	return S_OK;
}

void CTookee::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	//현재모드
	m_eCurMode = CGameMgr::Get_Instance()->GetMode();

	//현재모드와 이전모드를 비교해서 같냐
	if (m_eCurMode == m_ePreMode)
	{
		//모드가 안바뀜
	}
	else
	{
		//모드가 바뀐시점, 탑디로 바뀌었다면
		if (m_eCurMode == CGameMgr::TOPDEE)
		{
			//애니메이션 초기화
			m_iTexIndex = 31;
		}
		else if (m_eCurMode == CGameMgr::TOODEE)
		{   
			// 투디상태일 때
			/* 투키상태의 초기화 */
			m_eCurState = TOOKEE_IDLE;
			//점프의 초기화
			m_bJump = false;
			m_fJumpTime = 0.f;
		}
		m_ePreMode = m_eCurMode;
	}
}

void CTookee::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	switch (m_eCurState)
	{
	case CTookee::TOOKEE_LEFT:
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
			m_iMinFrame = 13;
			m_iMaxFrame = 24;
		}
		else if (m_eCurMode == CGameMgr::TOPDEE)
		{
			m_iMinFrame = 34;
			m_iMaxFrame = 35;
		}
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_RIGHT:
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
			m_iMinFrame = 13;
			m_iMaxFrame = 24;
		}
		else if (m_eCurMode == CGameMgr::TOPDEE)
		{
			m_iMinFrame = 40;
			m_iMaxFrame = 41;
		}
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_UP:
		if (m_eCurMode == CGameMgr::TOPDEE)
		{
			m_iMinFrame = 37;
			m_iMaxFrame = 38;
		}
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_DOWN:
		if (m_eCurMode == CGameMgr::TOPDEE)
		{
			m_iMinFrame = 31;
			m_iMaxFrame = 32;
		}
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_JUMP:
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_iMinFrame = 25;
			m_iMaxFrame = 29;
		}
		break;
	case CTookee::TOOKEE_IDLE:
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_iMinFrame = 0;
			m_iMaxFrame = 12;
		}
		else if (m_eCurMode == CGameMgr::TOPDEE)
		{
			//이전 상태가 어느방향이냐에 따라서 조정해야함

			//현재 바뀐모드가 탑디면 보정
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			//수정 했음 나름 부드러워짐
			vPos.x = _uint(vPos.x) + 0.5f;
			vPos.z = _uint(vPos.z) + 0.5f;

			vPos = MoveTowards(vCurPos, vPos, fTimeDelta * 3.f);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			
			m_iMinFrame = 31;
			m_iMaxFrame = 31;
		}
		m_fSpeed = 0.f;
		break;
	default:
		break;
	}

	/* TOODEE */
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		//To do TOODEE
		MoveFrameToodee(fTimeDelta);
		Jump(fTimeDelta);
		m_pTransformCom->Set_TransformDesc_Speed(m_fSpeed);
		m_pTransformCom->Go_Straight_2D(fTimeDelta);
	}
	/* TOPDEE */
	else if (m_eCurMode == CGameMgr::TOPDEE)
	{
		MoveFrameTopdee(fTimeDelta);
		//To do TOPDEE
		_float fCollisionDist;

		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurState == TOOKEE_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurState == TOOKEE_UP)
			vCurDir.z = 1.f;
		else if (m_eCurState == TOOKEE_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurState == TOOKEE_LEFT)
			vCurDir.x = -1.f;

		if (CGameMgr::Get_Instance()->Check_Not_Go(vPos, vCurDir, &fCollisionDist, false))
		{
			//Edit Hong
			if (fCollisionDist != 0.f)
			{
				if (m_eCurState == TOOKEE_LEFT)
				{
					vPos.x += fCollisionDist;
				}
				else if (m_eCurState == TOOKEE_RIGHT)
				{
					vPos.x -= fCollisionDist;
				}
				else if (m_eCurState == TOOKEE_UP)
				{
					vPos.z -= fCollisionDist;
				}
				else if (m_eCurState == TOOKEE_DOWN)
				{
					vPos.z += fCollisionDist;
				}
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			}
		}
	}

	if (m_eCurMode == CGameMgr::TOODEE)
	{
		//문제있다
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		
		_float4x4 matWorld;
		D3DXMatrixIdentity(&matWorld);
		m_pTransformCom->Set_WorldMatrix(matWorld);
		
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, m_pToodeeCom, m_pTransformCom);
	}
	else if (m_eCurMode == CGameMgr::TOPDEE)
	{
		
		_float4x4		ViewMatrix;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

		D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);
		
		m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, m_pTopdeeCom, m_pTransformCom);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTookee::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (m_eCurMode == CGameMgr::TOODEE)
	{
		m_pVIBufferToodeeCom->Render();
	}
	else if (m_eCurMode == CGameMgr::TOPDEE)
	{
		m_pVIBufferTopdeeCom->Render();
	}

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		m_pToodeeCom->Render(Matrix);
	}
	else if (m_eCurMode == CGameMgr::TOPDEE)
	{
		m_pTopdeeCom->Render(Matrix);
	}
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

	if (other->CompareTag(L"Bullet"))
	{
		m_bActive = false;
		return;
	}
	/* TOODEE */
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		if (other->CompareTag(L"Box") || other->CompareTag(L"Wall")) {
			CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
			Safe_AddRef(TargetBox);

			_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);

			if (CCollider::DIR_UP == eDireciton) {
				if (0.2f < m_fJumpTime && m_bJump && m_eCurMode == CGameMgr::TOODEE)
					m_bJump = false;
				m_fJumpTime = 0.f;
				m_fJumpPower = 17.f;
				m_fMaxJumpTime = 0.6f;
				m_eCurState = TOOKEE_IDLE;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + (fBoxSize * 0.5f)));
			}
			else if (CCollider::DIR_DOWN == eDireciton) {
				if (fMyLength > abs(vMyPos.z - vBoxPos.z))
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBoxPos.z) - 0.001f)));
			}
			else if (CCollider::DIR_LEFT == eDireciton) {
				if (m_eCurState == TOOKEE_RIGHT)
					m_pTransformCom->Translate(_float3(-1.f, 0.f, 0.f)* fTimeDelta * m_fSpeed);
				else if (m_fSpeed > 0.f && m_eCurState == TOOKEE_JUMP)
					m_pTransformCom->Translate(_float3(-1.f, 0.f, 0.f)* fTimeDelta * m_fSpeed);
				else if (m_eCurState == TOOKEE_IDLE)
					m_pTransformCom->Translate(_float3(-1.f, 0.f, 0.f)* fTimeDelta * m_fSpeed);
			}
			else if (CCollider::DIR_RIGHT == eDireciton) {
				if (m_eCurState == TOOKEE_LEFT)
					m_pTransformCom->Translate(_float3(1.f, 0.f, 0.f)* fTimeDelta * m_fSpeed);
				else if (m_fSpeed > 0.f && m_eCurState == TOOKEE_JUMP)
					m_pTransformCom->Translate(_float3(1.f, 0.f, 0.f)* fTimeDelta * m_fSpeed);
				else if (m_eCurState == TOOKEE_IDLE)
					m_pTransformCom->Translate(_float3(1.f, 0.f, 0.f)* fTimeDelta * m_fSpeed);
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
					m_eCurState = TOOKEE_IDLE;
					if ((fMyLength / 3) > abs(vMyPos.z - vBlockPos.z))
						m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z + ((fMyLength / 3) - abs(vMyPos.z - vBlockPos.z))));
					else
						m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBlockPos.z + (fBoxSize * 0.5f)));
				}
				else {
					switch (dynamic_cast<CWarpBlock*>(other)->GetPartnerDir()) {
					case CWarpBlock::DIR_UP:
						if (m_bJump) {
							m_fSpeed = 0.f;
							m_fJumpTime = 0.f;
							if (30 > m_fJumpPower) {
								m_fJumpPower += 1.f;
								m_fMaxJumpTime += 0.03f;
							}
						}
						else if (!m_bJump) {
							m_bJump = true;
							m_fSpeed = 0.f;
							m_fJumpTime = 0.f;
							m_fJumpPower = 17.f;
							m_fMaxJumpTime = 0.6f;
						}
						break;
					case CWarpBlock::DIR_RIGHT:
						m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
						m_fSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
						break;
					case CWarpBlock::DIR_DOWN:
						break;
					case CWarpBlock::DIR_LEFT:
						m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
						m_fSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
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
						m_fSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
						break;
					case CWarpBlock::DIR_DOWN:
						m_fJumpTime = 0.6f;
						break;
					case CWarpBlock::DIR_LEFT:
						m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
						m_fSpeed += 50.f * abs((m_fJumpPower * fTimeDelta) + m_vGravityPower);
						break;
					case CWarpBlock::DIR_END:
						break;
					}
				}
			}
			else if (CCollider::DIR_LEFT == eDireciton) {
				if (CWarpBlock::DIR_LEFT != dynamic_cast<CWarpBlock*>(other)->GetDir()) {
					if (m_eCurState == TOOKEE_RIGHT) {
						m_pTransformCom->Go_Straight_2D(-fTimeDelta);
					}
					else if (m_eCurState == TOOKEE_IDLE) {
						m_pTransformCom->Go_Straight_2D(-fTimeDelta);
					}
					else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT) && m_eCurState == TOOKEE_JUMP) {
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
					if (m_eCurState == TOOKEE_LEFT) {
						m_pTransformCom->Go_Straight_2D(-fTimeDelta);
					}
					else if (m_eCurState == TOOKEE_IDLE) {
						m_pTransformCom->Go_Straight_2D(-fTimeDelta);
					}
					else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT) && m_eCurState == TOOKEE_JUMP) {
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
	}
	/* TOPDEE */
	else if (m_eCurMode == CGameMgr::TOPDEE && other->CompareTag(L"Box") || other->CompareTag(L"Wall"))
	{
		//To do TOPDEE
		//이거 위치 비교로도 가능.
		CInteraction_Block* pInteraction_Block = dynamic_cast<CInteraction_Block*>(other);

		if (pInteraction_Block == nullptr)
			return;

		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//부딪힌 상자.
		TopdeeIsPushed(vOtherPos);//투키가 밀려나는거.

		if (vOtherPos.y != 0.5f)
			return;
		
		if (m_pBoxList == nullptr)
		{
			//if Collision We Must Check NextBox.
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			m_pBoxList = pGameInstance->Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Cube");
			if (m_pBoxList == nullptr)
				return;
		}

		_float3 vCurDir = { 0.f,0.f,0.f };

		if (m_eCurState == TOOKEE_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurState == TOOKEE_UP)
			vCurDir.z = 1.f;
		else if (m_eCurState == TOOKEE_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurState == TOOKEE_LEFT)
			vCurDir.x = -1.f;

		vOtherPos += vCurDir;//이게 민 박스의 다음 체크해야할 박스의 위치.
		
		_uint iCount = 0;

		CInteraction_Block* pBlock = dynamic_cast<CInteraction_Block*>(other);
		//현재 충돌한 블럭이 벽이라면
		if (pBlock == nullptr) 
			return;

		list<CGameObject*> PushList;
		_bool bCanPush = true;

		FindCanPushBoxes(vOtherPos, vCurDir, iCount, PushList, bCanPush);//list push back
		
		if (!bCanPush)
			return;
		
		_float fdist = 0.f;

		vOtherPos -= vCurDir;

		if (CGameMgr::Get_Instance()->Check_Not_Go(vOtherPos, vCurDir, &fdist, true)) {
			return;
		}

		vOtherPos += vCurDir;
		pBlock->Box_Push_More(fTimeDelta, vOtherPos, true);//First
														   //_uint iCount{ 0 };
		for (auto& iter = PushList.begin(); iter != PushList.end(); ++iter)
		{
			CInteraction_Block* pBlock = (CInteraction_Block*)(*iter);
			CTransform* pTransform = (CTransform*)pBlock->Get_Component(L"Com_Transform");
			_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
			pBlock->Box_Push_More(fTimeDelta, (vPos + vCurDir), true);
		}
		m_bPushBox = true;
	}
	else if (m_eCurMode == CGameMgr::TOPDEE && other->CompareTag(L"WarpBlock"))
	{
		CInteraction_Block* pInteraction_Block = dynamic_cast<CInteraction_Block*>(other);
		if (pInteraction_Block == nullptr)
			return;
		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//부딪힌 상자.
		TopdeeIsPushed(vOtherPos);//탑디가 밀려나는거.
		if (vOtherPos.y != 0.5f)
			return;
		
		if (m_pBoxList == nullptr)
		{
			//if Collision We Must Check NextBox.
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			m_pBoxList = pGameInstance->Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Cube");
			if (m_pBoxList == nullptr)
				return;
		}
		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurState == TOOKEE_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurState == TOOKEE_UP)
			vCurDir.z = 1.f;
		else if (m_eCurState == TOOKEE_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurState == TOOKEE_LEFT)
			vCurDir.x = -1.f;

		vOtherPos += vCurDir;//이게 민 박스의 다음 체크해야할 박스의 위치.
		_uint iCount = 0;

		CInteraction_Block* pBlock = dynamic_cast<CInteraction_Block*>(other);
		if (pBlock == nullptr)//지금미는 블록이 벽이니?
			return;
		CWarpBlock* pBlockWarp = dynamic_cast<CWarpBlock*>(other);
		if (pBlockWarp != nullptr)
		{
			//만약 워프블럭이라면.
			//여기사 나오는 디렉션은 밀려 나가는 디렉션임.
			if (pBlockWarp->GetDir() == eDireciton)
				return;
		}
		list<CGameObject*> PushList;
		_bool bCanPush{ true };
		FindCanPushBoxes(vOtherPos, vCurDir, iCount, PushList, bCanPush);//list push back
		if (!bCanPush)
			return;
		_float fdist{ 0.f };
		vOtherPos -= vCurDir;
		if (CGameMgr::Get_Instance()->Check_Not_Go(vOtherPos, vCurDir, &fdist, true)) {
			return;
		}
		vOtherPos += vCurDir;
		pBlock->Box_Push_More(fTimeDelta, vOtherPos, true);//First
														   //_uint iCount{ 0 };
		for (auto& iter = PushList.begin(); iter != PushList.end(); ++iter)
		{
			CInteraction_Block* pBlock = (CInteraction_Block*)(*iter);
			CTransform* pTransform = (CTransform*)pBlock->Get_Component(L"Com_Transform");
			_float3 vPos{ pTransform->Get_State(CTransform::STATE_POSITION) };
			pBlock->Box_Push_More(fTimeDelta, (vPos + vCurDir), true);
		}
		m_bPushBox = true;

	}
}

void CTookee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CTookee::Jump(_float fTimeDelta)
{
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
	if (m_fDrop_Endline + abs(m_vGravityPower) > fPos.z)
	{
		m_bJump = false;
		m_fJumpTime = 0.f;
		m_fJumpPower = 17.f;
		m_fMaxJumpTime = 0.6f;
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

	CVIBuffer_Rect::RECTDESC tRectDesc;
	tRectDesc.vSize.x = 1.5f;
	tRectDesc.vSize.y = 1.5f;
	tRectDesc.vSize.z = 0.f;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferTopdeeCom, this, &tRectDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer_Toodee_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), TEXT("Com_VIBuffer2"), (CComponent**)&m_pVIBufferToodeeCom, this)))
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pToodeeCom, this, &BoxColliderDesc)))
		return E_FAIL;

	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	BoxColliderDesc.fRadius = 0.5f;
	BoxColliderDesc.bIsTrigger = true;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider2"), (CComponent**)&m_pTopdeeCom, this, &BoxColliderDesc)))
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

	Safe_Release(m_pTopdeeCom);
	Safe_Release(m_pToodeeCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferToodeeCom);
	Safe_Release(m_pVIBufferTopdeeCom);
	Safe_Release(m_pRendererCom);
}

void CTookee::TopdeeIsPushed(const _float3 _vPos)
{
	_float3 vTookeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	_float fDist = D3DXVec3Length(&(vTookeePos - _vPos));
	
	fDist *= 0.2f;
	
	if (m_eCurState == TOOKEE_LEFT)
		vTookeePos.x += fDist;
	else if (m_eCurState == TOOKEE_RIGHT)
		vTookeePos.x -= fDist;
	else if (m_eCurState == TOOKEE_UP)
		vTookeePos.z -= fDist;
	else if (m_eCurState == TOOKEE_DOWN)
		vTookeePos.z += fDist;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTookeePos);

}

void CTookee::FindCanPushBoxes(_float3 _vNextBoxPos, _float3 vPushDir, _uint& iCountReFunc, list<CGameObject*>& PushList, _bool& bCanPush)
{
	//들어온값은 다음 박스에 해당. 리스트에 담겨야하는 사이즈는 2개이다.
	if (!bCanPush)
		return;
	auto& iter = m_pBoxList->begin();
	for (_uint i = 0; i < m_pBoxList->size(); ++i)
	{
		CTransform* pNextBlock = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
		_float3 vNextBlockPos = pNextBlock->Get_State(CTransform::STATE_POSITION);
		if (vNextBlockPos.y < 0.f)
		{//
			++iter;
			continue;
		}
		if (vPushDir.x == 0.f)
		{//Up or Down
			if ((_int)_vNextBoxPos.z == (_int)vNextBlockPos.z)//찾으려는값임.
			{
				if ((_int)_vNextBoxPos.x == (_int)vNextBlockPos.x) {
					/*_vNextBoxPos += vPushDir;*/
					++iCountReFunc;

					_float3 vNextBoxPosFix{ ((_uint)_vNextBoxPos.x + 0.5f),((_uint)_vNextBoxPos.y + 0.5f) ,((_uint)_vNextBoxPos.z + 0.5f) };
					_float fdist{ 0.f };
					if (CGameMgr::Get_Instance()->Check_Not_Go(vNextBoxPosFix, vPushDir, &fdist, true)) {//WallCheck
						bCanPush = false;
						return;
					}
					_vNextBoxPos += vPushDir;
					PushList.push_back(*iter);
					FindCanPushBoxes(_vNextBoxPos, vPushDir, iCountReFunc, PushList, bCanPush);
					break;
				}
			}
		}
		else if (vPushDir.z == 0.f)
		{
			if ((_int)_vNextBoxPos.x == (_int)vNextBlockPos.x)//찾으려는값임.
			{
				if ((_int)_vNextBoxPos.z == (_int)vNextBlockPos.z) {
					//_vNextBoxPos += vPushDir;
					++iCountReFunc;
					_float3 vNextBoxPosFix{ ((_uint)_vNextBoxPos.x + 0.5f),((_uint)_vNextBoxPos.y + 0.5f) ,((_uint)_vNextBoxPos.z + 0.5f) };
					_float fdist{ 0.f };
					if (CGameMgr::Get_Instance()->Check_Not_Go(vNextBoxPosFix, vPushDir, &fdist, true)) {
						bCanPush = false;
						return;
					}
					_vNextBoxPos += vPushDir;
					PushList.push_back(*iter);
					FindCanPushBoxes(_vNextBoxPos, vPushDir, iCountReFunc, PushList, bCanPush);
					break;
				}
			}
		}
		++iter;
	}
}

void CTookee::MoveFrameToodee(_float fTimeDelta)
{
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
}

void CTookee::MoveFrameTopdee(_float fTimeDelta)
{
	m_fFrame += ((_float)m_iMaxFrame - (_float)m_iMinFrame + 1.f) * fTimeDelta;
	m_iTexIndex = (_uint)m_fFrame + m_iMinFrame;

	if (m_iTexIndex > (_float)m_iMaxFrame) {
		m_fFrame = 0.f;
		m_iTexIndex = m_iMinFrame;
	}
}

