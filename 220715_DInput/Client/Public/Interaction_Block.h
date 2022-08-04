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

BEGIN(Client)//PURE
class CInteraction_Block : public CGameObject
{
protected:
	CInteraction_Block(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteraction_Block(const CInteraction_Block& rhs);
	virtual ~CInteraction_Block() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	//박스의 능력활성화할지 말지
	_bool m_bAbility = true;
#pragma region About_Topdee
public:
	_bool KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta, _float3 vPreLoaderPos);//When Topdee Raise Start
	void KKK_Is_Raise(_float3 vTargetPos);//When Topdee Raising Box.
	_bool KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta, _bool bHoleCall);//When Topdee ThrowBox
	void Box_Drop_More(_float fTimeDelta);//when Topdee Throw Box & Falling
	void Box_Push_More(_float fTimeDelta,_float3 vPushFinishPos, _bool bFirstCall);//When Topdee Push Box

public:
	_bool Get_bTopdeeRaise() { return m_bTopdeeRaise; }
	void Set_bTopdeeRaise(_bool _bTopdeeRaise) { m_bTopdeeRaise = _bTopdeeRaise; }

protected:
	_bool m_bDropBox{ false };	
	_bool m_bTopdeePush{ false };
	_bool m_bTopdeeRaise{ false };
	_float3 m_vPushFinishPos{ 0.f,0.f,0.f };
	
#pragma endregion About_Topdee

protected:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Terrain_Cube*	m_pVIBufferCom = nullptr;
	CCollider*				m_pCollCom = nullptr;
	CBoxCollider*			m_pBoxCollider = nullptr;

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
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END