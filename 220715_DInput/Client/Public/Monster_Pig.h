#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CBoxCollider;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMonster_Pig final : public CLandObject
{
public:
	typedef struct tagBackGroundDesc
	{
		_uint		iSizeX;
		_uint		iSizeY;
	}BACKDESC;

public://enum DIRECTION { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_END };
	enum PIG_DIRECTION { DIR_LEFT, DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_END };

private:
	CMonster_Pig(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster_Pig(const CMonster_Pig& rhs);
	virtual ~CMonster_Pig() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDirection);

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CBoxCollider*			m_pBoxCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float3	m_vTopdeePos;
	_float	m_fFrame = 0.f;
	_bool	m_bTurn;
	PIG_DIRECTION m_eCurDir = DIR_END;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();
	void	UpdateGravitiy(_float fTimeDelta);

private:
	bool					m_bOnAir;
	bool					m_bOnBlock;

public:
	static CMonster_Pig* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END