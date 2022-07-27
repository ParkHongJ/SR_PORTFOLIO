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
class CComponent;
class CLayer;
END

BEGIN(Client)

class CTopdee final : public CGameObject
{
public:
	enum TOPDEE_DIRECTION { DIR_LEFT, DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_END };
	enum TOPDEE_STATE	 {STATE_IDLE, STATE_JUMP, STATE_CLEAR, STATE_DEAD, STATE_END};

private:
	CTopdee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTopdee(const CTopdee& rhs);
	virtual ~CTopdee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerExit(CGameObject* other);
	virtual void OnTriggerEnter(CGameObject* other);
	virtual void OnTriggerStay(CGameObject*	other);
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CBoxCollider*			m_pBoxCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	
private:
	_uint	m_iFrame{ 4 }, m_iFirstFrame{ 4 };
	_bool	m_bMoveFrame{ false };
	TOPDEE_DIRECTION m_eCurDir{ DIR_END };

private:
	TOPDEE_STATE	m_eCurState{ STATE_IDLE };
	_bool	m_bPress{ false };
	_bool	m_bTurn{ false };

private:
	HRESULT Set_RenderState();
	HRESULT Set_ColliderState();
	HRESULT Reset_ColliderState();
	HRESULT Reset_RenderState();

private:
	void Topdee_Turn_Check();
	void Move_Frame(const TOPDEE_DIRECTION& _eInputDirection);
	void Not_My_Turn_Texture();
	void Go_Lerp(_float fTimeDelta);

private:
	HRESULT SetUp_Components();

private:
	void	KKK_FindBox(_float fTimeDelta);
	void	KKK_IsRaise(_float fTimeDelta,_char KKK_NotOverride);
	void	KKK_DropBox(_float fTimeDelta);
	
	list<class CGameObject*>* KKK_m_pBoxList;
	
	CGameObject* m_pRaiseObject = nullptr;
	_float3 m_vTargetDir{0.f,0.f,0.f};
	_float3 m_vBoxDropPos{ -1.f, -1.f, -1.f };
	_float m_fRaising_Box_DelayTimer{ 0.f };

	_float m_MyTurnY{ 0.5f };
	_float m_NotMyTurnY{ 0.05f };

public:
	static CTopdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END