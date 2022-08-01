#include "stdafx.h"
#include "..\Public\Block.h"

#include "GameInstance.h"
#include "GameMgr.h"

CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBlock::CBlock(const CBlock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBlock::Initialize_Prototype()
{
	return S_OK;
}

void CBlock::TextureSelect(const _float3 & vPos)
{
	if (vPos.x == 0.5f && vPos.z == 0.5f)// 좌하
		m_iTextureNum = 2;
	else if (vPos.x == 28.5f&&vPos.z == 0.5f)//우하
		m_iTextureNum = 3;
	else if (vPos.x == 28.5f&&vPos.z == 14.5f)//우상
		m_iTextureNum = 4;
	else if (vPos.x == 0.5f&&vPos.z == 14.5f)//좌하
		m_iTextureNum = 5;
	else if (((vPos.x != 0.5f) && (vPos.x != 28.5f)) && ((vPos.z == 0.5f) || (vPos.z == 14.5)))//가로
		m_iTextureNum = 1;
	else if (((vPos.x == 0.5f) || (vPos.x == 28.5f)) && ((vPos.z != 0.5f) && (vPos.z != 14.5)))//세로
		m_iTextureNum = 0;
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
		TextureSelect(vPos);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	}
	return S_OK;
}

void CBlock::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	if(m_bDropBox)
		Box_Drop_More(fTimeDelta);
	if (m_bTopdeePush)
		Box_Push_More(fTimeDelta,m_vPushFinishPos, m_vPushDir);
}

void CBlock::LateTick(_float fTimeDelta)
{
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
#pragma region About_Topdee
bool CBlock::KKK_Go_Lerp_Raise(_float3 vFinalPos, _float fTimeDelta, _float3 vPreLoaderPos)
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vDist = (vCurPosition - vFinalPos);
	_float fLength = D3DXVec3Length(&vDist);
	if (_int(fLength) <= 1)
	{
		if (((_int)vPreLoaderPos.x == (_int)vCurPosition.x) && ((_int)vPreLoaderPos.z == (_int)vCurPosition.z)) {
			vCurPosition = vCurPosition + (vFinalPos - vCurPosition) * (fTimeDelta * 5);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
			return true;
		}
		return false;
	}
	else
		return false;
}

void CBlock::KKK_Is_Raise(_float3 vTargetPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,vTargetPos);
}

_bool CBlock::KKK_Go_Lerp_Drop(_float3 vFinalPos, _float fTimeDelta, _bool bHoleCall)
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (!bHoleCall) {
		vCurPosition = vCurPosition + (vFinalPos - vCurPosition) * (fTimeDelta * 5);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);

		if (vCurPosition.y <= 0.55f)
			return true;
		return false;
	}
	else 
		m_bDropBox = true;
	return false;
}

void CBlock::Box_Drop_More(_float fTimeDelta)
{
	_float3 vBoxCurPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
	if (vBoxCurPos.y <= -0.45f) {//final Position is -0.45
		vBoxCurPos.y = -0.45f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vBoxCurPos));
		m_bDropBox = false;
		m_bEnabled = false;
		return;
	}
	_float3 vBoxDir = { 0.f,-1.f,0.f };
	_float fBoxSpeed = m_pTransformCom->Get_Speed();
	m_pTransformCom->Translate(vBoxDir *fTimeDelta* fBoxSpeed);
}

void CBlock::Box_Push_More(_float fTimeDelta, _float3 vPushFinishPos, _float3 vPushDir)
{
	if (!m_bEnabled)
		return;
	m_bTopdeePush = true;
	m_vPushFinishPos = vPushFinishPos;
	m_vPushDir = vPushDir;
	_float3 vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fDist = D3DXVec3Length(&(vCurPos - m_vPushFinishPos));
	if (fDist < 0.2f)
	{//위치에 도달했을때.
		Box_Push_Find_A_Place();
		vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_bTopdeePush = false;
		if (CGameMgr::Get_Instance()->Check_PushBox_Exactly(vCurPos))
		{//밀리다가 떨어져야 할때.
			m_bDropBox = true;
			m_bTopdeePush = false;
			return;
		}
		return;
	}
	//위치로 이동중일때.
	if (vPushDir.x == 0.f)
	{//Up or Down
		if (vPushDir.z > 0.f)
			m_pTransformCom->Go_Straight(fTimeDelta);
		else
			m_pTransformCom->Go_Backward(fTimeDelta);
	}
	else if (vPushDir.z == 0.f)
	{
		if (vPushDir.x > 0.f)
			m_pTransformCom->Go_Right(fTimeDelta);
		else
			m_pTransformCom->Go_Left(fTimeDelta);
	}

}

void CBlock::Box_Push_Find_A_Place()
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vRightPosition{ (_int(vCurPosition.x) + 0.5f),(_int(vCurPosition.y) + 0.5f),(_int(vCurPosition.z) + 0.5f) };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightPosition);
}
#pragma endregion About_Topdee
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
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCollCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

