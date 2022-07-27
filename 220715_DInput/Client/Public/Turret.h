#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CTurret final : public CGameObject
{
public:
	typedef enum Direction
	{
		DOWN = 0, 
		RIGHT, 
		LEFT, 
		UP
	}DIRECTION;
private:
	CTurret(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTurret(const CTurret& rhs);
	virtual ~CTurret() = default;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components(); 

private:
	DIRECTION m_eDir = UP;
	_float m_fCurrentTimer;

public:
	static CTurret* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

public:
	void Fire(_float fTimeDelta);
};

END