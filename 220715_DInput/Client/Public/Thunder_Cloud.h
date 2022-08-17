#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CBoxCollider;
class CCollider;
class CShader;
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
//==================================================RainEnd
	CTexture*	m_pTextureCom_Shadow = nullptr;
	CTransform*	m_pTransformCom_Shadow = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom_Shadow = nullptr;
	CShader* m_pShaderCom_Shadow = nullptr;
//==================================================ShadowEnd
	CBoxCollider* m_pBoxCom = nullptr;
	CCollider* m_pColliderCom = nullptr;

private:
	_float3 MoveTowards(_float3 current, _float3 target, float maxDistanceDelta)
	{
		_float3 a = target - current;
		float magnitude = D3DXVec3Length(&a);
		if (magnitude <= maxDistanceDelta || magnitude == 0.f)
		{
			return target;
		}
		return current + a / magnitude * maxDistanceDelta;
	}

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

	_float3 m_vToodeePos;
	_float3 m_vTopdeePos;

	_float3 m_vShadow_ToodeePos;
	_float3 m_vShadow_TopdeePos;

	_float m_fSndTime = 0.f;
};
END