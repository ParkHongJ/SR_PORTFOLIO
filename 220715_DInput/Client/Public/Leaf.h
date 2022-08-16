#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END


BEGIN(Client)
class CLeaf final : public CGameObject
{
private:
	CLeaf(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLeaf(const CLeaf& rhs);
	virtual ~CLeaf() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CShader* m_pShaderCom = nullptr;

public:
	static CLeaf* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

public:
	void SetDirection(  );

private:
	
	_uint m_iTexture = 0;
	_float3 m_vDir;
	_float m_fAngle = 0.f;
	_float m_fSpeed = 2.5f;

};
END

