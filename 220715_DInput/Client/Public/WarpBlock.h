#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction_Block.h"

BEGIN(Client)
class CWarpBlock final : public CInteraction_Block
{
public:
	enum WarpDir { WD_UP, WD_DOWN, WD_LEFT, WD_RIGHT, WD_END };
	WarpDir Get_WD(void) { return m_eWarpDir; }

	static _uint m_iWBCount;

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

private:
	_uint m_iWBNum;
	_uint m_iTextureNum =  0 ;
	WarpDir m_eWarpDir = WD_END;

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