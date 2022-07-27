#include "stdafx.h"
#include "..\Public\Topdee.h"
#include "KeyMgr.h"
#include "GameInstance.h"
CTopdee::CTopdee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTopdee::CTopdee(const CTopdee & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTopdee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTopdee::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	return S_OK;
}

void CTopdee::Tick(_float fTimeDelta)
{
	_float TopdeeSpeed = m_pTransformCom->Get_Speed();
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	Topdee_Turn_Check();
	KKK_IsRaise(fTimeDelta, 1);
	if (m_bTurn) {
		_float3 vTargetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (pGameInstance->Get_DIKState(DIK_UP) & 0x80)
		{
			Move_Frame(DIR_UP);
			m_vTargetDir = { 0.f, 0.f, 1.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed *fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (pGameInstance->Get_DIKState(DIK_DOWN) & 0x80)
		{
			Move_Frame(DIR_DOWN);
			m_vTargetDir = { 0.f, 0.f, -1.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (pGameInstance->Get_DIKState(DIK_LEFT) & 0x80)
		{
			Move_Frame(DIR_LEFT);
			m_vTargetDir = { -1.f, 0.f, 0.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (pGameInstance->Get_DIKState(DIK_RIGHT) & 0x80)
		{
			Move_Frame(DIR_RIGHT);
			m_vTargetDir = { 1.f, 0.f, 0.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('Z'))
		{//박스들기.
			KKK_DropBox(fTimeDelta);
			KKK_FindBox(fTimeDelta);
			m_bPress = true;
		}
		else
			m_bPress = false;
	}
	else if (!m_bTurn)
		Not_My_Turn_Texture();
	if (!m_bPress)	//no keyInput Go Lerp
		Go_Lerp(fTimeDelta);
	
	Safe_Release(pGameInstance);
}

void CTopdee::Go_Lerp(_float fTimeDelta)
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vFinalPosition;

	vFinalPosition.x = _int(vCurPosition.x) + 0.5f;
	vFinalPosition.y = _int(vCurPosition.y) + 0.5f;
	vFinalPosition.z = _int(vCurPosition.z) + 0.5f;

	vCurPosition = vCurPosition + (vFinalPosition - vCurPosition) * (fTimeDelta * 5);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
}

void CTopdee::Not_My_Turn_Texture()
{
	if (m_eCurDir == DIR_DOWN) {
		m_iFrame = 14;
	}
	else if (m_eCurDir == DIR_UP) {
		m_iFrame = 16;
	}
	else if (m_eCurDir == DIR_LEFT) {
		m_iFrame = 15;
	}
	else if (m_eCurDir == DIR_RIGHT)
	{
		m_iFrame = 13;
	}
}

void CTopdee::KKK_IsRaise(_float fTimeDelta, _char KKK_NotOverride)
{
	if (m_pRaiseObject == nullptr)
		return;
	if (m_fRaising_Box_DelayTimer == 15000.f) {
		//falling
		KKK_DropBox(fTimeDelta);
		return;
	}
	m_fRaising_Box_DelayTimer += fTimeDelta;
	_float3 vfinalPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vfinalPos.y += 1.f;
	m_pRaiseObject->KKK_Is_Raise(vfinalPos);
}

void CTopdee::Topdee_Turn_Check()
{
	
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	_float4x4		CamWorldMatrix;
	D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
	_float fCameraZ = (*(_float3*)&CamWorldMatrix.m[3][0]).z;
	if ((fCameraZ >= -1.f)&&(fCameraZ <= 1.f))
		m_bTurn = true;
	else
		m_bTurn = false;
	
}

void CTopdee::Move_Frame(const TOPDEE_DIRECTION& _eInputDirection)
{
	/*topdee 텍스쳐 셋팅 
	left
	 0, 1 Topdee idle
	 2, 3 Topdee Left
	 4, 5 Topdee Up
	 6, 7 Topdee Right
	 8~12 Jump
	 */
	if (m_eCurState == STATE_IDLE) {
		if (m_eCurDir == _eInputDirection) {
			if (!m_bMoveFrame) {
				++m_iFrame;
				m_bMoveFrame = true;
			}
			else {
				m_iFrame = m_iFirstFrame;
				m_bMoveFrame = false;
			}
		}
		else
		{
			m_eCurDir = _eInputDirection;
			if (m_eCurDir == DIR_DOWN) {
				m_iFirstFrame = 0;
			}

			else if (m_eCurDir == DIR_UP) {
				m_iFirstFrame = 4;
			}

			else if (m_eCurDir == DIR_LEFT) {
				m_iFirstFrame = 2;
			}

			else if (m_eCurDir == DIR_RIGHT)
			{
				m_iFirstFrame = 6;
			}
			m_iFrame = m_iFirstFrame;
			m_bMoveFrame = false;
		}
	}
}

void CTopdee::LateTick(_float fTimeDelta)
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_CollisionGroup(CCollider::TOPDEE, this);

}

HRESULT CTopdee::Render()
{
	//if (!m_bTurn)
		//m_iFrame = 13;
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos;
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_Texture(m_iFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTopdee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	
	return S_OK;
}

HRESULT CTopdee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CTopdee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GYUH, TEXT("Prototype_Component_Texture_Topdee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this)))
		return E_FAIL;
	return S_OK;
}

void CTopdee::KKK_FindBox(_float fTimeDelta)
{
	if (m_pRaiseObject != nullptr)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CLayer* pLayer = pGameInstance->KKK_GetBox();
	if (pLayer == nullptr)
		return;
	KKK_m_pBoxList = pLayer->KKK_Get_List();

	_float3 vTopdeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vTopdeePos.y += 1.f;

	auto& iter = (*KKK_m_pBoxList).begin(); // <- 박스가 없을때 터짐
	bool bMove{false};

	for (_uint i = 0; i < (*KKK_m_pBoxList).size(); ++i)
	{
		bMove = (*iter)->KKK_Go_Lerp_Raise(vTopdeePos, fTimeDelta);
		if (bMove)
			break;
		++iter;
	}

	if (!bMove)
		m_pRaiseObject = nullptr;
	else {
		m_fRaising_Box_DelayTimer = fTimeDelta;
		m_pRaiseObject = (*iter);
<<<<<<< HEAD
	}
	
=======

	Safe_Release(pGameInstance);
>>>>>>> feature/SJH
}

void CTopdee::KKK_DropBox(_float fTimeDelta)
{
	if (m_pRaiseObject == nullptr)
		return;
	if (_int(m_fRaising_Box_DelayTimer) < 1)
		return;
	
	if (m_vBoxDropPos == _float3(-1.f, -1.f, -1.f)) {
		m_fRaising_Box_DelayTimer = 15000.f;
		m_vBoxDropPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vBoxDropPos.y = 0.f;
		if (m_eCurDir == DIR_UP)
			m_vBoxDropPos.z += 1.f;
		else if (m_eCurDir == DIR_LEFT)
			m_vBoxDropPos.x -= 1.f;
		else if (m_eCurDir == DIR_RIGHT)
			m_vBoxDropPos.x += 1.f;
		else if (m_eCurDir == DIR_DOWN)
			m_vBoxDropPos.z -= 1.f;
	}
	
	if (m_pRaiseObject->KKK_Go_Lerp_Drop(m_vBoxDropPos, fTimeDelta)) {
		m_pRaiseObject = nullptr;
		m_fRaising_Box_DelayTimer = 0.f;
		m_vBoxDropPos = _float3(-1.f, -1.f, -1.f);
	}
}

CTopdee * CTopdee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTopdee*		pInstance = new CTopdee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTopdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTopdee::Clone(void* pArg)
{
	CTopdee*		pInstance = new CTopdee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CTopdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTopdee::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

