#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CCloud : public CGameObject
{
private:
	CCloud(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCloud(const CCloud& rhs);
	virtual ~CCloud() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();
public:
	_float3 Lerp(_float3 vPos, _float3 vTargetPos, _float fTimeDelta);

public:
	static CCloud* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
private:
	_float m_fFrame = 0.f;
	_float3 m_vToodeePos;
	_float3 m_vTopdeePos;
};
END
