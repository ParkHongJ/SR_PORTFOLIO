#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "GameMgr.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Toodee_Rect;
END

BEGIN(Client)
class CToodee final : public CGameObject
{
public:
	typedef struct PlayerInfo
	{
		_float3 vPos;
		_uint iNumLevel;
	}PLAYER_INFO;

private:
	enum DIR { TOODEE_LEFT, TOODEE_RIGHT, TOODEE_JUMP, TOODEE_PORTAL, TOODEE_DEAD, TOODEE_IDLE, TOODEE_END };

private:
	CToodee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CToodee(const CToodee& rhs);
	virtual ~CToodee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) override;
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDireciton) override;
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) override;

public:
	//Edit Hong. 기존 이펙트생성 함수로 변경 및 투키 조종함수 추가
	void CreateEffect();
	void SetStateTookee();

private:
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTexture* m_pTextureCom_Died = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Toodee_Rect* m_pVIBufferCom = nullptr;

private:
	_float3 m_fStartPos = { 0.f, 0.f, 0.f };
	DIR m_eToodeeDir = TOODEE_END;
	DIR m_eCurruntDir = TOODEE_END;
	_uint m_iTexIndexDied = 0;
	_uint m_iTexIndex = 0;
	_float m_fFrame = 0.f;
	_uint m_iMinFrame = 0;
	_uint m_iMaxFrame = 0;

	_float m_MoveSpeed = 0.f;

	_bool m_bJump = false;
	_bool m_bDrop = false;
	_float m_fJumpPower = 19.f;
	_float m_vGravityPower = 0.f;
	_float m_fJumpTime = 0.f;
	_float m_fMaxJumpTime = 0.6f;
	_float m_fForWarpPower = 0.f;
	_float m_fForWarpTime = 0.f;
	_float m_fWarpTimer = 0.f;
	_float m_fTempTimer = 0.f;
	_float m_fTimeD = 0.f;
	_float m_fDrop_Endline = 0.f;

	_float m_fSoundTimeDelta = 0.f;

	_bool m_bPortal = false;
	_bool m_bDiedEff = false;
	_bool m_bDiedSnd = false;

	_float m_fTimedelta;
	_float tick;
	//Edit Hong
	_bool m_bInput = true;
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

	void MakeSound(_tchar* pTag, _uint ID, _float Volum);



public:
	static CToodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END