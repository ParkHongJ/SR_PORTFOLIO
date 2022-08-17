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

	HRESULT Ready_Layer_Object(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);

	HRESULT Test_Cube();

private:
	typedef struct ObjInfo
	{
		_float3 vPos;
		_uint iNumLevel;
		_uint iDirection;
		_uint iTex;
	}OBJ_INFO;

public:
	static CLevel_SJH* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END