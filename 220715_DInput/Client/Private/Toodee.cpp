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

	/* For.Toodee_Col */
	SetTag(L"Toodee");

	/* For.Portal_Data */
	CGameMgr::Get_Instance()->Set_Object_Data(L"Toodee_Portal", &m_bPortal);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 0.5f, 3.f));

	return S_OK;
}

void CToodee::Tick(_float fTimeDelta)
{
	if (CGameMgr::Get_Instance()->Key_Down(DIK_T)) {
		if (m_bActive)
			m_bActive = false;
		else {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 0.5f, 3.f));
			m_bActive = true;
		}
	}

	/* For.Toodee_Turn */
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE) {
		if (m_bActive) {
			if (CGameMgr::Get_Instance()->Key_Down(DIK_Z)) {
				if(TOODEE_PORTAL != m_eCurruntDir)
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
		}
		else if (!m_bActive)
			m_eToodeeDir = TOODEE_DEAD;

		m_eCurruntDir = m_eToodeeDir;
	}

	if (m_bPortal) {
		m_eToodeeDir = TOODEE_PORTAL;
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
		m_eCurruntDir = m_eToodeeDir;
		if (CGameMgr::Get_Instance()->Get_Object_Data(L"Toodee_Portal")) {
			/* Topdee_Portal_ture */
		}
	}
}

void CToodee::LateTick(_float fTimeDelta)
{
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOODEE) {
		if (m_bActive) {
#pragma region Toodee_Speed_and_Frame
			switch (m_eCurruntDir)
			{
			case TOODEE_LEFT:
				m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
				if (5 > m_MoveSpeed)
					m_MoveSpeed += 0.1f;
				if (!m_bPortal) {
					if (11 < m_iTexIndex)
						m_iTexIndex = 5;
					++m_iTexIndex;
				}
				else {
					if (68 > m_iTexIndex || 72 < m_iTexIndex)
						m_iTexIndex = 68;
					++m_iTexIndex;
				}
				break;

			case TOODEE_RIGHT:
				m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
				if (5 > m_MoveSpeed)
					m_MoveSpeed += 0.1f;
				if (!m_bPortal) {
					if (11 < m_iTexIndex)
						m_iTexIndex = 5;
					++m_iTexIndex;
				}
				else {
					if (68 > m_iTexIndex || 72 < m_iTexIndex)
						m_iTexIndex = 68;
					++m_iTexIndex;
				}
				break;

			case TOODEE_JUMP:
				if (11 < m_iTexIndex)
					m_iTexIndex = 5;
				++m_iTexIndex;
				break;

			case TOODEE_PORTAL:
				if (68 > m_iTexIndex || 72 < m_iTexIndex)
					m_iTexIndex = 68;
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
#pragma endregion

#pragma region Toodee_Jump_and_Drop
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
#pragma endregion

			m_pTransformCom->Set_TransformDesc_Speed(m_MoveSpeed);
			m_pTransformCom->Go_Straight_2D(fTimeDelta);

			m_pBoxCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_Scaled());
			m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, this);
		}
		else if (!m_bActive) {
			if (68 > m_iTexIndex)
				m_iTexIndex = 68;

			m_fFrame += fTimeDelta;

			if (m_fFrame > 0.5f) {
				if (72 < m_iTexIndex)
					return;
				++m_iTexIndex;
				m_fFrame = 0.f;
			}
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CToodee::Render()
{
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
	/*_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
	//--------------------------------------------------------------

	return S_OK;
}

void CToodee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CToodee::OnTriggerStay(CGameObject * other, _float fTimeDelta)
{
	_float fBoxSize = 1.f;

	if (other->CompareTag(L"Box")){
		CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
		Safe_AddRef(TargetBox);

		/* maybe_fix_this_part */
		if(TargetBox->Get_State(CTransform::STATE_POSITION).x - (fBoxSize / 2.f) < m_pTransformCom->Get_State(CTransform::STATE_POSITION).x && 
			TargetBox->Get_State(CTransform::STATE_POSITION).x + (fBoxSize / 2.f) > m_pTransformCom->Get_State(CTransform::STATE_POSITION).x)
				m_fDrop_Endline = TargetBox->Get_State(CTransform::STATE_POSITION).z + (fBoxSize * 0.5f);

		Safe_Release(TargetBox);
	}
	else if (other->CompareTag(L"Pig")) {
		m_bActive = false;
	}
	else if (other->CompareTag(L"Portal")) {
		m_bPortal = true;
	}
}

void CToodee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	m_fDrop_Endline = 0.f;
	m_bPortal = false;
}

HRESULT CToodee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
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
	BoxColliderDesc.fRadius = 1.f;

	BoxColliderDesc.bIsTrigger = true;

	/* For.Com_BoxCollider */
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