#pragma once

#include "Client_Defines.h"
#include "Particle_Spark.h"
#include "GameObject.h"
#include "Base.h"
#include "Transform.h"
#include "Tookee.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CGameMgr final : public CBase
{
	DECLARE_SINGLETON(CGameMgr)

public:
	enum GAMEMODE { TOODEE, TOPDEE, THIRDEE, FREE, MODE_END };
	
private:
	CGameMgr();
	virtual ~CGameMgr() = default;

public:
	HRESULT Initialize(_uint iNumLevel = NULL);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	GAMEMODE GetMode() { return m_eGameMode; }

#pragma region about Hole
public://Kyu. When We OpenLevel, We have to Call This Function. It can Make CanNotGo Spot.
	void Open_Level_Append_ObstaclePos(LEVEL eLayerLevel, const _tchar* pLayerTag,_bool bHole);//Caution!! Obstacle Cannot change Pos.
	_bool Check_Not_Go(const _float3& vCurPos, const _float3& vObjectDir, _float* pOut_ObjectsDist, _bool bPushCheck);// This func Manage Can Go or Not.		
	_bool Check_Not_Go_Monster(const _float3& vCurPos, _float* pOut_ObjectsDist, _bool bPushCheck);					// This func Manage Can Go or Not.

	_bool Check_Box_Down(const _float3& vBoxPos, _uint* pOut_iHoleNum_in_Layer,LEVEL* pOut_eHoleLevel);		// This func Manage Box Can Down.
	_bool Check_PushBox_Exactly(const _float3& vBoxPos);													// This func Manage PushBox Can Down..
	list<_float3> m_Obstaclelist;
	LEVEL m_eHoleLevel{ LEVEL_END };// Remember Input Level.
	_uint m_iHoleFinishNum{0};// Remember Input FinalHoleNumber.
#pragma endregion about Hole
	_bool m_bLoadFinish{ false };
public:
	void Set_Object_Data(const _tchar* pTag, _bool* bData);
	_bool Get_Object_Data(const _tchar* pTag);
	
	void Particle_To_Player();
	void Player_Active();

	void Set_WB_Data_Pos(const _tchar* pTag, _float3 vPos);
	void Set_WB_Data_Dir(const _tchar* pTag, _uint iDir);
	_float3 Get_WB_Data_Dir(const _tchar* pTag);

	_bool Col_Obj(const _float3& vCurPos, _float* pOut_ObjectsDist, _uint* pPigDirOut, _bool bPushCheck);

	_bool Key_Pressing(_uchar KeyInput); // 누르는 동안 실행
	_bool Key_Down(_uchar KeyInput); // 누르는 동안 한번만 실행

	//현재 매커니즘
	//투디 or 탑디 이동명령 -> 게임매니저 -> 투키
	void SetTookee(CTookee* _TooKee) { 
		m_Tookee = _TooKee;
		Safe_AddRef(m_Tookee);
	}
	void SetStateTooKee(CTookee::STATE _eState) 
	{ 
		if (m_Tookee != nullptr)
		{
			m_Tookee->SetState(_eState); 
		}
	}
	void SetScaeTookee(_float3 _vScale) {
		if (m_Tookee != nullptr)
		{
			m_Tookee->SetScale(_vScale);
		}
	}
	void SetMoveSpeedTookee(_float _fSpeed) {
		if (m_Tookee != nullptr)
		{
			m_Tookee->SetSpeed(_fSpeed);
		}
	}
	void SetJumpTookee() {
		if (m_Tookee != nullptr)
		{
			m_Tookee->SetJump();
		}
	}
	void SetPosition(_float fTimeDelta, _float3 vDir) {
		if (m_Tookee != nullptr)
		{
			m_Tookee->SetPosition(fTimeDelta, vDir);
		}
	}

	//===Key===
	/* Key 전용 */
	void AddKey() { m_iKey++; }
	void DeleteKey();

	//=====WarpBlock=====
	HRESULT RegisterWarpBlock(class CWarpBlock* pWarpBlock);

	void SetGameMode(_bool _bRunMode) { m_bRunMode = _bRunMode; }
	_bool GetGameMode() { return m_bRunMode; }

	void SetDeadPos(_float3 _vPos);
	void SetFadeObj(_uint iNumLevel);
private:
	_bool m_bRunMode = false;
	map<const _tchar*, _bool*> m_Data;
	typedef map<const _tchar*, _bool*> DATA;

	map<const _tchar*, _float3> m_WBData;

	_bool m_bKeyState[256] = { false };

	GAMEMODE m_eGameMode = TOODEE/*MODE_END*/;

	class CParticle_Spark* m_pParticle_Spark = nullptr;


	_uint m_iNumLevel;
	_uint m_iKey = 0;

	class CTookee* m_Tookee = nullptr;
	
	class CWarpBlock* m_pFirstWarp = nullptr;
	class CFadeObject* m_pFadeObject = nullptr;

public:
	virtual void Free() override;
};
END