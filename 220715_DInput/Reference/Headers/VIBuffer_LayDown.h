#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_LayDown final : public CVIBuffer
{
public:
	typedef struct RectDesc
	{
		_float3 vSize;
	}RECTDESC;
private:
	CVIBuffer_LayDown(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_LayDown(const CVIBuffer_LayDown& rhs);
	virtual ~CVIBuffer_LayDown() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_LayDown* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
private:
	RECTDESC m_RectDesc;
};

END