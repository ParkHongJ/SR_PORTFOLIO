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
	HRESULT Ready_Layer_Block(const _tchar* pLayerTag);
public:
	static CHong* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;


public:
	void GetFiles(vector<_tchar*> &vList, _tchar* sPath, bool bAllDirectories);
	void SaveWall();
private:
	_float3	m_vPosition = { 0.5f,0.0f,0.5f };
	vector<_tchar*> m_list;//test
	vector<class CGameObject*> m_pGameObject;
	vector<class CGameObject*> m_pPrototypes;
};
END
