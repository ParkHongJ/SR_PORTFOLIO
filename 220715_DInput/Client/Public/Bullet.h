#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CBoxCollider;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBullet final : public CGameObject
{
public:
	typedef enum Direction
	{
		DOWN = 0,
		RIGHT,
		LEFT,
		UP
	}DIRECTION;
	typedef struct BulletDesc
	{
		DIRECTION eDir; // 방향
		_float3 vPos; // 시작위치
		_float3 vDir;

	}BULLET_DESC;
private:
	CBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CBoxCollider*			m_pBoxCollider = nullptr;
	CCollider*				m_pCollider = nullptr;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();
	DIRECTION m_eDir = RIGHT;
	_float m_fSpeed = 6.5f;
	_float m_fFrame;
	_float3 m_vDir;

public:
	void SetUp(void* pArg);
	void SetDirection(_float3 _vDir);

public:
	virtual void OnTriggerStay(CGameObject* other, _float fTimeDelta, _uint eDirection) override;

public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END