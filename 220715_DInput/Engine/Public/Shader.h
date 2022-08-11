#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader(const CShader& rhs);
	virtual ~CShader() = default; 

public:
	virtual HRESULT Initialize_Prototype(const _tchar * pShaderFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);
	HRESULT End();
	HRESULT Set_RawValue(D3DXHANDLE hHandle, const void * pData, _uint iSize);
	HRESULT Set_Texture(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture);

private:
	LPD3DXEFFECT			m_pEffect = nullptr;

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END