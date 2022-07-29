#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Terrain_Cube;
END
BEGIN(Client)
class CKeyBlock
{
private:
	CKeyBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CKeyBlock(const CKeyBlock& rhs);
	virtual ~CKeyBlock() = default;
//public:
//	virtual HRESULT Initialize_Prototype() override;
//	virtual HRESULT Initialize(void* pArg) override;
//	virtual void Tick(_float fTimeDelta) override;
//	virtual void LateTick(_float fTimeDelta) override;
//	virtual HRESULT Render() override;
};

END