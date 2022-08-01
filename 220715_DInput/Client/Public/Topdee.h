#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CLayer;
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CComponent;
class CBoxCollider;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTopdee final : public CLandObject
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

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta);

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CBoxCollider*			m_pBoxCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

	CVIBuffer_Rect*			m_pBuffer_PreLoader_Com = nullptr;
	CTransform*				m_pTransform_PreLoader_Com = nullptr;
	CRenderer*				m_pRenderer_PreLoader_Com = nullptr;
	CTexture*				m_pTexture_PreLoader_Com = nullptr;
private:
	_uint	m_iFrame{ 4 }, m_iFirstFrame{ 4 };
	_bool	m_bMoveFrame{ false };
	TOPDEE_DIRECTION m_eCurDir{ DIR_END };

private:
	TOPDEE_STATE	m_eCurState{ STATE_IDLE };
	_bool	m_bPress{ false };
	_bool	m_bTurn{ false };
	
private:
	HRESULT SetUp_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Set_ColliderState();
	HRESULT Reset_ColliderState();

private:
	void	KKK_FindBox(_float fTimeDelta);
	void	KKK_IsRaise(_float fTimeDelta, _char KKK_NotOverride);
	void	KKK_DropBox(_float fTimeDelta);

private:
	void Move_Frame(const TOPDEE_DIRECTION& _eInputDirection);
	void Not_My_Turn_Texture();
	void Go_Lerp(_float fTimeDelta);
	void Topdee_PreLoader_Pos_Mgr();

private:
	CLevel::IntializeDesc* m_tInitializeDesc;
	list<class CGameObject*>* KKK_m_pBoxList;
	
	CGameObject* m_pRaiseObject = nullptr;
	_float3 m_vTargetDir{0.f,0.f,0.f};
	_float3 m_vBoxDropPos{ -1.f, -1.f, -1.f };
	_float m_fRaising_Box_DelayTimer{ 0.f };
	_float m_fCollision_Box_DelayTimer{ 0.f };

	_float m_MyTurnY{ 0.8f };
	_float m_NotMyTurnY{ 0.1f };
	_float m_OriPreLoaderY{ 0.8f };

public:
	static CTopdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END