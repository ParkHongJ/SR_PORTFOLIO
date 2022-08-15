#pragma once

#include "Base.h"

#pragma region For.FMOD
#include "fmod.hpp"

#include <iostream>
#include <io.h>
#include <vector>
#include <tchar.h>
#include <string>

using namespace std;
using std::string;
using std::wstring;

#ifdef UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

#ifdef ENVIRONMENT32
#define T_FIND_DATA _tfinddata_t
#define T_FIND_FIRST _tfindfirst
#define T_FIND_NEXT _tfindnext
#endif
#ifdef ENVIRONMENT64
#define T_FIND_DATA _tfinddata64_t
#define T_FIND_FIRST _tfindfirst64
#define T_FIND_NEXT _tfindnext64
#endif

#define SOUND_MAX 1.f
#define SOUND_MIN 0.f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f
#pragma endregion

class ENGINE_DLL C_FMOD final : public CBase
{
	DECLARE_SINGLETON(C_FMOD)

public:
	enum CHANNELID { 
		BGM, BGM1, BGM2,
		PLAYER,
		EFFECT, EFFECT2, EFFECT3,
		UI,
		SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3,
		MAXCHANNEL
	};

private:
	C_FMOD();
	virtual ~C_FMOD() = default;

public:
	HRESULT Initialize();
	HRESULT LoadSoundFile();
	HRESULT CheackPlaying(_uint Channel_ID);
	void Tick(_float fTimeDelta);

public:
	int  VolumeUp(_uint eID, _float _vol);
	int  VolumeDown(_uint eID, _float _vol);
	int  BGMVolumeUp(_float _vol, _uint eID);
	int  BGMVolumeDown(_float _vol, _uint eID);
	int  Pause(_uint eID);
	int  Mute(_uint eID);
	void PlayEffect(_tchar* pSoundKey, _uint eID, _float _vol);
	void PlayBGM(_tchar* pSoundKey, _uint eID, _float _vol);
	void StopSound(_uint eID);
	void StopAll();
	void InitMute() { m_bMute = false; }

private:
	FMOD_RESULT m_Result;
	FMOD::System* m_pSystem = nullptr;
	FMOD::Channel* m_pChannel[MAXCHANNEL];
	FMOD::ChannelGroup* m_pChannelGroupA = nullptr;
	map<const _tchar*, FMOD::Sound*> m_mapSound;
	void* extradriverdata = nullptr;

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	_bool m_bPause = false;
	_bool m_bMute = false;

public:
	virtual void Free() override;
};

