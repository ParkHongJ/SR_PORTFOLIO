#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CHong final : public CLevel
{
public:
	typedef struct TagInfo
	{
		wstring pPrototypeTag;
		wstring pLayerTag;
	}TAG_INFO;

	typedef struct ObjInfo
	{
		_float3 vPos;
		_uint iNumLevel;
		_uint iDirection;
		_uint iTex;
	}OBJ_INFO;
private:
	CHong(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CHong() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Block(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
public:
	static CHong* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
public:
	void GetFiles(vector<_tchar*> &vList, _tchar* sPath, bool bAllDirectories);
	void SaveGameObject();
	void LoadGameObject();
private:
	_float3	m_vPosition = { 0.5f, 0.5f, 0.5f }; // 현재 타일위치
	LPD3DXMESH m_pSphereMesh;

	

	map<TAG_INFO*, OBJ_INFO*> m_pObjects;
	static int iDir_Select;
	static int iLevel_Select;
	_float m_fMoveSize = 1.f;
	map<pair<const _tchar*, const _tchar*>, _float3> m_TestMap;
	//<<프로토타입태그, 레이어태그> 포지션값>
	//          키값               벨류
};
END
