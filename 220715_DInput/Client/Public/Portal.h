#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPortal : public CGameObject
{
private:
	CPortal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void OnTriggerExit(CGameObject* other, float fTimeDelta);
	virtual void OnTriggerEnter(CGameObject* other, float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, float fTimeDelta, _uint eDireciton);
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	CTexture* m_pTextureCom_For_Topdee = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom_For_Topdee = nullptr;
	CTexture* m_pTextureCom_for_Toodee = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom_for_Toodee = nullptr;

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_float m_fFrame = 0.f;

	_bool m_bClear = false;
	_bool m_bNextLeel = false;
	_float m_fFrame_For_TopToo = 0.f;
};
END