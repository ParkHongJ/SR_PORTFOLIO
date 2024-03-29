#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	typedef struct TerrainDesc
	{
		_float3 vPos;
		_uint iNumLevel;
	}TERRAINDESC; 
private:
	CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	//CVIBuffer_Terrain*		m_pVIBufferTerrain = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();


	//_float m_fSizeX{ 30.f }, m_fSizeY{ 16.f };
	_float m_fSizeX{ 29.f }, m_fSizeY{ 15.f };
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END