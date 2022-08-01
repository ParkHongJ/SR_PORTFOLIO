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
class CGravityBlock : public CGameObject
{

private:
	CGravityBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGravityBlock(const CGravityBlock& rhs);
	virtual ~CGravityBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDirection);

	void	UpdateGravitiy(_float fTimeDelta);
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
private:
	bool					m_bOnAir;
	bool					m_bOnBlock;

public:
	static CGravityBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

