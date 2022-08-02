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

class CBlock final : public CGameObject
{

private:
	CBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock(const CBlock& rhs);
	virtual ~CBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta);
#pragma region About_Topdee
public:
	_bool KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta, _float3 vPreLoaderPos);//When Topdee Raise Start
	void KKK_Is_Raise(_float3 vTargetPos);//When Topdee Raising Box.
	_bool KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta, _bool bHoleCall);//When Topdee ThrowBox
	void Box_Drop_More(_float fTimeDelta);//when Topdee Throw Box & Falling
	void Box_Push_More(_float fTimeDelta,_float3 vPushFinishPos, _float3 vPushDir);//When Topdee Push Box
	void Box_Push_Find_A_Place();//When Topdee Push and Find Right Place
	_bool m_bDropBox{ false };	
	_bool m_bTopdeePush{ false };
	_float3 m_vPushDir{ 0.f,0.f,0.f };
	_float3 m_vPushFinishPos{ 0.f,0.f,0.f };
#pragma endregion About_Topdee

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Terrain_Cube*	m_pVIBufferCom = nullptr;
	CCollider*				m_pCollCom = nullptr;
	CBoxCollider*			m_pBoxCollider = nullptr;

	_uint m_iTextureNum{ 0 };

	
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();
	void TextureSelect(const _float3& vPos);
public:
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
public:
	static CBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};


END