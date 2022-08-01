#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CHong final : public CLevel
{
private:
	CHong(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CHong() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Block(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
public:
	static CHong* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
public:
	void GetFiles(vector<_tchar*> &vList, _tchar* sPath, bool bAllDirectories);
	void SaveGameObject();
	void LoadGameObject();
private:
	_float3	m_vPosition = { 0.5f, 0.0f, 0.5f }; // 현재 타일위치
	vector<_float3> m_list;
	//vector<_tchar*> m_list;//test
	vector<class CGameObject*> m_pGameObject;
	vector<class CGameObject*> m_pPrototypes;
	map<const _tchar*, _float3 > m_Test;
	LPD3DXMESH m_pSphereMesh;


	//이걸 저장하려고함

	map<pair<const _tchar*, const _tchar*>, _float3> m_TestMap;
	//<<프로토타입태그, 레이어태그> 포지션값>
	//          키값               벨류

	const _tchar* m_SelectLayer = L"";
	const _tchar* m_SelectPrototypes = L"";
};
END
