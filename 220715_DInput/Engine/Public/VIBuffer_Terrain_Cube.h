#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain_Cube final : public CVIBuffer
{
private:
	CVIBuffer_Terrain_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain_Cube(const CVIBuffer_Terrain_Cube& rhs);
	virtual ~CVIBuffer_Terrain_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Terrain_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
END