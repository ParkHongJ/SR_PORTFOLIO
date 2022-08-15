#include "C_FMOD.h"

IMPLEMENT_SINGLETON(C_FMOD)

C_FMOD::C_FMOD()
{
}

HRESULT C_FMOD::Initialize()
{
	m_Result = FMOD::System_Create(&m_pSystem);
	if (m_Result != FMOD_OK) {
		MSG_BOX(L"FMOD_System_Create_Failed");
		return E_FAIL;
	}

	m_Result = m_pSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	if (m_Result != FMOD_OK) {
		MSG_BOX(L"FMOD_System_Init_Failed");
		return E_FAIL;
	}

	/*m_Result = m_pSystem->createChannelGroup(NULL, &m_pChannelGroupA);
	if (m_Result != FMOD_OK) {
		MSG_BOX(L"FMOD_Create_ChannelGroup_Failed");
		return E_FAIL;
	}*/

	if (FAILED(LoadSoundFile()))
		return E_FAIL;

	return S_OK;
}

HRESULT C_FMOD::LoadSoundFile()
{
	_tfinddata64_t fd;
	__int64 handle = _tfindfirst64(L"../../Client/Bin/Resources/Sound/*.*", &fd);
	if (handle == -1 || handle == 0)
		return E_FAIL;

	int iResult = 0;

	char szCurPath[128] = "../../Client/Bin/Resources/Sound/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1)
	{
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD::Sound* pSound = nullptr;

		m_Result = m_pSystem->createSound(szFullPath, FMOD_LOOP_OFF, nullptr, &pSound);
		if (m_Result == FMOD_OK)
		{
			int iLength = static_cast<int>(strlen(szFilename) + 1);

			_tchar* m_pSoundTag = new _tchar[iLength];
			ZeroMemory(m_pSoundTag, sizeof(_tchar) * iLength);
			MultiByteToWideChar(CP_ACP, 0, szFilename, iLength, m_pSoundTag, iLength);

			m_mapSound.emplace(m_pSoundTag, pSound);
		}
		iResult = _tfindnext64(handle, &fd);
	}

	m_Result = m_pSystem->update();
	if (m_Result != FMOD_OK) {
		MSG_BOX(L"FMOD_Update_Failed");
		return E_FAIL;
	}

	_findclose(handle);

	return S_OK;
}

HRESULT C_FMOD::CheackPlaying(_uint Channel_ID)
{
	if (m_pChannel[Channel_ID])
	{
		_bool playing = false;

		m_pChannel[Channel_ID]->isPlaying(&playing);

		if (playing) {		
			return S_OK;
		}
		else if (!playing) {
			MSG_BOX(TEXT("Stop_Playing_Sound"));
			return S_OK;
		}
	}

	return S_OK;
}

void C_FMOD::Tick(_float fTimeDelta)
{
	m_pSystem->update();
}

int C_FMOD::VolumeUp(_uint eID, _float _vol)
{
	if (m_volume < SOUND_MAX) {
		m_volume += _vol;
	}
	m_pChannel[eID]->setVolume(m_volume);

	return 0;
}

int C_FMOD::VolumeDown(_uint eID, _float _vol)
{
	if (m_volume > SOUND_MIN) {
		m_volume -= _vol;
	}

	m_pChannel[eID]->setVolume(m_volume);

	return 0;
}

int C_FMOD::BGMVolumeUp(_float _vol, _uint eID)
{
	if (m_BGMvolume < SOUND_MAX) {
		m_BGMvolume += _vol;
	}
	
	m_pChannel[eID]->setVolume(m_volume);

	return 0;
}

int C_FMOD::BGMVolumeDown(_float _vol, _uint eID)
{
	if (m_BGMvolume > SOUND_MIN) {
		m_BGMvolume -= _vol;
	}

	m_pChannel[eID]->setVolume(m_volume);

	return 0;
}

int C_FMOD::Pause(_uint eID)
{
	m_bPause = !m_bPause;

	m_pChannel[eID]->setPaused(m_bPause);

	return 0;
}

int C_FMOD::Mute(_uint eID)
{
	m_bMute = !m_bMute;

	m_pChannel[eID]->setMute(m_bMute);

	return 0;
}

void C_FMOD::PlayEffect(_tchar * pSoundKey, _uint eID, _float _vol)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end()) {
		MSG_BOX(TEXT("SoundKey_Search_Failed"));
		return;
	}
	
	m_pSystem->playSound(iter->second, m_pChannelGroupA, FALSE, &m_pChannel[eID]);

	if (_vol >= SOUND_MAX)
		_vol = 1.f;
	else if (_vol <= SOUND_MIN)
		_vol = 0.f;

	m_pChannel[eID]->setVolume(_vol);

	m_pSystem->update();
}

void C_FMOD::PlayBGM(_tchar * pSoundKey, _uint eID, _float _vol)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end()) {
		MSG_BOX(TEXT("SoundKey_Search_Failed"));
		return;
	}

	m_pSystem->playSound(iter->second, m_pChannelGroupA, FALSE, &m_pChannel[eID]);
	m_pChannel[eID]->setMode(FMOD_LOOP_NORMAL);
	m_pChannel[eID]->setVolume(_vol);
	m_pSystem->update();
}

void C_FMOD::StopSound(_uint eID)
{
	m_pChannel[eID]->stop();
}

void C_FMOD::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		m_pChannel[i]->stop();
}

void C_FMOD::Free()
{
	for (auto& pair : m_mapSound) {
		delete[] pair.first;
		pair.second->release();
	}
	m_mapSound.clear();

	m_pSystem->release();
	m_pSystem->close();
}