#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_SJH final : public CLevel
{
private:
	CLevel_SJH(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_SJH() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Toodee(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Topdee(const _tchar* pLayerTag, void* pArg = nullptr);

	HRESULT Ready_Layer_Monster_Pig(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Turret(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Bullet(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Portal(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Cloud(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Key(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Wall(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_KeyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Hole(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_Block(const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Ready_Layer_WarpBlock(const _tchar* pLayerTag, void* pArg = nullptr);


	HRESULT Ready_Layer_Object(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);

private:
	void	LoadGameObject();
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
	map<TAG_INFO*, _float3> m_pObjects;

public:
	static CLevel_SJH* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END