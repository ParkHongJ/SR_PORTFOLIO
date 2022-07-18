#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CMyBox final: public CGameObject
{
private:
	CMyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMyBox(const CMyBox& rhs);
	virtual ~CMyBox() = default;
	
public:
	virtual HRESULT Initialize_Prototype()		override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void Tick(_float fTimeDelta)		override;
	virtual void LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

private:
	CTexture*				m_pTextureCom			= nullptr;
	CRenderer*				m_pRendererCom			= nullptr;
	CTransform*				m_pTransformCom			= nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom			= nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CMyBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()							override;
};

END