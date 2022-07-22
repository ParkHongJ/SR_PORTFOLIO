#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Toodee_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Toodee_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Toodee_Rect(const CVIBuffer_Toodee_Rect& rhs);
	virtual ~CVIBuffer_Toodee_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Toodee_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
END