#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CCollider;
class CRenderer;
class CTransform;
class CBoxCollider;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CHole final : public CGameObject
{
private:
	CHole(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHole(const CHole& rhs);
	virtual ~CHole() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
public:
	virtual void OnTriggerExit(CGameObject* other, float fTimeDelta) override;
	virtual void OnTriggerEnter(CGameObject* other, float fTimeDelta)override;
	virtual void OnTriggerStay(CGameObject*	other, float fTimeDelta)override;

private:
	void Turn_Check();

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CBoxCollider*			m_pBoxCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	
private:
	_uint		m_iFrameNum{ 0 };
	_float		m_fTimeDelta{ 0.f };
	_bool		m_bTopdeeTurn{ false };
	_bool		m_bFallFinish{ false };

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

public:
	static CHole* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END