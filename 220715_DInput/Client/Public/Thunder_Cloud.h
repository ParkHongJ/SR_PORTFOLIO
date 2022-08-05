#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END


BEGIN(Client)
class CThunder_Cloud : public CGameObject
{
private:
	CThunder_Cloud(LPDIRECT3DDEVICE9 pGraphic_Device);
	CThunder_Cloud(const CThunder_Cloud& rhs);
	virtual ~CThunder_Cloud() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerStay(CGameObject* other, _float fTimeDelta, _uint eDirection) override;

private:
	CTexture* m_pTextureCom_Cloud = nullptr;
	CRenderer* m_pRendererCom_Cloud = nullptr;
	CTransform* m_pTransformCom_Cloud = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom_Cloud = nullptr;
//==================================================CloudEnd
	CTexture*	m_pTextureCom_Rain = nullptr;
	CTransform* m_pTransformCom_Rain = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom_Rain = nullptr;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

public:
	static CThunder_Cloud* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_float m_fFrame_Cloud = 0.f;
	_float m_fFrame_Rain = 0.f;
};
END