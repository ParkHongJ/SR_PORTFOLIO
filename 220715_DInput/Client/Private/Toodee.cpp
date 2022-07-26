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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.3f, 0.f));

	return S_OK;
}

void CToodee::Tick(_float fTimeDelta)
{
	/* For.Topdee Stop */
	if (GetKeyState('X') & 0x8000) {
		_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_Run) {
			fPos.y = 0.001f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

			m_Run = false;
		}
		else {
			fPos.y = 0.3f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

			m_Run = true;
		}
	}

	/* For.Toodee Dead */
	if (GetKeyState('F') & 0x8000) {
		if (m_Dead) {
			m_Dead = false;
		}
		else {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.3f, 0.f));
			m_Dead = true;
		}
	}

	if (!m_Run || m_Dead)
		return;

	if (GetKeyState('Z') & 0x8000) {
		m_eToodeeDir = TOODEE_JUMP;
		m_bJump = true;
	}

	if (GetKeyState(VK_LEFT) & 0x8000) {
		m_eToodeeDir = TOODEE_LEFT;
		if (m_eCurruntDir != m_eToodeeDir) {
			m_eCurruntDir = m_eToodeeDir;
			m_MoveSpeed = 0.f;
		}
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000) {
		m_eToodeeDir = TOODEE_RIGHT;
		if (m_eCurruntDir != m_eToodeeDir) {
			m_eCurruntDir = m_eToodeeDir;
			m_MoveSpeed = 0.f;
		}
	}
	else {
		m_eToodeeDir = TOODEE_IDLE;
		m_eCurruntDir = m_eToodeeDir;
	}
}

void CToodee::LateTick(_float fTimeDelta)
{
	if (m_Dead)
		return;

	if (m_Run) {
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

		if (m_bJump) {
			fPos -= m_fJumpPower * fTimeDelta * _float3(0.f, 0.f, -1.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

			if (m_bJump && m_Temp_For_Jump > fPos.z) {
				fPos.z = m_Temp_For_Jump;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
				m_bJump = false;
				m_fJumpTime = 0.f;
			}
		}

		if (0.f < m_pTransformCom->Get_State(CTransform::STATE_POSITION).z) {
			D3DXVECTOR3 vGravityPower = _float3(0.f, 0.f, -1.f) * 1.63f * m_fJumpTime * 0.5f;
			fPos += vGravityPower;

			if (m_fJumpTime >= 0.6f)
				m_fJumpTime = 0.6f;
			else
				m_fJumpTime += fTimeDelta;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
		}
		else {
			fPos.z = m_Temp_For_Jump;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
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
	//m_pColliderCom->Render();
	//--------------------------------------------------------------

	_float3 temp = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	char Sour[256] = { "Toodee" };
	char szBuf[MAX_PATH] = {};

	ImGui::Begin(Sour);

	ImGui::Text("Toodee");

	strcpy(szBuf, Sour);
	strcat(szBuf, "_x");
	ImGui::SliderFloat(szBuf, &temp.x, -100.0f, 100.0f);

	strcpy(szBuf, Sour);
	strcat(szBuf, "_y");
	ImGui::SliderFloat(szBuf, &temp.y, -100.0f, 100.0f);

	strcpy(szBuf, Sour);
	strcat(szBuf, "_z");
	ImGui::SliderFloat(szBuf, &temp.z, -100.0f, 100.0f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);

	ImGui::Spacing();
	ImGui::End();

	return S_OK;
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

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		//return E_FAIL;

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
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}