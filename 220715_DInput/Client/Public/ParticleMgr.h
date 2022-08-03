#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

/* ������Ʈ�� ���� �ʿ��� �͵��� �����ϴ� Ŭ����. ������Ʈ Ǯ�� ���*/

BEGIN(Client)
class CParticleMgr final : public CBase
{
	DECLARE_SINGLETON(CParticleMgr);
public:
	enum PARTICLE_TYPE
	{
		PARTICLE,
		BULLET,
		PARTICLE_END
	};
private:
	CParticleMgr();
	virtual ~CParticleMgr() = default;

public:
	HRESULT Initialize(_uint iNumLevel);
	HRESULT ReuseObj(_uint iNumLevel, const _float3& vPos, const _float3& vDir, PARTICLE_TYPE eType);
private:
	void CreateBullet(_uint iNumLevel, const _float3& vPos, const _float3& vDir);
	void CreateParticle(_uint iNumLevel, const _float3& vPos, const _float3& vDir);
public:
	virtual void Free() override;

private:
	list<class CGameObject*>* m_Particles;
	list<class CGameObject*>* m_Bullets;
};
END