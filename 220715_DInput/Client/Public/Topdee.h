#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CLayer;
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CComponent;
class CBoxCollider;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTopdee final : public CLandObject
{
public:
	typedef struct PlayerInfo
	{
		_float3 vPos;
		_uint iNumLevel;
	}PLAYER_INFO;
public:
	enum TOPDEE_DIRECTION { DIR_LEFT, DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_END };
	enum TOPDEE_STATE	 {STATE_IDLE, STATE_JUMP, STATE_CLEAR, STATE_DEAD, STATE_END};

private:
	CTopdee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTopdee(const CTopdee& rhs);
	virtual ~CTopdee() = default;
private:
	void DeadCheck(_float fTimeDelta);// GameOver
	void RayCastedCheck();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

#pragma region OnCollision
public:
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDirection);
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta);
#pragma endregion OnCollision

#pragma region Component
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CBoxCollider*			m_pBoxCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

	CVIBuffer_Rect*			m_pBuffer_PreLoader_Com = nullptr;
	CTransform*				m_pTransform_PreLoader_Com = nullptr;
	CRenderer*				m_pRenderer_PreLoader_Com = nullptr;
	CTexture*				m_pTexture_PreLoader_Com = nullptr;
#pragma endregion Component

#pragma region Variable
private:
	_bool	m_bDead{ false }, m_bClear{ false };
	_uint	m_iFrame{ 4 }, m_iFirstFrame{ 4 };
	_bool	m_bMoveFrame{ false };
	TOPDEE_DIRECTION m_eCurDir{ DIR_END };
	TOPDEE_STATE	m_eCurState{ STATE_IDLE };
	_bool	m_bPress{ false };
	_bool	m_bTurn{ false };
	_bool	m_bPortal{ false };

	LEVEL m_eHoleLevel{ LEVEL_END }; // for Remember HoleLevel

	_float3 m_vTargetDir{ 0.f,0.f,0.f };
	_float3 m_vBoxDropPos{ -1.f, -1.f, -1.f };
	_float m_fRaising_Box_DelayTimer{ 0.f };
	_float m_fDeadTimer{ 0.f };

	_float m_MyTurnY{ 0.5f };
	_float m_NotMyTurnY{ 0.1f };

	_uint m_iMoveFrameAdd{ 0 };
	_bool m_bMoveFrameAdd{ false };
	
	list<class CGameObject*>* KKK_m_pBoxList = nullptr;
	CGameObject* m_pRaiseObject = nullptr;
	_float m_fPushBoxDelayTimer{ 0.f };
	_bool	m_bPushBox{ false };
	_bool	m_bDropBox{ false };

	//Edit Hong
	_bool m_bTookeeMove = true;
	_bool m_bInput = true;
#pragma endregion Variable

private:
	_float m_fSoundTimeDelta = 0.f;
	_bool m_bDiedSnd = false;

#pragma region RenderState
private:
	void MakeSound(_tchar* pTag, _uint ID, _float Volum);
	
	HRESULT SetUp_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Set_ColliderState();
	HRESULT Reset_ColliderState();
#pragma endregion RenderState

#pragma region About_Box
private:
	void	KKK_FindBox(_float fTimeDelta);//ㄷ던질박스를 찾는
	void	KKK_IsRaise(_float fTimeDelta, _char KKK_NotOverride);//들고이쓴
	void	KKK_DropBox(_float fTimeDelta);//박스를 던진
	void	TopdeeIsPushed(const _float3 _vOtherPos, _float fTimeDelta);//박스가 탑디를 조금 밀쳐내는
	void	FindCanPushBoxes(_float3 _vNextBoxPos, _float3 vPushDir, _uint& iCountReFunc, list<CGameObject*>& PushList,_bool& bCanPush);//박스앞에박스있는지 밀때체크
	void	Rotate_WarpBlock();
#pragma endregion About_Box

private:
	void Move_Frame(const TOPDEE_DIRECTION& _eInputDirection);
	void Not_My_Turn_Texture();
	void Go_Lerp(_float fTimeDelta);
	void Topdee_PreLoader_Pos_Mgr();
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
	static CTopdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END