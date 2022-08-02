#include "stdafx.h"
#include "..\Public\Block.h"

#include "GameInstance.h"


CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteraction_Block(pGraphic_Device)
{
}

CBlock::CBlock(const CBlock & rhs)
	: CInteraction_Block(rhs)
{
}

HRESULT CBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlock::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//======================
	SetTag(L"Box");
	//======================
	if (pArg != nullptr)
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	}
	return S_OK;
}

void CBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
}

void CBlock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (!m_bActive)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::BLOCK, m_pBoxCollider, m_pTransformCom);
}

HRESULT CBlock::Render()
{
	if (!m_bActive)
		S_OK;
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureNum)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	/*_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCollider->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
	//--------------------------------------------------------------

	
	return S_OK;
}

void CBlock::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CBlock::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta)
{
}
//#pragma region About_Topdee
//bool CBlock::KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta, _float3 vPreLoaderPos)
//{
//	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
//	_float3 vDist = (vCurPosition - vFinalPos);
//	_float fLength = D3DXVec3Length(&vDist);
//	if (_int(fLength) <= 1)
//	{
//		if (((_int)vPreLoaderPos.x == (_int)vCurPosition.x) && ((_int)vPreLoaderPos.z == (_int)vCurPosition.z)) {
//			vCurPosition = vCurPosition + (vFinalPos - vCurPosition) * (fTimeDelta * 5);
//			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
//			return true;
//		}
//		return false;
//	}
//	else
//		return false;
//}
//
//void CBlock::KKK_Is_Raise(_float3 vTargetPos)
//{
//	m_pTransformCom->Set_State(CTransform::STATE_POSITION,vTargetPos);
//}
//
//_bool CBlock::KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta, _bool bHoleCall)
//{
//	_float fBoxSpeed{ m_pTransformCom->Get_Speed() };
//	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);//Box Cur Pos.
//	if (!bHoleCall) {
//		_float3 vHaveToReturnPos{ vFinalPos };
//		vFinalPos = MoveTowards(vCurPosition, vFinalPos, fTimeDelta*fBoxSpeed);
//		if(vFinalPos == vHaveToReturnPos)
//			return true;
//		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFinalPos);
//		return false;
//	}
//	else 
//		m_bDropBox = true;
//	return false;
//}
//
//void CBlock::Box_Drop_More(_float fTimeDelta)
//{
//	_float3 vBoxCurPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
//	if (vBoxCurPos.y <= -0.45f) {//final Position is -0.45
//		vBoxCurPos.y = -0.45f;
//		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vBoxCurPos));
//		m_bDropBox = false;
//		m_bEnabled = false;
//		return;
//	}
//	_float3 vBoxDir = { 0.f,-1.f,0.f };
//	_float fBoxSpeed = m_pTransformCom->Get_Speed();
//	m_pTransformCom->Translate(vBoxDir *fTimeDelta* fBoxSpeed);
//}
//
//void CBlock::Box_Push_More(_float fTimeDelta, _float3 vPushFinishPos, _bool bFirstCall)
//{
//	if (!m_bEnabled)
//		return;
//	if (bFirstCall)
//	{
//		m_bTopdeePush = true;
//		m_vPushFinishPos = vPushFinishPos;
//	}
//	_float3 vCurPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
//	_float fBoxSpeed{ m_pTransformCom->Get_Speed() };
//	vCurPos = MoveTowards(vCurPos, m_vPushFinishPos, fTimeDelta*fBoxSpeed);
//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
//	if (vCurPos == m_vPushFinishPos)
//	{//떨어질 위치는 아니고 도착했을때.
//		m_bTopdeePush = false;
//		if (CGameMgr::Get_Instance()->Check_PushBox_Exactly(vCurPos))
//		{//밀리다가 떨어져야 할때.
//			m_bDropBox = true;
//		}
//		return;
//	}
//}
//#pragma endregion About_Topdee
HRESULT CBlock::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CBlock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pCollCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxDesc;
	BoxDesc.vPos = { 0.f,0.f,0.f };
	BoxDesc.vSize = { 1.f,1.f,1.f };
	BoxDesc.bIsTrigger = false;
	BoxDesc.fRadius = 0.5f;
	/* For.Com_BoxCollider*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCollider, this, &BoxDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CBlock * CBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock*		pInstance = new CBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlock::Clone(void* pArg)
{
	CBlock*		pInstance = new CBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock::Free()
{
	__super::Free();
	
}

