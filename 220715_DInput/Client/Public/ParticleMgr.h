#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CParticleMgr final : public CBase
{
	DECLARE_SINGLETON(CParticleMgr);
private:
	CParticleMgr();
	virtual ~CParticleMgr() = default;

public:
	HRESULT Initialize(_uint iNumLevel);
	HRESULT ReuseObj(_uint iNumLevel, const _float3& vPos, const _float3& vDir);
public:
	virtual void Free() override;

private:
	list<class CGameObject*>* m_Particles;

};
END