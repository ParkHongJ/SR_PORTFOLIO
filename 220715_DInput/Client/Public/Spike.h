#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CBoxCollider;
class CTransform;
class CRenderer;
class CCollider;
class CTexture;
END

BEGIN(Client)
class CSpike : public CGameObject
{
private:
	CSpike(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpike(const CSpike& rhs);
	virtual ~CSpike() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerExit(CGameObject* other);
	virtual void OnTriggerEnter(CGameObject* other);
	virtual void OnTriggerStay(CGameObject*	other);

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
	//void SpikeAngleMgr();

private:
	HRESULT SetUp_Components();

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
public:
	static CSpike* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
	//Current To Target Tick, maxDistanceDelta(fTimeDelta * Speed)
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
	_float3 m_vTopdeePos;
	_float3 m_vToodeePos;
	_float m_fSpeed = 13.f;//�̵��ӵ�
	_uint m_iFrame = 0;
	_float m_fTime = 0.f;
};
END