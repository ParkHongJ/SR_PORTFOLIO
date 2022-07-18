#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CCollider : public CComponent
{
public:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	HRESULT Render();
	void SetRadius(_float _fRadius) { m_fRadius = _fRadius; }
	const _float GetRadius() { return m_fRadius; }
	//나중에 박스,원, 캡슐 등으로 바뀔수있어서 따로뺌
protected:
	_float3 m_vPos;

	//나중에 박스,원, 캡슐 등으로 바뀔수있어서 따로뺌
private:
	_float m_fRadius = 1.f;
	LPD3DXMESH m_pSphere = nullptr;
	
};

END