#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) {};
	//virtual void OnTriggerStay(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerStay(CGameObject* other, _float fTimeDelta, _uint eDirection) {};
	virtual void OnTriggerExit(CGameObject*  other, _float fTimeDelta) {};

public:
	virtual bool KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta,_float3 vPreLoaderPos) { return true; };
	virtual void KKK_Is_Raise(_float3 vTargetPos) {};
	virtual _bool KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta,_bool bHoleCall) { return true; };

protected:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
	
protected: /* 객체에게 추가된 컴포넌트들을 키로 분류하여 보관한다. */
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, class CComponent** ppOut, CGameObject* _pOwner, void* pArg = nullptr);
	class CComponent* Get_Component(const _tchar* pComponentTag);
private:
	class CComponent* Find_Component(const _tchar* pComponentTag);
	
public:
	bool IsActive() { return m_bActive; }
	bool IsEnabled() { return m_bEnabled; }
	void SetTag(const _tchar* _tag) { m_Tag = _tag; }
	void SetEnabled(bool _bEnabled) { m_bEnabled = _bEnabled; }
	bool CompareTag(const _tchar* _tag) { 
		if (m_Tag == _tag) 
			return TRUE; 
		else return FALSE; 
	}
	bool GetEnabled() { return m_bEnabled; }

protected:
	bool m_bActive;
	const _tchar* m_Tag;
	bool m_bEnabled = true;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END