#include "stdafx.h"
#include "..\Public\Topdee.h"

#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Interaction_Block.h"
#include "ElectricBlock.h"
#include "WarpBlock.h"

CTopdee::CTopdee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{
}

CTopdee::CTopdee(const CTopdee & rhs)
	: CLandObject(rhs)
{
}

HRESULT CTopdee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTopdee::Initialize(void * pArg)
{
	PLAYER_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(PLAYER_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/* For.Topdee_Col */
	SetTag(L"Topdee");
	_float3 vPos = ObjInfo.vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
#pragma region WhiteRect
	_float3 vPreLoaderPos = m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION);

	vPreLoaderPos.y += 1.f;
	m_pTransform_PreLoader_Com->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransform_PreLoader_Com->Set_State(CTransform::STATE_POSITION, _float3(vPreLoaderPos));
#pragma endregion WhiteRect

	if (KKK_m_pBoxList != nullptr)
	{

	}
	if (m_pRaiseObject != nullptr)
	{
		m_pRaiseObject = nullptr;
	}
	/* For.Portal_Data */
	CGameMgr::Get_Instance()->Set_Object_Data(L"Topdee_Portal", &m_bPortal);
	CGameMgr::Get_Instance()->Set_Object_Data(L"Topdee_Dead", &m_bActive);
	return S_OK;
}

void CTopdee::Tick(_float fTimeDelta)
{
	
	if (m_bPushBox)
		m_fPushBoxDelayTimer += fTimeDelta;

	//Edit Hong;
	if (CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_NextLevel")) {
		m_bInput = false;
	}


	_float TopdeeSpeed = m_pTransformCom->Get_Speed();
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	DeadCheck(fTimeDelta);
	if (m_bDead) {
		Safe_Release(pGameInstance);
		return;
	}
	KKK_IsRaise(fTimeDelta, 1);
	Topdee_PreLoader_Pos_Mgr();
	if (m_bInput)
	{
		if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)
		{
			if (!m_bActive) {
				Safe_Release(pGameInstance);
				return;
			}
			_float3 vTargetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 CheckTookeePos{ ((_int)vTargetPos.x + 0.5f),0.5f,((_int)vTargetPos.z + 0.5f) };
			if (CGameMgr::Get_Instance()->Key_Pressing(DIK_UP))
			{
				Move_Frame(DIR_UP);
				m_fSoundTimeDelta += fTimeDelta;
				if (m_fSoundTimeDelta > 0.5f) {
					MakeSound(TEXT("footstepsSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					m_fSoundTimeDelta = 0.f;
				}
				m_vTargetDir = { 0.f, 0.f, 1.f };
				_float fDist{ -5.f };
				if (CGameMgr::Get_Instance()->Check_Not_Go(CheckTookeePos, m_vTargetDir, &fDist, false))
				{
					m_bTookeeMove = false;
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_UP);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
				vTargetPos = m_vTargetDir * TopdeeSpeed *fTimeDelta;
				m_pTransformCom->Translate(vTargetPos);
				m_bPress = true;
				if (m_bTookeeMove) {
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_UP);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_DOWN))
			{
				Move_Frame(DIR_DOWN);
				m_fSoundTimeDelta += fTimeDelta;
				if (m_fSoundTimeDelta > 0.5f) {
					MakeSound(TEXT("footstepsSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					m_fSoundTimeDelta = 0.f;
				}
				m_vTargetDir = { 0.f, 0.f, -1.f };
				_float fDist{ -5.f };
				if (CGameMgr::Get_Instance()->Check_Not_Go(CheckTookeePos, m_vTargetDir, &fDist, false))
				{
					m_bTookeeMove = false;
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_DOWN);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
				vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
				m_pTransformCom->Translate(vTargetPos);
				m_bPress = true;
				//Edit Hong
				if (m_bTookeeMove) {
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_DOWN);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT))
			{
				Move_Frame(DIR_LEFT);
				m_fSoundTimeDelta += fTimeDelta;
				if (m_fSoundTimeDelta > 0.5f) {
					MakeSound(TEXT("footstepsSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					m_fSoundTimeDelta = 0.f;
				}
				m_vTargetDir = { -1.f, 0.f, 0.f };
				_float fDist{ -5.f };
				if (CGameMgr::Get_Instance()->Check_Not_Go(CheckTookeePos, m_vTargetDir, &fDist, false))
				{
					m_bTookeeMove = false;

					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_LEFT);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
				vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
				m_pTransformCom->Translate(vTargetPos);
				m_bPress = true;
				//Edit Hong
				if (m_bTookeeMove) {
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_LEFT);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
			}
			else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT))
			{
				Move_Frame(DIR_RIGHT);
				m_fSoundTimeDelta += fTimeDelta;
				if (m_fSoundTimeDelta > 0.5f) {
					MakeSound(TEXT("footstepsSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
					m_fSoundTimeDelta = 0.f;
				}
				m_vTargetDir = { 1.f, 0.f, 0.f };
				_float fDist{ -5.f };
				if (CGameMgr::Get_Instance()->Check_Not_Go(CheckTookeePos, m_vTargetDir, &fDist, false))
				{
					m_bTookeeMove = false;
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_RIGHT);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
				vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
				m_pTransformCom->Translate(vTargetPos);
				m_bPress = true;
				//Edit Hong
				if (m_bTookeeMove) {
					CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_RIGHT);
					CGameMgr::Get_Instance()->SetPosition(fTimeDelta, m_vTargetDir);
				}
			}
			else if (CGameMgr::Get_Instance()->Key_Down(DIK_Z))
			{//박스들기.

				KKK_DropBox(fTimeDelta);
				KKK_FindBox(fTimeDelta);
				m_bPress = true;
			}
			else if (CGameMgr::Get_Instance()->Key_Down(DIK_SPACE))
			{
				Rotate_WarpBlock();
			}
			else
				m_bPress = false;
		}
		else
			Not_My_Turn_Texture();
	}
	if (!m_bPress && m_bActive)
	{
		Go_Lerp(fTimeDelta);
		//Edit Hong 
		//키를뗏으면 투키를 보정시켜라
		CGameMgr::Get_Instance()->SetStateTooKee(CTookee::TOOKEE_IDLE);
	}

	m_bTookeeMove = true;
	Safe_Release(pGameInstance);
}

void CTopdee::RayCastedCheck()
{
	if (!m_bRayCasted)
		return;
	//레이캐스팅되었다면 들고있는물체가무엇인지 체크.
	if (m_pRaiseObject == nullptr)
		m_bActive = false;//들고있는 물체가 없었다면 나가리
	if (dynamic_cast<CElectricBlock*>(m_pRaiseObject) != nullptr)
		return;//일렉트릭블럭이라면 ok
	else
		m_bActive = false;
}

void CTopdee::DeadCheck(_float fTimeDelta)
{
	RayCastedCheck();
	if (m_bActive)
		return;
	if (m_bDead)
		return;
	if (m_eCurState == STATE_IDLE) {
		m_iFrame = 17;
		if (!m_bDiedSnd) {
			MakeSound(TEXT("dieSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
			m_bDiedSnd = true;//죽음시작
			
		}
	}

	m_eCurState = STATE_DEAD;

	m_fDeadTimer += 5.f * fTimeDelta;

	if (m_fDeadTimer > 0.8f)
	{
		m_fDeadTimer = 0.f;
		++m_iFrame;
		m_bActive = false;
	}
	if (m_iFrame > 21) {
		m_iFrame = 17;
		m_fDeadTimer = 0.f;
		for (int i = 0; i < 50; i++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<float> distr(-5.5f, 5.5f);
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vPos2 = vPos;
			vPos.x += distr(eng);
			vPos.z += distr(eng);
			CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);
			
		}
		m_bDead = true;

	}
}

void CTopdee::Go_Lerp(_float fTimeDelta)
{
	//Hong Edit
	/*_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vFinalPosition;

	vFinalPosition.x = _int(vCurPosition.x) + 0.5f;
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)
		vFinalPosition.y = m_MyTurnY;
	else
		vFinalPosition.y = m_NotMyTurnY;
	vFinalPosition.z = _int(vCurPosition.z) + 0.5f;

	vCurPosition = vCurPosition + (vFinalPosition - vCurPosition) * (fTimeDelta * 5);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);*/

	if (CGameMgr::Get_Instance()->GetMode()==CGameMgr::TOPDEE)
	{
		//현재 바뀐모드가 탑디면 보정
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		//수정 했음 나름 부드러워짐
		vPos.x = _uint(vPos.x) + 0.5f;
		vPos.y = m_MyTurnY;
		vPos.z = _uint(vPos.z) + 0.5f;

		vPos = MoveTowards(vCurPos, vPos, fTimeDelta * 3.f);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

void CTopdee::Topdee_PreLoader_Pos_Mgr()
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_int iPreLoaderPosX{ _int(vCurPosition.x) }, iPreLoaderPosY{ _int(vCurPosition.y) }, iPreLoaderPosZ{ _int(vCurPosition.z) };
	if (m_eCurDir == DIR_DOWN)
		--iPreLoaderPosZ;
	else if (m_eCurDir == DIR_UP)
		++iPreLoaderPosZ;
	else if (m_eCurDir == DIR_LEFT)
		--iPreLoaderPosX;
	else if (m_eCurDir == DIR_RIGHT)
		++iPreLoaderPosX;

	m_pTransform_PreLoader_Com->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransform_PreLoader_Com->Set_State(CTransform::STATE_POSITION, _float3(_float(iPreLoaderPosX) + 0.5f, _float(iPreLoaderPosY), _float(iPreLoaderPosZ) + 0.5f));
}

void CTopdee::Not_My_Turn_Texture()
{
	if (m_eCurState == STATE_DEAD)
		return;
	if (m_eCurDir == DIR_DOWN)
		m_iFrame = 14;
	else if (m_eCurDir == DIR_UP)
		m_iFrame = 16;
	else if (m_eCurDir == DIR_LEFT)
		m_iFrame = 15;
	else if (m_eCurDir == DIR_RIGHT)
		m_iFrame = 13;
}

void CTopdee::KKK_IsRaise(_float fTimeDelta, _char KKK_NotOverride)
{
	if (m_pRaiseObject == nullptr)
		return;
	else
	{
		if (((CInteraction_Block*)m_pRaiseObject)->Get_bDropFinish())
		{
			m_bDropBox = false;
			m_pRaiseObject = nullptr;
			m_fRaising_Box_DelayTimer = 0.f;
			m_vBoxDropPos = _float3(-1.f, -1.f, -1.f);
			return;
		}
	}
	if (m_fRaising_Box_DelayTimer == 15000.f) {
		//falling
		KKK_DropBox(fTimeDelta);
		return;
	}
	m_fRaising_Box_DelayTimer += fTimeDelta;
	_float3 vfinalPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vfinalPos.y += 1.f;
	m_pRaiseObject->KKK_Is_Raise(vfinalPos);
}

void CTopdee::Move_Frame(const TOPDEE_DIRECTION& _eInputDirection)
{
	/*topdee ??? ??
	left
	 0, 1 Topdee idle
	 2, 3, 4 Topdee Left
	 5, 6 Topdee Up
	 7, 8, 9 Topdee Right
	 10 ~ 12 Jump
	 17 ~ 21 Dead
	 */
	if (!m_bActive)
		return;
	if (m_eCurState == STATE_IDLE) {
		if (m_eCurDir == _eInputDirection) {
			if (!m_bMoveFrame) {
				if (m_iMoveFrameAdd < 20) {
					++m_iMoveFrameAdd;
					return;
				}
				++m_iFrame;
				m_bMoveFrame = true;
				m_iMoveFrameAdd = 0;
			}
			else {
				if (m_eCurDir == DIR_UP || m_eCurDir == DIR_DOWN) {
					m_iFrame = m_iFirstFrame;
					m_bMoveFrame = false;
				}
				else
				{
					if (!m_bMoveFrameAdd) {
						if (m_iMoveFrameAdd < 20) {
							++m_iMoveFrameAdd;
							return;
						}
						++m_iFrame;
						m_bMoveFrameAdd = true;
						m_iMoveFrameAdd = 0;
					}
					else
					{//초기화
						m_iFrame = m_iFirstFrame;
						m_bMoveFrame = false;
						m_bMoveFrameAdd = false;
						m_iMoveFrameAdd = 0;
					}
				}
			}
		}
		else
		{
			m_eCurDir = _eInputDirection;
			if (m_eCurDir == DIR_DOWN) {
				m_iFirstFrame = 0;
			}

			else if (m_eCurDir == DIR_UP) {
				m_iFirstFrame = 5;
			}

			else if (m_eCurDir == DIR_LEFT) {
				m_iFirstFrame = 2;
			}

			else if (m_eCurDir == DIR_RIGHT)
			{
				m_iFirstFrame = 7;
			}
			m_iFrame = m_iFirstFrame;
			m_bMoveFrame = false;
		}
	}


}

void CTopdee::LateTick(_float fTimeDelta)
{
	if (m_bDead)
		return;
#pragma region BillBoard
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	//========================================================================
	m_pRenderer_PreLoader_Com->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	//========================================================================
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//vPos.y = 0.5f;
#pragma endregion BillBoard
#pragma region Collision_Obstacle
	_float fCollisionDist{ 0.f };
	_float3 vCurDir{ 0.f,0.f,0.f };
	if (m_eCurDir == DIR_DOWN)
		vCurDir.z = -1.0f;
	else if (m_eCurDir == DIR_UP)
		vCurDir.z = 1.f;
	else if (m_eCurDir == DIR_RIGHT)
		vCurDir.x = 1.f;
	else if (m_eCurDir == DIR_LEFT)
		vCurDir.x = -1.f;
	if (CGameMgr::Get_Instance()->Check_Not_Go(vPos, vCurDir, &fCollisionDist, false))
	{
		//Edit Hong
		if (fCollisionDist != 0.f)
		{
			_float3 vTookeePos = { 0.f,0.f,0.f };
			if (m_eCurDir == DIR_LEFT)
			{
				vPos.x += fCollisionDist;
				_float fPos{ fCollisionDist + 1.f };
				vTookeePos.x = 1.f;
				//vTookeePos.x = 1.f;
			}
			else if (m_eCurDir == DIR_RIGHT)
			{
				vPos.x -= fCollisionDist;
				_float fPos{ fCollisionDist + 1.f };
				vTookeePos.x = -1.f;
			}
			else if (m_eCurDir == DIR_UP)
			{
				vPos.z -= fCollisionDist;
				_float fPos{ fCollisionDist + 1.f };
				vTookeePos.z = -1.f;
				//vTookeePos.z = -1.f;
			}
			else if (m_eCurDir == DIR_DOWN)
			{
				vPos.z += fCollisionDist;
				_float fPos{ fCollisionDist + 1.f };
				vTookeePos.z = 1.f;
				//vTookeePos.z = 1.f;
			}
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

			CGameMgr::Get_Instance()->SetPosition(fTimeDelta, vTookeePos);
		}
	}

#pragma endregion Collision_Obstacle	
	m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, m_pBoxCom, m_pTransformCom);

}

HRESULT CTopdee::Render()
{
	if (m_bDead|| m_bClear)
		return S_OK;
	if ((CGameMgr::Get_Instance()->Get_Object_Data(L"Toodee_Portal")) && (CGameMgr::Get_Instance()->Get_Object_Data(L"Topdee_Portal"))) {
		m_bClear = true;
		return S_OK;
	}

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
#pragma region PreLoader
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE) {
		if (FAILED(m_pTransform_PreLoader_Com->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pTexture_PreLoader_Com->Bind_Texture(0)))
			return E_FAIL;

		if (FAILED(Set_RenderState()))
			return E_FAIL;
		m_pBuffer_PreLoader_Com->Render();
		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}
#pragma endregion PreLoader

	return S_OK;
}
void CTopdee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{

}

void CTopdee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (other->CompareTag(L"Portal"))
	{
		m_bPortal = true;

	}
	else if (other->CompareTag(L"Pig"))
	{
		m_bActive = false;
	}
	else if (other->CompareTag(L"Bullet"))
	{
		m_bActive = false;
	}
	else if (other->CompareTag(L"WarpBlock"))
	{
		CInteraction_Block* pInteraction_Block = dynamic_cast<CInteraction_Block*>(other);
		if (pInteraction_Block == nullptr || pInteraction_Block->Get_bTopdeeRaise())
			return;
		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//부딪힌 상자.
		TopdeeIsPushed(vOtherPos, fTimeDelta);//탑디가 밀려나는거.
		if (vOtherPos.y != 0.5f)
			return;
		if (!m_bPushBox) {//MakseDelay
			m_fPushBoxDelayTimer += fTimeDelta;
		}
		else if ((m_bPushBox) && (m_fPushBoxDelayTimer < 0.1f)) { //처음 실행되었고
			return;
		}
		else if ((m_bPushBox) && (m_fPushBoxDelayTimer > 0.1f)) {
			m_fPushBoxDelayTimer = 0.f;
			m_bPushBox = false;
		}

		if (KKK_m_pBoxList == nullptr)
		{//if Collision We Must Check NextBox.
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			KKK_m_pBoxList = pGameInstance->Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Cube");
			if (KKK_m_pBoxList == nullptr)
				return;
		}
		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurDir == DIR_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurDir == DIR_UP)
			vCurDir.z = 1.f;
		else if (m_eCurDir == DIR_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurDir == DIR_LEFT)
			vCurDir.x = -1.f;
		vOtherPos += vCurDir;//이게 민 박스의 다음 체크해야할 박스의 위치.
		_uint iCount = 0;
		CInteraction_Block* pBlock = dynamic_cast<CInteraction_Block*>(other);
		if (pBlock == nullptr)//지금미는 블록이 벽이니?
			return;
		CWarpBlock* pBlockWarp = dynamic_cast<CWarpBlock*>(other);
		if (pBlockWarp != nullptr)
		{//만약 워프블럭이라면.
		 //여기사 나오는 디렉션은 밀려 나가는 디렉션임.
			if (pBlockWarp->GetDir() == eDirection)
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
	else if (other->CompareTag(L"Box"))
	{
		//이거 위치 비교로도 가능.
		/*if (!other->IsEnabled())
			return;*/
		CInteraction_Block* pInteraction_Block = dynamic_cast<CInteraction_Block*>(other);
		if (pInteraction_Block == nullptr || pInteraction_Block->Get_bTopdeeRaise())
			return;
		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//부딪힌 상자.
		TopdeeIsPushed(vOtherPos, fTimeDelta);//탑디가 밀려나는거.
		if (vOtherPos.y != 0.5f)
			return;
		if (!m_bPushBox) {//MakseDelay
			m_fPushBoxDelayTimer += fTimeDelta;
		}
		else if ((m_bPushBox) && (m_fPushBoxDelayTimer < 0.1f)) { //처음 실행되었고
			return;
		}
		else if ((m_bPushBox) && (m_fPushBoxDelayTimer > 0.1f)) {
			m_fPushBoxDelayTimer = 0.f;
			m_bPushBox = false;
		}

		if (KKK_m_pBoxList == nullptr)
		{//if Collision We Must Check NextBox.
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			KKK_m_pBoxList = pGameInstance->Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Cube");
			if (KKK_m_pBoxList == nullptr)
				return;
		}
		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurDir == DIR_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurDir == DIR_UP)
			vCurDir.z = 1.f;
		else if (m_eCurDir == DIR_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurDir == DIR_LEFT)
			vCurDir.x = -1.f;
		vOtherPos += vCurDir;//이게 민 박스의 다음 체크해야할 박스의 위치.
		_uint iCount = 0;
		CInteraction_Block* pBlock = dynamic_cast<CInteraction_Block*>(other);
		if (pBlock == nullptr)//지금미는 블록이 벽이니?
			return;
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
		if (m_fSoundTimeDelta > 0.1f) {
			MakeSound(TEXT("pushSnd.wav"), C_FMOD::CHANNELID::EFFECT3, SOUND_MAX);
			m_fSoundTimeDelta = 0.f;
		}
		m_bPushBox = true;
	}
	else if (other->CompareTag(L"Wall"))
	{
		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//부딪힌 상자.
		TopdeeIsPushed(vOtherPos, fTimeDelta);//탑디가 밀려나는거.
		if (vOtherPos.y != 0.5f)
			return;
	}
}

void CTopdee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	m_bPortal = false;
	m_eCurState = STATE_IDLE;
}

void CTopdee::TopdeeIsPushed(const _float3 _vOtherPos, _float fTimeDelta)
{
	//Box Pushing Topdee
	_float3 vTopdeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fDist = D3DXVec3Length(&(vTopdeePos - _vOtherPos));
	fDist *= 0.2f;
	_float3 vPos = { 0.f,0.f,0.f };

	if (m_eCurDir == DIR_LEFT)
	{
		vTopdeePos.x += fDist;
		vPos.x = 1.f;
	}
	else if (m_eCurDir == DIR_RIGHT)
	{
		vTopdeePos.x -= fDist;
		vPos.x = -1.f;
	}
	else if (m_eCurDir == DIR_UP)
	{
		vTopdeePos.z -= fDist;
		vPos.z = -1.f;
	}
	else if (m_eCurDir == DIR_DOWN)
	{
		vTopdeePos.z += fDist;
		vPos.z = 1.f;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTopdeePos);
	
	CGameMgr::Get_Instance()->SetPosition(fTimeDelta * 1.7f, vPos);
}

void CTopdee::FindCanPushBoxes(_float3 _vNextBoxPos, _float3 vPushDir, _uint& iCountReFunc, list<CGameObject*>& PushList, _bool& bCanPush)
{
	//들어온값은 다음 박스에 해당. 리스트에 담겨야하는 사이즈는 2개이다.
	if (!bCanPush)
		return;
	auto& iter = KKK_m_pBoxList->begin();
	for (_uint i = 0; i < KKK_m_pBoxList->size(); ++i)
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

void CTopdee::Rotate_WarpBlock()
{
	if (m_pRaiseObject == nullptr)
		return;
	CWarpBlock* pWarpBlock = dynamic_cast<CWarpBlock*>(m_pRaiseObject);
	if (pWarpBlock == nullptr)
		return;
	pWarpBlock->Rotate_WarpBlock();
}

#pragma region SetRender & Components
HRESULT CTopdee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CTopdee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CTopdee::Set_ColliderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return S_OK;
}

HRESULT CTopdee::Reset_ColliderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return S_OK;
}

void CTopdee::MakeSound(_tchar * pTag, _uint ID, _float Volum)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayEffect(pTag, ID, Volum);

	Safe_Release(pGameInstance);
}

HRESULT CTopdee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC tRectDesc;
	tRectDesc.vSize.x = 1.5f;
	tRectDesc.vSize.y = 1.5f;
	tRectDesc.vSize.z = 0.f;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this, &tRectDesc)))
		return E_FAIL;

	/*이거 수정해라*/
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Topdee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	//=================================================================
	/* For.PreLoader */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	/* For.Com_Renderer_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer_PreLoader"), (CComponent**)&m_pRenderer_PreLoader_Com, this)))
		return E_FAIL;
	/* For.Com_VIBuffer_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_PreLoader"), (CComponent**)&m_pBuffer_PreLoader_Com, this)))
		return E_FAIL;
	/* For.Com_Transform_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_PreLoader"), (CComponent**)&m_pTransform_PreLoader_Com, this, &TransformDesc)))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"), TEXT("Com_Texture_PreLoader"), (CComponent**)&m_pTexture_PreLoader_Com, this)))
		return E_FAIL;
	//=================================================================


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	BoxColliderDesc.fRadius = 0.5f;
	BoxColliderDesc.bIsTrigger = true;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;
	return S_OK;
}
#pragma endregion SetRender & Components
void CTopdee::KKK_FindBox(_float fTimeDelta)
{
	if (m_pRaiseObject != nullptr|| m_bDropBox)
			return;
	
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (KKK_m_pBoxList == nullptr) {
		KKK_m_pBoxList = pGameInstance->Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Cube");
		if (KKK_m_pBoxList == nullptr)
			return;
	}
	_float3 vTopdeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vTopdeePos.y += 1.f;

	auto& iter = (*KKK_m_pBoxList).begin();
	bool bMove{ false };
	_float3 vPreLoaderPos = m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION);
	for (_uint i = 0; i < (*KKK_m_pBoxList).size(); ++i)
	{
		bMove = (*iter)->KKK_Go_Lerp_Raise(vTopdeePos, fTimeDelta, vPreLoaderPos);
		if (bMove)
		{
			MakeSound(TEXT("pickUpSnd.wav"), C_FMOD::CHANNELID::EFFECT3, SOUND_MAX);
			break;
		}
		++iter;
	}

	if (!bMove)
		m_pRaiseObject = nullptr;
	else {
		m_fRaising_Box_DelayTimer = fTimeDelta;
		m_pRaiseObject = (*iter);
		((CInteraction_Block*)(*iter))->Set_bTopdeeRaise(true);
		
		//m_pRaiseObject->SetEnabled(false);
	}

}

void CTopdee::KKK_DropBox(_float fTimeDelta)
{
	if (m_pRaiseObject == nullptr)
		return;
	if (_int(m_fRaising_Box_DelayTimer) < 1)
		return;

	m_bDropBox = true;
	if (m_vBoxDropPos == _float3(-1.f, -1.f, -1.f)) {
		_float3 vDropPosCheck{ m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION) };
		vDropPosCheck.y = 0.5f;
		_float fDist{ -1.f };
		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurDir == DIR_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurDir == DIR_UP)
			vCurDir.z = 1.f;
		else if (m_eCurDir == DIR_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurDir == DIR_LEFT)
			vCurDir.x = -1.f;
		if (CGameMgr::Get_Instance()->Check_Not_Go(vDropPosCheck, vCurDir, &fDist, true))
		{//final Wall and Block DropPos 
			return;
		}
		for (auto& iter = KKK_m_pBoxList->begin(); iter != KKK_m_pBoxList->end(); ++iter)
		{
			CTransform* pTransform = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
			if (pTransform->Get_State(CTransform::STATE_POSITION) == vDropPosCheck)//이미 그자리에 박스가 있는 상황이라면
				return;
		}
		m_vBoxDropPos = vDropPosCheck;
		m_fRaising_Box_DelayTimer = 15000.f;
	}
	if (m_pRaiseObject->KKK_Go_Lerp_Drop(m_vBoxDropPos, fTimeDelta, false)) {
		_uint iLayerHoleNum{ 0 };
		m_bDropBox = false;
		if (CGameMgr::Get_Instance()->Check_Box_Down(m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION), &iLayerHoleNum, &m_eHoleLevel))//ask Can Box Drop?
		{
			//Rigid Hole
			list<CGameObject*>*pHoleList = CGameInstance::Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Hole");
			if (pHoleList == nullptr)
				return;
			auto& iter = pHoleList->begin();
			for (_uint i = 0; i < iLayerHoleNum; ++i)
				++iter;
			((CTransform*)(*iter)->Get_Component(L"Com_Transform"))->Set_State(CTransform::STATE_POSITION, _float3{ -100.f,-100.f,-100.f });	//it Makes Dont Make Bomb
			m_pRaiseObject->KKK_Go_Lerp_Drop(_float3(0.f, 0.f, 0.f), fTimeDelta, true);//it can Make Drop More.
		}
#pragma region Particle
		for (int i = 0; i < 10; i++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<float> distr(-.5f, .5f);
			_float3 vPos = m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION);
			_float3 vPos2 = vPos;
			vPos.x += distr(eng);
			vPos.z += distr(eng);
			/*CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
				vPos,
				vPos - vPos2,
				CParticleMgr::PARTICLE);*/
		}
#pragma endregion Particle
		((CInteraction_Block*)m_pRaiseObject)->Set_bTopdeeRaise(false);
		m_pRaiseObject = nullptr;

		MakeSound(TEXT("dropSnd.wav"), C_FMOD::CHANNELID::EFFECT3, SOUND_MAX);
		m_fRaising_Box_DelayTimer = 0.f;
		m_vBoxDropPos = _float3(-1.f, -1.f, -1.f);
	}
}

CTopdee * CTopdee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTopdee*		pInstance = new CTopdee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{

		(TEXT("Failed To Created : CTopdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTopdee::Clone(void* pArg)
{
	CTopdee*		pInstance = new CTopdee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CTopdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTopdee::Free()
{
	m_pRaiseObject = nullptr;
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

	Safe_Release(m_pBuffer_PreLoader_Com);
	Safe_Release(m_pTransform_PreLoader_Com);
	Safe_Release(m_pRenderer_PreLoader_Com);
	Safe_Release(m_pTexture_PreLoader_Com);

}
