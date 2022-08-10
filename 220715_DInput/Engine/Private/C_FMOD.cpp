#include "C_FMOD.h"

IMPLEMENT_SINGLETON(C_FMOD)

C_FMOD::C_FMOD()
{
}

HRESULT C_FMOD::Initialize()
{
	m_Result = FMOD::System_Create(&m_pSystem);
	if (m_Result != FMOD_OK) return E_FAIL;

	m_Result = m_pSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	if (m_Result != FMOD_OK) return E_FAIL;

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

		m_Result = m_pSystem->createSound(szFullPath, FMOD_LOOP_NORMAL, nullptr, &pSound);
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
		bool playing = false;

		m_pChannel[Channel_ID]->isPlaying(&playing);

		if (playing) {
			m_Result = m_pSystem->update();
			if (m_Result != FMOD_OK) {
				MSG_BOX(L"FMOD_Update_Failed");
				return E_FAIL;
			}

			return S_OK;
		}

		if (!playing) {
			MSG_BOX(TEXT("Stop_Playing_Error"));
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT C_FMOD::Play(const _tchar* pSoundTag, _bool bLoop, _uint iChannelID, _float fVolum)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundTag));

	if (iter == m_mapSound.end()) {
		MSG_BOX(TEXT("Search_Tag_Error"));
		return E_FAIL;
	}

	m_Result = m_pSystem->playSound(iter->second, nullptr, bLoop, &m_pChannel[iChannelID]);

	m_pChannel[iChannelID]->setVolume(fVolum);

	m_Result = m_pSystem->update();
	if (m_Result != FMOD_OK) {
		MSG_BOX(L"FMOD_Update_Failed");
		return E_FAIL;
	}

	CheackPlaying(iChannelID);

	return S_OK;
}

void C_FMOD::Tick(_float fTimeDelta)
{
	m_Result = m_pSystem->update();
	if (m_Result != FMOD_OK) {
		MSG_BOX(L"FMOD_Update_Failed");
		return;
	}
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