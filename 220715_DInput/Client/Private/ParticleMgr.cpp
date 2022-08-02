#include "stdafx.h"
#include "..\Public\ParticleMgr.h"
#include "GameInstance.h"
#include "Particle.h"
IMPLEMENT_SINGLETON(CParticleMgr)


CParticleMgr::CParticleMgr()
{
}

HRESULT CParticleMgr::Initialize(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	for (int i = 0; i < 100; i++)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Particle"),
			iNumLevel, L"Layer_Particle")))
			return E_FAIL;
	}

	m_Particles = (pGameInstance->GetLayer(iNumLevel, L"Layer_Particle"));
	
	for (auto& iter = m_Particles->begin(); iter != m_Particles->end(); ++iter)
	{
		(*iter)->SetActive(FALSE);
	}

	/*auto& iter = (*m_Particles).begin();

	for (_uint i = 0; i < (*m_Particles).size(); ++i)
	{
		Safe_AddRef(*iter);
		iter++;
	}*/
	Safe_Release(pGameInstance);
	return S_OK;
}

void CParticleMgr::Free()
{
	m_Particles = nullptr;
}

HRESULT CParticleMgr::ReuseObj(_uint iNumLevel, const _float3& vPos, const _float3& vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Particles->begin(); iter != m_Particles->end(); ++iter)
	{
		//오브젝트가 죽었다면 
		//사용이 준비 되었다면
		//m_bActive로 판단함
		if (!(*iter)->IsActive())
		{
			//살려주고
			(*iter)->SetActive(TRUE);
			//위치셋팅
			CTransform* pTransform = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
			if (pTransform == nullptr)
			{
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//방향셋팅
			dynamic_cast<CParticle*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return S_OK;
		}
 		if (iter == m_Particles->end())
		{
			MSG_BOX(L"파티클 다썼음");
		}
	}
	Safe_Release(pGameInstance);
	return S_OK;
}
