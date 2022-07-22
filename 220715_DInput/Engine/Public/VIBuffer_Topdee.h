#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Topdee final : public CVIBuffer
{
private:
	CVIBuffer_Topdee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Topdee(const CVIBuffer_Topdee& rhs);
	virtual ~CVIBuffer_Topdee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Topdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END