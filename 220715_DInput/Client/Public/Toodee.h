#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "GameMgr.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Toodee_Rect;
END

BEGIN(Client)
class CToodee final : public CGameObject
{
private:
	enum DIR { TOODEE_LEFT, TOODEE_RIGHT, TOODEE_JUMP, TOODEE_PORTAL, TOODEE_DEAD, TOODEE_IDLE, TOODEE_END };

private:
	CToodee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CToodee(const CToodee& rhs);
	virtual ~CToodee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta);
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta);

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Toodee_Rect* m_pVIBufferCom = nullptr;

private:
	DIR m_eToodeeDir = TOODEE_END;
	DIR m_eCurruntDir = TOODEE_END;
	_uint m_iTexIndex = 0;
	_float m_fFrame = 0.f;

	_float m_MoveSpeed = 0.f;

	_bool m_bJump = false;
	_float m_fJumpPower = 18.f;
	_float m_fJumpTime = 0.f;
	_float m_fDrop_Endline = 0.f;

	_bool m_bPortal = false;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

public:
	static CToodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END