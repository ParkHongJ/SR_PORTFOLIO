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
	_bool							m_isCloned = false;
	_bool							m_bEnabled = true;
public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
	void SetEnabled(_bool _bEnabled) { m_bEnabled = _bEnabled; }
	

public:
	HRESULT SetOwner(class CGameObject* _pOwner);
	class CGameObject* GetOwner() {	return m_pOwner; }
protected:
	class CGameObject* m_pOwner = nullptr;
};

END