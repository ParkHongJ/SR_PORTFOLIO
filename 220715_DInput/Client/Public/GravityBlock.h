#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameMgr.h"
#include "Interaction_Block.h"

BEGIN(Client)
class CGravityBlock : public CInteraction_Block
{

private:
	CGravityBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGravityBlock(const CGravityBlock& rhs);
	virtual ~CGravityBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDirection)override;

	void	UpdateGravitiy(_float fTimeDelta);
	_float3 Lerp(_float3 vPos, _float3 vTargetPos, _float fTimeDelta)
	{
		//a + (b - a) * t.
		return vPos + (vTargetPos - vPos) * fTimeDelta;
	}
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();
private:
	_bool m_bOnAir;
	_bool m_bOnBlock;
	_bool m_bAbility = true;
	CGameMgr::GAMEMODE m_ePreMode;

public:
	static CGravityBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

