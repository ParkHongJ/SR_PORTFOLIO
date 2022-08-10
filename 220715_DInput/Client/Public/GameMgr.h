#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Base.h"

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
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	GAMEMODE GetMode() { return m_eGameMode; }

public://Kyu. When We OpenLevel, We have to Call This Function. It can Make CanNotGo Spot.
	void Open_Level_Append_ObstaclePos(LEVEL eLayerLevel, const _tchar* pLayerTag,_bool bHole);//Caution!! Obstacle Cannot change Pos.
	_bool Check_Not_Go(const _float3& vCurPos, _float* pOut_ObjectsDist,_bool bPushCheck);					// This func Manage Can Go or Not.
	_bool Check_Not_Go_Monster(const _float3& vCurPos, _float* pOut_ObjectsDist, _bool bPushCheck);					// This func Manage Can Go or Not.

	_bool Check_Box_Down(const _float3& vBoxPos, _uint* pOut_iHoleNum_in_Layer,LEVEL* pOut_eHoleLevel);		// This func Manage Box Can Down.
	_bool Check_PushBox_Exactly(const _float3& vBoxPos);													// This func Manage PushBox Can Down..
	
	list<_float3> m_Obstaclelist;
	LEVEL m_eHoleLevel{ LEVEL_END };// Remember Input Level.
	_uint m_iHoleFinishNum{0};// Remember Input FinalHoleNumber.

public:
	void Set_Object_Data(const _tchar* pTag, _bool* bData);
	_bool Get_Object_Data(const _tchar* pTag);
	void Player_Active();

	void Set_WB_Data_Pos(const _tchar* pTag, _float3 vPos);
	void Set_WB_Data_Dir(const _tchar* pTag, _uint iDir);

	_bool Col_Obj(const _float3& vCurPos, _float* pOut_ObjectsDist, _uint* pPigDirOut, _bool bPushCheck);

	_bool Key_Pressing(_uchar KeyInput); // 누르는 동안 실행
	_bool Key_Down(_uchar KeyInput); // 누르는 동안 한번만 실행

private:
	map<const _tchar*, _bool*> m_Data;
	typedef map<const _tchar*, _bool*> DATA;

	map<const _tchar*, _float3> m_WBData;

	_bool m_bKeyState[256] = { false };

	GAMEMODE m_eGameMode = TOODEE/*MODE_END*/;
public:
	virtual void Free() override;
};
END