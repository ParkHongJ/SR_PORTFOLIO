#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return S_OK;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

// Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create());

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
	{
		Safe_Release(pPrototype);
		return S_OK;
	}

	m_Prototypes.emplace(pPrototypeTag, pPrototype);			

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObjectToLayer(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg)
{
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);

	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);

	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);

	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

CLayer * CObject_Manager::KKK_GetBoxLayer()
{
	CLayer*		pLayer = Find_Layer(6, L"Layer_Cube");
	if (pLayer == nullptr)
		return nullptr;
	else
		return pLayer;
}

CLayer * CObject_Manager::Get_Layer(const _tchar * pLayerTag, _uint iLevelIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (pLayer == nullptr)
		return nullptr;
	else
		return pLayer;
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
			{
				Pair.second->Tick(fTimeDelta);
			}
		}			
	}
	
	
}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->LateTick(fTimeDelta);
		}
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}

	m_pLayers[iLevelIndex].clear();
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);


	for (auto& Pair : m_Prototypes)	
		Safe_Release(Pair.second);

	m_Prototypes.clear();
	
}

class CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iLayerIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iLayerIndex);

}

list<class CGameObject*>* CObject_Manager::GetLayer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	//CLayer*		pLayer = Find_Layer(6, L"Layer_Cube");
	if (nullptr == pLayer)
		return nullptr;

	/*이거 수정해라*/
	return pLayer->KKK_Get_List();
}