#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Terrain_Cube;
END
BEGIN(Client)
class CKeyBlock : public CGameObject
{
private:
	CKeyBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CKeyBlock(const CKeyBlock& rhs);
	virtual ~CKeyBlock() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void SetDead();
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Terrain_Cube*	m_pVIBufferCom = nullptr;
	CCollider*				m_pCollCom = nullptr;
	CBoxCollider*			m_pBoxCollider = nullptr;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();
public:
	static CKeyBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END