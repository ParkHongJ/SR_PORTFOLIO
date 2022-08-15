#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CSTAGE_RETURN1 final : public CLevel
{
private:
	CSTAGE_RETURN1(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CSTAGE_RETURN1() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Toodee(const _tchar* pLayerTag, void* pArg = nullptr);
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

	map<TAG_INFO*, OBJ_INFO*> m_pObjects;

public:
	static CSTAGE_RETURN1* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END