#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMap);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _bool Picking(class CTransform* pTransform, _float3* pOut);
	virtual _float Compute_Height(_float3 vTargetPos);

private:
	_uint			m_iNumVerticesX, m_iNumVerticesZ;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END