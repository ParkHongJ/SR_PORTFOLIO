#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Toodee(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Block(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Monster_Pig(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Topdee(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Turret(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Bullet(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Portal(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Hole(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Cloud(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Key(const _tchar* pLayerTag);
	HRESULT Ready_Layer_KeyBox(const _tchar* pLayerTag);
private:
	void	LoadGameObject();
	vector<_float3> m_list;

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END