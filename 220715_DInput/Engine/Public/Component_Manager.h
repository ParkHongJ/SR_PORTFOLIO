#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain_Cube.h"
#include "VIBuffer_Topdee.h"
#include "VIBuffer_Toodee_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_LayDown.h"
BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

private:
	_uint												m_iNumLevels = 0;

	map<const _tchar*, class CComponent*>*				m_pPrototypes = nullptr;
	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	class CComponent* Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END