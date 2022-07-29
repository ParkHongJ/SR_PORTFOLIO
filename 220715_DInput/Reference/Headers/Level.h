#pragma once

/* 클라이언트에서 생성할 레벨들이 반드시 상속받아야 할 부코클래스다. */

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;
public:
	//When we Ready_Layer to Object, We can define Information What you want. 
	typedef struct IntializeDesc {
		list<_float3> vHoleList;
		list<_float3> vSpikeList;
		_float3 vTopdeePos;
	}INITDESC;

	INITDESC m_tInit;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;


public:
	virtual void Free() override;
};

END