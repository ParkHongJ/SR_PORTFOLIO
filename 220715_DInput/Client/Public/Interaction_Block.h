#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CBoxCollider;
class CVIBuffer_Terrain_Cube;
END

BEGIN(Client)//PURE
class CInteraction_Block : public CGameObject
{
protected:
	enum Shader { SHADER_DEFAULT, SHADER_INHOLE, SHADER_WARP };
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
	Shader m_eShaderSelect = SHADER_DEFAULT;

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
	_bool Get_bDropFinish(){ return m_bDropFinish; }
	void MakeSound(_tchar * pTag, _uint ID, _float Volum)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlayEffect(pTag, ID, Volum);

		Safe_Release(pGameInstance);
	}
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
protected:
	_bool m_bDropBox{ false };	
	_bool m_bTopdeePush{ false };
	_bool m_bTopdeeRaise{ false };
	_bool m_bDropFinish{ false };//완전히 떨어져서 쓸모없어진거
	_float3 m_vPushFinishPos{ 0.f,0.f,0.f };
	_float m_fSoundTimeDelta{ 0.f };
#pragma endregion About_Topdee

protected:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Terrain_Cube*	m_pVIBufferCom = nullptr;
	CCollider*				m_pCollCom = nullptr;
	CBoxCollider*			m_pBoxCollider = nullptr;
	CShader*				m_pShaderCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END