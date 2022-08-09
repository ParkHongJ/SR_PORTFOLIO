#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction_Block.h"

BEGIN(Client)
class CWarpBlock final : public CInteraction_Block
{
private:
	enum WARP_DIRECTION { WARP_TOP, WARP_RIGHT, WARP_DOWN, WARP_LEFT, WARP_END };
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
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta);

public:
	WARP_DIRECTION Get_WarpDirection() { return m_eWarpDirection; }

private:
	_uint m_iTextureNum =  0 ;
	WARP_DIRECTION m_eWarpDirection = WARP_END ;

	CWarpBlock* pOtherWarpBlock = nullptr;

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