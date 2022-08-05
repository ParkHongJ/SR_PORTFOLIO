#include "stdafx.h"
#include "..\Public\ParticleMgr.h"
#include "GameInstance.h"
#include "Particle.h"
#include "Bullet.h"
#include "Particle_Button.h"
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
		MSG_BOX(L"��ƼŬ �̹� ����");
	}
	if (m_Bullets != nullptr)
	{
		MSG_BOX(L"�Ѿ� �̹� ����");
	}
	/*=============
	===Particles===
	=============*/
	_uint iLevel = iNumLevel;
	for (int i = 0; i < 200; i++)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(
			TEXT("Prototype_GameObject_Particle"),
			iNumLevel, L"Layer_Particle", &iLevel)))
			MSG_BOX(L"��ƼŬ ���� ����");
	}

	m_Particles = pGameInstance->GetLayer(iNumLevel, L"Layer_Particle");

	if (m_Particles == nullptr)
	{
		int a = 10;
	}
	//��ƼŬ ��Ȱ��ȭ ���·� ����
	for (auto& iter = m_Particles->begin(); iter != m_Particles->end(); ++iter)
	{
		(*iter)->SetActive(FALSE);
	}

	/*=============
	====Bullets====
	=============*/
	for (int i = 0; i < 30; i++)
	{
		//�Ѿ� ����
		if (FAILED(pGameInstance->Add_GameObjectToLayer(
			TEXT("Prototype_GameObject_Bullet"),
			iNumLevel, L"Layer_Bullet", &iLevel)))
			MSG_BOX(L"�Ѿ� ���� ����");
	}

	//�Ѿ� �޾ƿ���
	m_Bullets = pGameInstance->GetLayer(iNumLevel, L"Layer_Bullet");

	if (m_Bullets == nullptr)
	{
		int a = 1;
	}
	//�Ѿ� ��Ȱ��ȭ ���·� ����
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
			iNumLevel, L"Layer_Particle_Button")))
			return E_FAIL;
	}

	m_Buttons = pGameInstance->GetLayer(iNumLevel, L"Layer_Particle_Button");

	if (m_Buttons == nullptr)
	{
		int a = 10;
	}
	//��ƼŬ ��Ȱ��ȭ ���·� ����
	for (auto& iter = m_Buttons->begin(); iter != m_Buttons->end(); ++iter)
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
	default:
		MSG_BOX(L"�߸��� ��ƼŬ Ÿ���Դϴ�.");
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
		//������Ʈ�� �׾��ٸ� 
		//����� �غ� �Ǿ��ٸ�
		//m_bActive�� �Ǵ���
		if (!(*iter)->IsActive())
		{
			//����ְ�
			(*iter)->SetActive(TRUE);
			//��ġ����
			CTransform* pTransform = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
			if (pTransform == nullptr)
			{
				Safe_Release(pGameInstance);
				return;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//�������
			//dynamic_cast<CBullet*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		if (iter == m_Bullets->end())
		{
			MSG_BOX(L"��ƼŬ �ٽ���");
		}
	}
	Safe_Release(pGameInstance);
}

void CParticleMgr::CreateParticle(_uint iNumLevel, const _float3& vPos, const _float3& vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Particles->begin(); iter != m_Particles->end(); ++iter)
	{
		//������Ʈ�� �׾��ٸ� 
		//����� �غ� �Ǿ��ٸ�
		//m_bActive�� �Ǵ���
		if (!(*iter)->IsActive())
		{
			//����ְ�
			(*iter)->SetActive(TRUE);
			//��ġ����
			CTransform* pTransform = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
			if (pTransform == nullptr)
			{
				Safe_Release(pGameInstance);
				return;
			}
			pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//�������
			dynamic_cast<CParticle*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		if (iter == m_Particles->end())
		{
			MSG_BOX(L"��ƼŬ �ٽ���");
		}
	}
	Safe_Release(pGameInstance);
}

void CParticleMgr::CreateButton(_uint iNumLevel, const _float3 & vPos, const _float3 & vDir)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (auto& iter = m_Particles->begin(); iter != m_Particles->end(); ++iter)
	{
		//������Ʈ�� �׾��ٸ� 
		//����� �غ� �Ǿ��ٸ�
		//m_bActive�� �Ǵ���
		if (!(*iter)->IsActive())
		{
			//����ְ�
			(*iter)->SetActive(TRUE);
			//��ġ����
			CTransform* pTransform = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
			if (pTransform == nullptr)
			{
				Safe_Release(pGameInstance);
				return;
			}
			pTransform->Set_Scale(_float3(0.7f, 0.7f, 0.7f));
			pTransform->Set_State(CTransform::STATE_POSITION, vPos);
			//�������
			dynamic_cast<CParticle_Button*>(*iter)->SetDirection(vDir);
			Safe_Release(pGameInstance);
			return;
		}
		if (iter == m_Particles->end())
		{
			MSG_BOX(L"��ƼŬ �ٽ���");
		}
	}
	Safe_Release(pGameInstance);
}
