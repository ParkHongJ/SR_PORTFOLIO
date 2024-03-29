#include "stdafx.h"
#include "..\Public\ParticleMgr.h"
#include "GameInstance.h"
#include "Particle.h"
#include "Bullet.h"
#include "Particle_Button.h"
#include "Particle_Warp.h"
IMPLEMENT_SINGLETON(CParticleMgr)


CParticleMgr::CParticleMgr()
{
}

HRESULT CParticleMgr::Initialize(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (m_Particles != nullptr)
	{
		MSG_BOX(L"파티클 이미 있음");
	}
	if (m_Bullets != nullptr)
	{
		MSG_BOX(L"총알 이미 있음");
	}
	_uint iLevel = iNumLevel;
	
	/*=============
	===Particles===
	=============*/
	for (int i = 0; i < 200; i++)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(
			TEXT("Prototype_GameObject_Particle"),
			iNumLevel, L"Layer_Particle", &iLevel)))
			MSG_BOX(L"파티클 생성 실패");
	}

	m_Particles = pGameInstance->GetLayer(iNumLevel, L"Layer_Particle");

	if (m_Particles != nullptr)
	{
		//파티클 비활성화 상태로 놓기
		for (auto& iter = m_Particles->begin(); iter != m_Particles->end(); ++iter)
		{
			(*iter)->SetActive(FALSE);
		}
	}
	
	/*=============
	====Bullets====
	=============*/
	for (int i = 0; i < 100; i++)
	{
		//총알 생성
		if (FAILED(pGameInstance->Add_GameObjectToLayer(
			TEXT("Prototype_GameObject_Bullet"),
			iNumLevel, L"Layer_Bullet", &iLevel)))
			MSG_BOX(L"총알 생성 실패");
	}

	//총알 받아오기
	m_Bullets = pGameInstance->GetLayer(iNumLevel, L"Layer_Bullet");


	if (m_Bullets == nullptr)
	{
		int a = 1;
	}
	//총알 비활성화 상태로 놓기
	for (auto& iter = m_Bullets->begin(); iter != m_Bullets->end(); ++iter)
	{
		(*iter)->SetActive(FALSE);
	}
	

	/*=============
	====Buttons====
	=============*/
	for (int i = 0; i < 200; i++)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(
			TEXT("Prototype_GameObject_Particle_Button"),
			iNumLevel, L"Layer_Particle_Button", &iLevel)))
			return E_FAIL;
	}

	m_Buttons = pGameInstance->GetLayer(iNumLevel, L"Layer_Particle_Button");

	if (m_Buttons == nullptr)
	{
		int a = 10;
	}
	//파티클 비활성화 상태로 놓기
	for (auto& iter = m_Buttons->begin(); iter != m_Buttons->end(); ++iter)
	{
		(*iter)->SetActive(FALSE);
	}

	/*=============
	====Warps====
	=============*/
	for (int i = 0; i < 100; i++)
	{
		//총알 생성
		if (FAILED(pGameInstance->Add_GameObjectToLayer(
			TEXT("Prototype_GameObject_Particle_Warp"),
			iNumLevel, L"Layer_Particle_Warp", &iLevel)))
			MSG_BOX(L"WarpParticle 생성 실패");
	}

	//총알 받아오기
	m_Warps = pGameInstance->GetLayer(iNumLevel, L"Layer_Particle_Warp");

	if (m_Warps == nullptr)
	{
		int a = 1;
	}
	//총알 비활성화 상태로 놓기
	for (auto& iter = m_Warps->begin(); iter != m_Warps->end(); ++iter)
	{
		(*iter)->SetActive(FALSE);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CParticleMgr::Free()
{
	m_Particles = nullptr;
	m_Bullets = nullptr;
	m_Buttons = nullptr;
	m_Warps = nullptr;
}

void CParticleMgr::EraseButton()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Buttons->begin(); iter != m_Buttons->end(); ++iter)
	{
		dynamic_cast<CParticle_Button*>(*iter)->ButtonDead();
		int a = 10;
	}
	Safe_Release(pGameInstance);
}

HRESULT CParticleMgr::ReuseObj(_uint iNumLevel, const _float3& vPos, const _float3& vDir, PARTICLE_TYPE eType)
{
	switch (eType)
	{
	case CParticleMgr::PARTICLE:
		CreateParticle(iNumLevel, vPos, vDir);
		break;
	case CParticleMgr::BULLET:
		CreateBullet(iNumLevel, vPos, vDir);
		break;
	case CParticleMgr::BUTTON:
		CreateButton(iNumLevel, vPos, vDir);
		break;
	case CParticleMgr::WARP:
		CreateWarps(iNumLevel, vPos, vDir);
		break;
	default:
		MSG_BOX(L"잘못된 파티클 타입입니다.");
		break;
	}
	
	return S_OK;
}

void CParticleMgr::CreateBullet(_uint iNumLevel, const _float3& vPos, const _float3& vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Bullets->begin(); iter != m_Bullets->end(); ++iter)
	{
		if (m_Bullets == nullptr)
		{
			break;
		}
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
				return;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//방향셋팅
			dynamic_cast<CBullet*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		if (iter == m_Bullets->end())
		{
			MSG_BOX(L"파티클 다썼음");
		}
	}
	Safe_Release(pGameInstance);
}

void CParticleMgr::CreateParticle(_uint iNumLevel, const _float3& vPos, const _float3& vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (m_Particles == nullptr)
		return;
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
				return;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//방향셋팅
			dynamic_cast<CParticle*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		if (iter == m_Particles->end())
		{
			MSG_BOX(L"파티클 다썼음");
		}
	}
	Safe_Release(pGameInstance);
}

void CParticleMgr::CreateButton(_uint iNumLevel, const _float3 & vPos, const _float3 & vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Buttons->begin(); iter != m_Buttons->end(); ++iter)
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
				return;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//방향셋팅
			dynamic_cast<CParticle_Button*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		if (iter == m_Buttons->end())
		{
			MSG_BOX(L"파티클 다썼음");
		}
	}
	Safe_Release(pGameInstance);
}

void CParticleMgr::CreateWarps(_uint iNumLevel, const _float3 & vPos, const _float3 & vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Warps->begin(); iter != m_Warps->end(); ++iter)
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
				return;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//방향셋팅
			dynamic_cast<CParticle_Warp*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		else
			continue;
		if (iter == m_Particles->end())
		{
			MSG_BOX(L"warp파티클 다썼음");
		}
	}
	Safe_Release(pGameInstance);
}
