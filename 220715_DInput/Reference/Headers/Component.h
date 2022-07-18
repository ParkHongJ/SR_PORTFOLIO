#pragma once

#include "Base.h"

/*  각종 컴포넌트들의 부모가 되는 클래스다. */

BEGIN(Engine)

class ENGINE_DLL CComponent  abstract : public CBase
{
protected:
	CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = nullptr;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
	
public:
	void SetOwner(class CGameObject* _pOwner);
protected:
	class CGameObject* m_pOwner = nullptr;
};

END