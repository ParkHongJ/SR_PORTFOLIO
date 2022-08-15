#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction_Block.h"

BEGIN(Client)
class CElectricBlock final : public CInteraction_Block
{

private:
	CElectricBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CElectricBlock(const CElectricBlock& rhs);
	virtual ~CElectricBlock() = default;

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
	_uint m_iTextureNum = 0;
	void TextureChange();

private:
	HRESULT SetUp_Components();

public:
	static CElectricBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END