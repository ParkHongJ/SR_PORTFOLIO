#include "stdafx.h"
#include "Toodee.h"

#include "GameInstance.h"

CToodee::CToodee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CToodee::CToodee(const CToodee & rhs)
	: CGameObject(rhs)
{
}

HRESULT CToodee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CToodee::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetTag(L"Toodee");

	CGameMgr::Get_Instance()->Set_Player_Active(L"Toodee", this);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 0.5f, 3.f));

	return S_OK;
}

void CToodee::Tick(_float fTimeDelta)
{
	/* For.Toodee Dead */
	if (CGameMgr::Get_Instance()->Key_Down(DIK_T)) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 0.5f, 3.f));
		m_bActive = true;
		m_eToodeeDir = TOODEE_IDLE;
		CGameMgr::Get_Instance()->Set_Player_Active(L"Toodee", this);
	}

	if (CGameMgr::Get_Instance()->Key_Down(DIK_F)) {
		m_bActive = false;
		m_eToodeeDir = TOODEE_DEAD;
		m_eCurruntDir = m_eToodeeDir;
		CGameMgr::Get_Instance()->Set_Player_Active(L"Toodee", this);
	}

	if (!m_bActive)
		return;

	/* For.Toodee Run */
	_float4x4 ViewMatrixInv;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);
	_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (5.f > (*(_float3*)&ViewMatrixInv.m[3][0]).z) {
		fPos.y = 0.001f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
		m_bRun = false;
	}
	else {
		fPos.y = 0.5f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
		m_bRun = true;
	}

	if (!m_bRun || !m_bActive)
		return;

	if (CGameMgr::Get_Instance()->Key_Down(DIK_Z)) {
		m_eToodeeDir = TOODEE_JUMP;
		m_bJump = true;
	}

	if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT)) {
		m_eToodeeDir = TOODEE_LEFT;
		if (m_eCurruntDir != m_eToodeeDir)
			m_MoveSpeed = 0.f;
	}
	else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT)) {
		m_eToodeeDir = TOODEE_RIGHT;
		if (m_eCurruntDir != m_eToodeeDir)
			m_MoveSpeed = 0.f;
	}
	else {
		m_eToodeeDir = TOODEE_IDLE;
	}

	m_eCurruntDir = m_eToodeeDir;
}

void CToodee::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	if (m_bRun) {
		/*For.Test*/
		switch (m_eCurruntDir)
		{
		case TOODEE_LEFT:
			m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
			if (5 > m_MoveSpeed)
				m_MoveSpeed += 0.1f;
			if (11 < m_iTexIndex)
				m_iTexIndex = 5;
			++m_iTexIndex;
			break;

		case TOODEE_RIGHT:
			m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
			if (5 > m_MoveSpeed)
				m_MoveSpeed += 0.1f;
			if (11 < m_iTexIndex)
				m_iTexIndex = 5;
			++m_iTexIndex;
			break;

		case TOODEE_JUMP:
			if (11 < m_iTexIndex)
				m_iTexIndex = 5;
			++m_iTexIndex;
			break;

		case TOODEE_IDLE:
			if (0 < m_MoveSpeed)
				m_MoveSpeed -= 0.1f;
			else
				m_MoveSpeed = 0.f;
			if (11 < m_iTexIndex)
				m_iTexIndex = 0;
			++m_iTexIndex;
			break;
		}

		m_pTransformCom->Set_TransformDesc_Speed(m_MoveSpeed);
		m_pTransformCom->Go_Straight_2D(fTimeDelta);

		_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		D3DXVECTOR3 vGravityPower = _float3(0.f, 0.f, -1.f) * 1.63f * m_fJumpTime * 0.5f;

		if (m_bJump) {
			fPos -= m_fJumpPower * fTimeDelta * _float3(0.f, 0.f, -1.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

			if (m_bJump && m_fDrop_Endline - vGravityPower.z  > fPos.z) {
				fPos.z = m_fDrop_Endline;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
				m_fJumpTime = 0.f;
				m_bJump = false;
			}
		}

		if (m_fDrop_Endline - vGravityPower.z < fPos.z) {
			fPos += vGravityPower;

			if (m_fJumpTime > 0.6f)
				m_fJumpTime = 0.6f;
			else
				m_fJumpTime += fTimeDelta;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
		}
		else {
			fPos.z = m_fDrop_Endline;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
			m_fJumpTime = 0.f;
		}
	}

	m_pColliderCom->Add_CollisionGroup(CCollider::TOODEE, this);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CToodee::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------

	return S_OK;
}

void CToodee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CToodee::OnTriggerStay(CGameObject * other, _float fTimeDelta)
{
	_float fBoxSize = 1.f;

	if (other->CompareTag(L"Box"))
	{
		CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
		Safe_AddRef(TargetBox);

		if(TargetBox->Get_State(CTransform::STATE_POSITION).z + (fBoxSize * 0.5f) <= m_pTransformCom->Get_State(CTransform::STATE_POSITION).z)
			m_fDrop_Endline = TargetBox->Get_State(CTransform::STATE_POSITION).z + (fBoxSize * 0.5f);

		Safe_Release(TargetBox);
	}
}

void CToodee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	m_fDrop_Endline = 0.f;
}

HRESULT CToodee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CToodee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CToodee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_SJH, TEXT("Prototype_Component_Texture_Toodee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.5f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 1.f);
	BoxColliderDesc.bIsTrigger = true;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CToodee * CToodee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CToodee* pInstance = new CToodee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CToodee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CToodee::Clone(void* pArg)
{
	CToodee* pInstance = new CToodee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CToodee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToodee::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}