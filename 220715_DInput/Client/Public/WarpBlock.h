#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction_Block.h"

BEGIN(Client)
class CWarpBlock final : public CInteraction_Block
{
public:
	enum DIRECTION { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_END };
	
private:
	CWarpBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWarpBlock(const CWarpBlock& rhs);
	virtual ~CWarpBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDirection) override;

public:
	void SetPartner(CWarpBlock* pPartner) { 
		m_pPartner = pPartner; 
	}
	_float3 GetTeleportPos() { return m_vTeleportPos; }
	DIRECTION GetDir() { return m_eDir; }
	DIRECTION GetPartnerDir() { return m_pPartner->GetDir(); }

private:
	_uint m_iWBNum;
	_uint m_iTextureNum =  0 ;
	DIRECTION m_eDir = DIR_END;
	
	//텔레포트할 위치
	_float3 m_vTeleportPos;

	//포탈블럭과 m_vTeleportPos 사이의 거리
	_float m_fWarpDistance;

	//깐부
	CWarpBlock* m_pPartner = nullptr;
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

public:
	static CWarpBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END