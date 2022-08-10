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
class CParticle_Spark final : public CGameObject
{
private:
	CParticle_Spark(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticle_Spark(const CParticle_Spark& rhs);
	virtual ~CParticle_Spark() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Make_Line(const _float3& vStartPoint, const _float3& vEndPoint);
	_float3 Lerp(_float3 vPos, _float3 vTargetPos, _float fTimeDelta);
	void Go_To_Player(_float fTimeDelta);
	void Reset_State();

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
	static CParticle_Spark* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_float m_fSpeed = 8.f;

	_float3 m_vStartPoint = { 0.f,0.f,0.f };
	_float3 m_vEndPoint = { 0.f,0.f,0.f };
	_bool m_bLineStart = false;

};
END

