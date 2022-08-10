#pragma once
#include "Client_Defines.h"
#include "Level.h"
BEGIN(Client)
class CLevel_GyuTest final : public CLevel
{
public:
	CLevel_GyuTest(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GyuTest() = default;
public:
	virtual HRESULT Initialize() override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_WarpBox(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Topdee(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Wall(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_WarpBlock(const _tchar* pLayerTag, void* pArg = nullptr);//warp
	HRESULT Ready_Layer_Toodee(const _tchar* pLayerTag);//toodee
	HRESULT Ready_Layer_Particle_Spark(const _tchar* pLayerTag);//toodee
	HRESULT Ready_Layer_Object(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_ElectricBlock(const _tchar* pLayerTag, void* pArg = nullptr);//Electric
	HRESULT Ready_Layer_GravityBlock(const _tchar* pLayerTag, void* pArg = nullptr);//Electric
	void	LoadGameObject();

public:
	static CLevel_GyuTest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
private:
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

	map<TAG_INFO*, OBJ_INFO*> m_pObjects;
	_float3	m_vPosition = { 0.5f, 0.0f, 0.5f }; // 현재 타일위치
};
END