#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CVIBuffer_Rect;
class CTransform;
END


BEGIN(Client)
class CKey : public CGameObject
{
private:
	CKey(LPDIRECT3DDEVICE9 pGraphic_Device);
	CKey(const CKey& rhs);
	virtual ~CKey() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerStay(CGameObject* other, _float fTimeDelta, _uint eDirection) override;
private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

public:
	static CKey* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_float m_fFrame = 0.f;

	//열쇠를 먹으면 없어지는 박스리스트 모음
	list<class CGameObject*>* m_pBoxList;
};
END