#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Toodee_Rect;
END

BEGIN(Client)
class CTookee final : public CGameObject
{
public:
	enum STATE { TOOKEE_LEFT, TOOKEE_RIGHT, TOOKEE_UP, TOOKEE_DOWN, TOOKEE_JUMP, TOOKEE_IDLE, TOOKEE_END };

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


	_bool m_bJump = false;
	_bool m_bDrop = false;
	_float m_fJumpPower = 17.f;
	_float m_fJumpTime = 0.f;
	_float m_fMaxJumpTime = 0.6f;
	_float m_fDrop_Endline = 0.f;

	_uint m_ePreMode = 0;
	_uint m_eCurMode = 0;
	//투키의 현재상태, TOODE, TOPDEE 분리할까? 말까?
	STATE m_eCurState;
	_float3 m_fScale = {1.f,1.f,1.f};
	_float m_fSpeed = 5.f;
//TOODEE일 경우 : X축 이동과 점프, 중력이 필요함
//TOPDEE일 경우 : X,Z축 이동이 필요함 홀과 충돌필요
public:
	/*===============
	==TooKeeControl==
	===============*/
	void Move(STATE _eState, _float fTimeDelta);
	void Jump(_float fTimeDelta);
	void CreateEffect();
	void SetState(STATE _eState) { m_eCurState = _eState; }
	void SetJump() { m_bJump = true; }
	void SetSpeed(_float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetScale(_float3 _vScale);
	void SetPosition(_float fTimeDelta, _float3 vDir);
public:
	/* For Topdee*/
	void	TopdeeIsPushed(const _float3 _vPos);
	void	FindCanPushBoxes(_float3 _vNextBoxPos, _float3 vPushDir, _uint& iCountReFunc, list<CGameObject*>& PushList, _bool& bCanPush);//박스앞에박스있는지 밀때체크

private:
	//For Topdee
	list<class CGameObject*>* m_pBoxList = nullptr;
	_bool	m_bPushBox = false;
	_float m_fPushBoxDelayTimer = 0.f;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();
public:
	static CTookee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END