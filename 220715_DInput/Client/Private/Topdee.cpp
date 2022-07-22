#include "stdafx.h"
#include "..\Public\Topdee.h"

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
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (pGameInstance->Get_DIKState(DIK_UP) & 0x80)
	{
		Move_Frame(DIR_UP);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else if (pGameInstance->Get_DIKState(DIK_DOWN) & 0x80)
	{
		Move_Frame(DIR_DOWN);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else if (pGameInstance->Get_DIKState(DIK_LEFT) & 0x80)
	{
		Move_Frame(DIR_LEFT);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else if (pGameInstance->Get_DIKState(DIK_RIGHT) & 0x80)
	{
		Move_Frame(DIR_RIGHT);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKState(DIK_Z) & 0x80)
	{//����
		m_eCurState = STATE_JUMP;
	}
	Jumping();
	Safe_Release(pGameInstance);
}

void CTopdee::Jumping()
{
	if (m_eCurState != STATE_JUMP)
		return;
	
	
}

void CTopdee::Move_Frame(const TOPDEE_DIRECTION& _eInputDirection)
{
	/*topdee �ؽ��� ���� 
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
				/*m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));*/
				m_pTransformCom->TransOnlyLook(D3DXToRadian(180.f));
				m_bDown = true;
			}
			else if (m_eCurDir == DIR_UP) {
				m_iFirstFrame = 4;
				m_pTransformCom->TransOnlyLook(D3DXToRadian(0.f));
				/*m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(0.f));*/
				m_bDown = false;
			}
			else if (m_eCurDir == DIR_LEFT) {
				m_iFirstFrame = 2;
				m_pTransformCom->TransOnlyLook(D3DXToRadian(270.f));
			}
			else if (m_eCurDir == DIR_RIGHT)
			{
				m_iFirstFrame = 6;
				m_pTransformCom->TransOnlyLook(D3DXToRadian(90.f));
			}
			m_iFrame = m_iFirstFrame;
			m_bMoveFrame = false;
		}
	}
	else if (m_eCurState == STATE_JUMP)
	{
		m_eCurDir = _eInputDirection;
		if (!m_bJump) {
			m_iFrame = 8;
			m_bJump = true;
		}
		if (m_eCurDir == DIR_LEFT)
		{
			m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
		}
		++m_iFrame;
		if (m_iFrame == 13)
			m_iFrame = 8;
	}

}


void CTopdee::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTopdee::Render()
{
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

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/
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


	return S_OK;
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
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
