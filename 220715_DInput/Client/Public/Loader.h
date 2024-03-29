#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 특정 레벨을 위한 리소스, 객체원형 등을 생성하는 역활을 한다. */
/* 서브스레드릉ㄹ 생성하여 위 작업을 수행하게끔 한다. */

BEGIN(Client)

class CLoader final : public CBase
{
public:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	LEVEL Get_NextLevelID() const {
		return m_eNextLevelID;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

	CRITICAL_SECTION Get_CS() const {
		return m_CriticalSection;
	}

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForGamePlayLevel();
	HRESULT Loading_ForHongLevel();
	HRESULT Loading_ForGyuHLevel();
	HRESULT Loading_ForGyuTestLevel();
	HRESULT Loading_ForSENILevel(_uint iNumLevel);
	HRESULT Loading_ForSJHLevel();
	HRESULT Loading_ForLEVEL2(_uint iNumLevel);
	HRESULT Loading_ForLEVEL3(_uint iNumLevel);
	HRESULT Loading_ForLEVEL4(_uint iNumLevel);
	HRESULT Loading_ForLEVEL5(_uint iNumLevel);
	HRESULT Loading_ForLEVEL6(_uint iNumLevel);
	HRESULT Loading_ForLEVEL7(_uint iNumLevel);
	HRESULT Loading_ForLEVEL8(_uint iNumLevel);
	HRESULT Loading_ForLEVEL9(_uint iNumLevel);
	HRESULT Loading_ForLEVEL10(_uint iNumLevel);

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;

private:
	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CriticalSection;
	LEVEL				m_eNextLevelID = LEVEL_END;

private:
	_tchar				m_szLoadingText[MAX_PATH] = TEXT("");
	_bool				m_isFinished = false;

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END