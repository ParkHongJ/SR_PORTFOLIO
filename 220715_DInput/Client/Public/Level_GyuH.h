#pragma once
#include "Client_Defines.h"
#include "Level.h"
BEGIN(Client)
class CLevel_GyuH final : public CLevel
{
public:
	CLevel_GyuH(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GyuH() = default;
public:
	virtual HRESULT Initialize() override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Topdee(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Block(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Hole(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Spike(const _tchar* pLayerTag, void* pArg = nullptr);

	void	LoadGameObject();

public:
	static CLevel_GyuH* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
private:
	vector<_float3> m_list;
	_float3	m_vPosition = { 0.5f, 0.0f, 0.5f }; // 현재 타일위치
};
END