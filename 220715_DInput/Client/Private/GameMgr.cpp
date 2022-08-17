#include "stdafx.h"
#include "..\Public\GameMgr.h"

#include "GameInstance.h"
#include "KeyBlock.h"
#include "Tookee.h"
#include "WarpBlock.h"
#include "FadeObject.h"
IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
}

HRESULT CGameMgr::Initialize(_uint iNumLevel)
{
	//게임모드의 초기화
	m_eGameMode = TOODEE;
	m_iHoleFinishNum = 0;
	m_iNumLevel = iNumLevel;
	m_iKey = 0;
	m_Obstaclelist.clear();
	m_bLoadFinish = true;
	if (m_Tookee != nullptr)
	{
		Safe_Release(m_Tookee);
	}
	m_bRunMode = false;
	m_pFadeObject = nullptr;

	return S_OK;
}

void CGameMgr::Tick(_float fTimeDelta)
{
	if (Key_Down(DIK_X)) {
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if(TOODEE == m_eGameMode)
			pGameInstance->PlayEffect(TEXT("transUpSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
		else if(TOPDEE == m_eGameMode)
			pGameInstance->PlayEffect(TEXT("transDownSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);

		Safe_Release(pGameInstance);
		Particle_To_Player();
		Player_Active();
	}
}

void CGameMgr::LateTick(_float fTimeDelta)
{
}

void CGameMgr::Open_Level_Append_ObstaclePos(LEVEL eLayerLevel, const _tchar* pLayerTag, _bool bHole)
{
	list<CGameObject*>* pList = CGameInstance::Get_Instance()->GetLayer(eLayerLevel, pLayerTag);
	if (pList == nullptr)
		return;
	for (auto& iter = pList->begin(); iter != pList->end(); ++iter)
	{
		CTransform* pTransform = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
		if (pTransform == nullptr)
			return;
		m_Obstaclelist.push_back(pTransform->Get_State(CTransform::STATE_POSITION));
	}
	if (bHole) {
		m_eHoleLevel = eLayerLevel;
		m_iHoleFinishNum = (_uint)m_Obstaclelist.size();
	}
}

_bool CGameMgr::Check_Not_Go(const _float3& vCurPos, const _float3& vObjectDir, _float* pOut_ObjectsDist, _bool bPushCheck)// This func Manage Can Go or Not.
{
	if (m_Obstaclelist.empty())
		return false;

	_uint i = 0;
	for (auto& iter = m_Obstaclelist.begin(); iter != m_Obstaclelist.end(); ++iter)
	{
		if (bPushCheck) {
			if (i < m_iHoleFinishNum) {
				++i;
				continue;
			}
		}
		_float3 vObject_DirVector = ((*iter) - vCurPos);
		_float fDistance = D3DXVec3Length(&vObject_DirVector);
		if (fDistance <= 1.f) {//Objects Position Compare
			if (bPushCheck) {
				if (*pOut_ObjectsDist == -1.f)
				{
					if (fDistance == 0.f)
						return true;
					else {
						++i;
						continue;
					}
				}
				else if (vObject_DirVector != vObjectDir) {
					++i;
					continue;
				}
			}
			else
			{
				if (*pOut_ObjectsDist == -5.f) {
					if ((_int)vObject_DirVector.x == (_int)vObjectDir.x) {
						if ((_int)vObject_DirVector.z == (_int)vObjectDir.z)
							return true;
					}
					else
						continue;
				}
				
			}
			*pOut_ObjectsDist = abs(fDistance - 1.0f);
			return true;
		}
		++i;
	}
	return false;
}

_bool CGameMgr::Check_Not_Go_Monster(const _float3 & vCurPos, _float* pOut_ObjectsDist, _bool bPushCheck)
{
	if (m_Obstaclelist.empty())
		return false;

	for (auto& iter = m_Obstaclelist.begin(); iter != m_Obstaclelist.end(); ++iter)
	{
		_float fDistance = D3DXVec3Length(&((*iter) - vCurPos));
		if (fDistance <= 1.0f) {//Objects Position Compare
			*pOut_ObjectsDist = abs(fDistance - 1.0f);
			return true;
		}
	}
	return false;
}

_bool CGameMgr::Check_Box_Down(const _float3 & vBoxPos, _uint * pOut_iHoleNum_in_Layer, LEVEL* pOut_eHoleLevel)
{
	if (m_Obstaclelist.empty())
		return false;
	_uint i = 0;
	for (auto& iter = m_Obstaclelist.begin(); iter != m_Obstaclelist.end(); ++iter)
	{
		_float3 vPosOnlyXZ{ vBoxPos.x, 0.05f, vBoxPos.z };
		_float fDistance = D3DXVec3Length(&((*iter) - vPosOnlyXZ));
		if (fDistance <= 0.23f) {//Debug it Length is 1.9999f
			*pOut_eHoleLevel = m_eHoleLevel;
			*pOut_iHoleNum_in_Layer = i;
			iter->x = -100.f;
			iter->y = -100.f;
			iter->z = -100.f;
			return true;
		}
		++i;
	}
	return false;
}

_bool CGameMgr::Check_PushBox_Exactly(const _float3 & vBoxPos)
{
	if (m_Obstaclelist.empty())
		return false;
	_uint i = 0;
	for (auto& iter = m_Obstaclelist.begin(); iter != m_Obstaclelist.end(); ++iter)
	{
		_float3 vPosOnlyXZ{ vBoxPos.x,0.01f,vBoxPos.z };//0.3 means Hole.y is 0.3f
		_float fDistance = D3DXVec3Length(&((*iter) - vPosOnlyXZ));
		if (fDistance <= 0.2f) {//Debug it Length is 1.9999f
			iter->x = -100.f;
			iter->y = -100.f;
			iter->z = -100.f;
			list<CGameObject*>* pList = CGameInstance::Get_Instance()->GetLayer(m_eHoleLevel, (L"Layer_Hole"));
			if (pList == nullptr)
				return false;
			_uint j = 0;
			for (auto& iter = pList->begin(); (iter) != pList->end(); ++iter)
			{
				if (j != i) {
					++j;
					continue;
				}
				else
				{//rigid Hole
					CTransform* pTransform =(CTransform*)(*iter)->Get_Component(L"Com_Transform");
					pTransform->Set_State(CTransform::STATE_POSITION, _float3(-100.f, -100.f, -100.f));
					break;
				}
			}
			return true;
		}
		++i;
	}
	return false;
}

void CGameMgr::Set_Object_Data(const _tchar * pTag, _bool * bData)
{
	for (auto& pair : m_Data)
		if (pair.first == pTag) {
			pair.second = bData;
			return;
		}

	m_Data.emplace(pTag, bData);
	return;
}

_bool CGameMgr::Get_Object_Data(const _tchar * pTag)
{
	for (auto& pair : m_Data)
		if (pair.first == pTag) {
			return *(pair.second);
		}

	return false;
}

void CGameMgr::Particle_To_Player()
{
	if (m_pParticle_Spark == nullptr)
	{
		list<CGameObject*>* pList = CGameInstance::Get_Instance()->GetLayer(LEVEL_STATIC, L"Layer_Particle_Spark");
		if (pList == nullptr||pList ->empty())
			return;
		m_pParticle_Spark = (CParticle_Spark*)(pList->front());
	}
	
	CGameObject* pTopdee = CGameInstance::Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Topdee")->front();
	CGameObject* pToodee = CGameInstance::Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Toodee")->front();
	
	if (pTopdee == nullptr || pToodee == nullptr)
		return;
	
	CTransform* pTopdeeTrans = (CTransform*)(pTopdee->Get_Component(L"Com_Transform"));
	_float3 vTopdeePos{ pTopdeeTrans->Get_State(CTransform::STATE_POSITION) };

	CTransform* pToodeeTrans = (CTransform*)(pToodee->Get_Component(L"Com_Transform"));
	_float3 vToodeePos{ pToodeeTrans->Get_State(CTransform::STATE_POSITION) };

	if (m_eGameMode == TOODEE)//투디에서 탑디로.
		m_pParticle_Spark->Make_Line(vToodeePos, vTopdeePos);
	else if (m_eGameMode == TOPDEE)//탑디에서 투디로
		m_pParticle_Spark->Make_Line(vTopdeePos, vToodeePos);
	
}

void CGameMgr::Player_Active()
{
	m_eGameMode == TOODEE ? m_eGameMode = TOPDEE : m_eGameMode = TOODEE;
}

void CGameMgr::Set_WB_Data_Pos(const _tchar * pTag, _float3 vPos)
{
	for (auto& pair : m_WBData)
		if (pair.first == pTag) {
			pair.second = vPos;
			return;
		}

	m_WBData.emplace(pTag, vPos);

	return;
}

void CGameMgr::Set_WB_Data_Dir(const _tchar * pTag, _uint iDir)
{
	_float3 vDir = { 0.f, 0.f, 0.f };

	switch (iDir)
	{
	case 0:
		vDir = { 0.f, 0.f, 1.f };
		break;

	case 1:
		vDir = { 0.f, 0.f, -1.f };
		break;

	case 2:
		vDir = { -1.f, 0.f, 0.f };
		break;

	case 3:
		vDir = { 1.f, 0.f, 0.f };
		break;
	}

	for (auto& pair : m_WBData)
		if (pair.first == pTag) {
			pair.second = vDir;
			return;
		}

	m_WBData.emplace(pTag, vDir);

	return;
}

_float3 CGameMgr::Get_WB_Data_Dir(const _tchar * pTag)
{
	for (auto& pair : m_WBData)
		if (pair.first == pTag) {
			return pair.second;
		}

	return _float3(0.f, 0.f, 0.f);
}

_bool CGameMgr::Col_Obj(const _float3 & vCurPos, _float * pOut_ObjectsDist, _uint * pPigDirOut, _bool bPushCheck)
{
	if (m_Obstaclelist.empty())
		return false;

	_uint i = 0;
	for (auto& iter = m_Obstaclelist.begin(); iter != m_Obstaclelist.end(); ++iter)
	{
		if (bPushCheck) {
			if (i < m_iHoleFinishNum) {
				++i;
				continue;
			}
		}
		_float3 fDistance = (*iter) - vCurPos;
		if (abs(fDistance.z) > abs(fDistance.x)) {
			/*Dir Up/Down*/
			/* - / + */
			if (fDistance.z < 0) {
				//vCurPos->OnTriggerStay((*iter), fTimeDelta, DIR_UP);
				//iter->OnTriggerStay(vPos, fTimeDelta, DIR_UP);
			}
			else {
				//vCurPos->OnTriggerStay((*iter), fTimeDelta, DIR_DOWN);
				//iter->OnTriggerStay(vPos, fTimeDelta, DIR_DOWN);
			}
		}
		else if (abs(fDistance.x) > abs(fDistance.z)) {
			/*Dir Left/Right*/
			/* + / - */
			if (fDistance.x > 0) {
				//vCurPos->OnTriggerStay((*iter), fTimeDelta, DIR_LEFT);
				//iter->OnTriggerStay(vPos, fTimeDelta, DIR_LEFT);
			}
			else {
				//vCurPos->OnTriggerStay((*iter), fTimeDelta, DIR_RIGHT);
				//iter->OnTriggerStay(vPos, fTimeDelta, DIR_RIGHT);
			}
		}
		//이런거는 collider에서 하는거 아닌지.................

		//if (1.f >= fDistance) {
		//	*pOut_ObjectsDist = fDistance - 1.0f;
		//	*pPigDirOut = /*방향출력*/;
		//	return true;
		//}
		//++i;
	}

	return false;
}

_bool CGameMgr::Key_Pressing(_uchar KeyInput)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKState(KeyInput) & 0x80) {
		m_bKeyState[KeyInput] = true;
		Safe_Release(pGameInstance);
		return true;
	}

	Safe_Release(pGameInstance);
	return false;
}

_bool CGameMgr::Key_Down(_uchar KeyInput)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (!m_bKeyState[KeyInput] && (pGameInstance->Get_DIKState(KeyInput) & 0x80)) {
		m_bKeyState[KeyInput] = true;
		Safe_Release(pGameInstance);
		return true;
	}

	for (_uint i = 0; i < 256; ++i) {
		if (m_bKeyState[i] && !(pGameInstance->Get_DIKState(i) & 0x80)) {
			m_bKeyState[i] = false;
		}
	}

	Safe_Release(pGameInstance);
	return false;
}

void CGameMgr::DeleteKey()
{
	m_iKey--;

	if (m_iKey <= 0)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		list<CGameObject*>* m_pBoxList = pGameInstance->GetLayer(m_iNumLevel, L"Layer_KeyBox");
		if (m_pBoxList == nullptr)
			return;
		for (auto& iter : *m_pBoxList)
		{
			dynamic_cast<CKeyBlock*>(iter)->SetDead();
		}

		Safe_Release(pGameInstance);
	}
}

HRESULT CGameMgr::RegisterWarpBlock(CWarpBlock * pWarpBlock)
{
	//첫번째로 등록하는 워프 블록이 비어있다면
	if (m_pFirstWarp == nullptr)
	{
		//등록하고 레.카 추가
		m_pFirstWarp = pWarpBlock;
		Safe_AddRef(pWarpBlock);
	}
	else
	{
		//첫번째 워프블록이 이미있다면
		//두번째 워프블록 등록
		m_pFirstWarp->SetPartner(pWarpBlock);
		pWarpBlock->SetPartner(m_pFirstWarp);

		//서로 파트너를 맺어주고 레.카 초기화
		Safe_Release(m_pFirstWarp);
	}
	return S_OK;
}

void CGameMgr::SetDeadPos(_float3 _vPos)
{
	m_pFadeObject->SetOffset(_vPos);
}

void CGameMgr::SetFadeObj(_uint iNumLevel)
{
	m_pFadeObject = (CFadeObject*)CGameInstance::Get_Instance()->GetLayer(iNumLevel, L"Layer_Fade")->front();
}

void CGameMgr::Free()
{

	m_Data.clear();

	if (m_Tookee != nullptr)
		Safe_Release(m_Tookee);

	//워프블럭을 안쓰는 라운드는 삭제하면안됨!
	if (m_pFirstWarp != nullptr)
		Safe_Release(m_pFirstWarp);
}
