#include "..\Public\LandObject.h"

#include "Object_Manager.h"
#include "VIBuffer.h"
#include "Transform.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_LandDesc, pArg, sizeof(LANDDESC));

	return S_OK;
}

void CLandObject::Tick(_float fTimeDelta)
{
}

void CLandObject::LateTick(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(CTransform* pTransform, _float fMagicNumber)
{
	CObject_Manager*		pObjectMgr = CObject_Manager::Get_Instance();

	Safe_AddRef(pObjectMgr);

	CVIBuffer*			pVIBuffer = (CVIBuffer*)pObjectMgr->Get_Component(m_LandDesc.iTerrainLevelIndex, m_LandDesc.pLayerTag, m_LandDesc.pTerrainBufferComTag, m_LandDesc.iTerrainObjectIndex);

	_float3		vTargetPos = pTransform->Get_State(CTransform::STATE_POSITION);

	vTargetPos.y = pVIBuffer->Compute_Height(vTargetPos) + fMagicNumber;

	pTransform->Set_State(CTransform::STATE_POSITION, vTargetPos);

	Safe_Release(pObjectMgr);

	return S_OK;
}

_float3 CLandObject::SetUp_Topdee(CTransform* pTransform, _uint iTerrainLevelIndex, _tchar* pLayerTag, _uint	iTerrainObjectIndex, _tchar* pTerrainBufferComTag)
{
	CObject_Manager*		pObjectMgr = CObject_Manager::Get_Instance();

	Safe_AddRef(pObjectMgr);

	CTransform*			pTransformm = (CTransform*)pObjectMgr->Get_Component(iTerrainLevelIndex, pLayerTag, pTerrainBufferComTag, iTerrainObjectIndex);

	_float3		vTargetPos = pTransformm->Get_State(CTransform::STATE_POSITION);

	Safe_Release(pObjectMgr);

	return vTargetPos;
}

void CLandObject::Free()
{
	__super::Free();

}
