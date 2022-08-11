#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END


BEGIN(Client)
class CParticle_Button final : public CGameObject
{
private:
	CParticle_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticle_Button(const CParticle_Button& rhs);
	virtual ~CParticle_Button() = default;
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
public:
	static CParticle_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

public:
	void SetDirection(const _float3& vDir);
	void ButtonDead();

private:
	_float m_fFrame = 0.f;
	_float3 m_vDir;
	_float m_fSpeed = 2.5f;
	_float m_fTimer = 0.f;
	_uint m_iRand;
	_bool m_bCheck;
};
END

