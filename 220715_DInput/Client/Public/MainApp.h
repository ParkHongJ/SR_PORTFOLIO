#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CCollider;
class CGameInstance;
END


BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();	

private:
	CGameInstance*				m_pGameInstance = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CCollider*					m_pCollider = nullptr;
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

#ifdef _DEBUG
private:
	_float			m_fTimeAcc = 0.f;
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
#endif // _DEBUG

private:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Default_SamplerState();
	HRESULT Ready_Default_RenderState();

private:  /* For.Static */
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Prototype_GameObject();


public:
	static CMainApp* Create();
	virtual void Free();
};


END