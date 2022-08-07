#pragma once
#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Toodee_Rect;
END

BEGIN(Client)
class CTookee : public CLandObject
{
private:
	CTookee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTookee(const CTookee& rhs);
	virtual ~CTookee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) override;
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDireciton) override;
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) override;

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	CVIBuffer_Toodee_Rect* m_pVIBufferCom = nullptr;

	_uint m_iTexIndex = 0;
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();
	CTransform* m_Toodee;
public:
	static CTookee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END