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
class CButton : public CGameObject
{
private:
	CButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CButton(const CButton& rhs);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerStay(CGameObject* other, _float fTimeDelta, _uint eDirection) override;
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) override;
	HRESULT GetBoxList();
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
	static CButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	//열쇠를 먹으면 없어지는 박스리스트 모음 ->버튼으로 교체해야함
	list<class CGameObject*>* m_pBoxList;
};
END