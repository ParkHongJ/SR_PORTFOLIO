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
	enum GAMEMODE { TOODEE, TOPDEE, THIRDEE, MODE_END };
	
private:
	CGameMgr();
	virtual ~CGameMgr() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	GAMEMODE GetMode() { return m_eGameMode; }

public:
	void Set_Player_Active(const _tchar* pTag, CGameObject* pPlayer);
	_bool IsPlayerActive(const _tchar* pTag);

	_bool Key_Pressing(_uchar KeyInput); // 누르는 동안 실행
	_bool Key_Down(_uchar KeyInput); // 누르는 동안 한번만 실행

private:
	map<const _tchar*, _bool> m_Data;
	typedef map<const _tchar*, _bool> DATA;

	_bool m_bKeyState[256] = { false };

	GAMEMODE m_eGameMode = TOODEE/*MODE_END*/;
public:
	virtual void Free() override;
};
END