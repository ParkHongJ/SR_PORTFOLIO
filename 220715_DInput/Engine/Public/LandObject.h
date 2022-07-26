#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
public:
	typedef struct tagLandObjectDesc
	{
		_uint	iTerrainLevelIndex;
		_tchar* pLayerTag;
		_uint	iTerrainObjectIndex;
		_tchar* pTerrainBufferComTag;
	}LANDDESC;

protected:
	CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LANDDESC				m_LandDesc;

public:
	HRESULT SetUp_OnTerrain(class CTransform* pTransform, _float fMagicNumber = 0.f);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END