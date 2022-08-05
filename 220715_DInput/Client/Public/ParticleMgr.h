#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

/* 오브젝트가 많이 필요한 것들을 관리하는 클래스. 오브젝트 풀링 사용*/

BEGIN(Client)
class CParticleMgr final : public CBase
{
	DECLARE_SINGLETON(CParticleMgr);
public:
	enum PARTICLE_TYPE
	{
		PARTICLE,
		BULLET,
		BUTTON,
		PARTICLE_END
	};
private:
	CParticleMgr();
	virtual ~CParticleMgr() {
		m_Particles = nullptr;
		m_Bullets = nullptr;
		m_Buttons = nullptr;
	};

public:
	HRESULT Initialize(_uint iNumLevel);
	HRESULT ReuseObj(_uint iNumLevel, const _float3& vPos, const _float3& vDir, PARTICLE_TYPE eType);
private:
	void CreateBullet(_uint iNumLevel, const _float3& vPos, const _float3& vDir);
	void CreateParticle(_uint iNumLevel, const _float3& vPos, const _float3& vDir);
	void CreateButton(_uint iNumLevel, const _float3& vPos, const _float3& vDir);
public:
	virtual void Free() override;

private:
	list<class CGameObject*>* m_Particles = nullptr;
	list<class CGameObject*>* m_Bullets = nullptr;
	list<class CGameObject*>* m_Buttons = nullptr;
};
END