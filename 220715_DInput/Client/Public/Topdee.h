#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CVIBuffer_Rect;
END
//topdee �ؽ��� ���� 
//left
// 0, 1 Topdee idle
// 2, 3 Topdee Left
// 4, 5 Topdee Up

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

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
private:
	_uint	m_iFrame{ 4 }, m_iFirstFrame{ 4 };
	_bool	m_bMoveFrame{ false };
	TOPDEE_DIRECTION m_eCurDir{ DIR_END };

private:
	void	Jumping();
	TOPDEE_STATE	m_eCurState{ STATE_IDLE };
	_uint	m_iCurJumpPower;
	_uint	m_iOriginJumpPower{5};
	_bool	m_bJump{ false };
	_bool	m_bDown{ false };
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	void Move_Frame(const TOPDEE_DIRECTION& _eInputDirection);
private:
	HRESULT SetUp_Components();

public:
	static CTopdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END