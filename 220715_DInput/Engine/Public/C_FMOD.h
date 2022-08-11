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

#define SOUND_MAX 10.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 5.f
#define SOUND_WEIGHT 1.f
#pragma endregion

class C_FMOD final : public CBase
{
	DECLARE_SINGLETON(C_FMOD)

public:
	enum CHANNELID { BGM, PLAYER, EFFECT, UI, SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3, MAXCHANNEL };

private:
	C_FMOD();
	virtual ~C_FMOD() = default;

public:
	//FMOD::System* Get_System() { return m_pSystem; }
	//FMOD::Channel* Get_Channel(_uint ChannelID) { return m_pChannel[ChannelID]; }

public:
	HRESULT Initialize();
	HRESULT LoadSoundFile();
	HRESULT CheackPlaying(_uint Channel_ID);
	//void Tick(_float fTimeDelta);

public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(_tchar* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(_tchar* pSoundKey, _float _vol);
	void StopSound(CHANNELID eID);
	void StopAll();

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

public:
	virtual void Free() override;
};

