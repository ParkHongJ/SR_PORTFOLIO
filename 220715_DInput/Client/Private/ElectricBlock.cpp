#include "stdafx.h"
#include "..\Public\ElectricBlock.h"
#include "GameInstance.h"
#include "Hong.h"
#include "GameMgr.h"

CElectricBlock::CElectricBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteraction_Block(pGraphic_Device)
{
}

CElectricBlock::CElectricBlock(const CElectricBlock & rhs)
	: CInteraction_Block(rhs)
{
}

HRESULT CElectricBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CElectricBlock::Initialize(void * pArg)
{// Texture 1 is On 0 is Off
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_bRayCasted = false;

	SetTag(L"Box");
	if (m_pTransformCom != nullptr && pArg != nullptr)
	{
		_float3 vPos;
		vPos = ObjInfo.vPos;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	return S_OK;
}

void CElectricBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	 
	if (m_iNumLevel == 5)
	{//6.5 7.5 투디모드 6.5 9.5아래부터
		_float3 vBoxPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
		if ((_int)vBoxPos.x != 6)
			return;
		else if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::GAMEMODE::TOODEE)
		{
			if ((_int)vBoxPos.z <= 9)
				m_bRayCasted = true;
		}
		else if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::GAMEMODE::TOPDEE)
		{
			if ((_int)vBoxPos.z == 7)
				m_bRayCasted = true;
		}
	}
	else if (m_iNumLevel == 8)
	{//투디 모드 59.5 2.5 탑디도동일 투디모드 71.5 8.5아래
		_float3 vBoxPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
		if (((_int)vBoxPos.x != 59)&&((_int)vBoxPos.x != 71))
			return;
		else if(((_int)vBoxPos.x == 59) && ((_int)vBoxPos.z == 2))
			m_bRayCasted = true;
		else if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::GAMEMODE::TOODEE)
		{
			if(((_int)vBoxPos.x == 71) && ((_int)vBoxPos.z <= 8))
				m_bRayCasted = true;
			
		}
	}
}

void CElectricBlock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (!m_bActive)
		return;
	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	_float4x4 ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3TransformCoord(&vPos, &vPos, &ViewMatrix);
	D3DXVec3TransformCoord(&vPos, &vPos, &ProjMatrix);

	if (vPos.x + 0.1f < -1.f)
	{
		return;
	}
	else if (vPos.x - 0.1f > 1.f)
	{
		return;
	}
	if (m_bRayCasted)
		TextureChange();
	else
		m_iTextureNum = 0;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::INTEREACTION, m_pBoxCollider, m_pTransformCom);
}

HRESULT CElectricBlock::Render()
{
	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture", m_iTextureNum);


	m_pShaderCom->Begin(m_eShaderSelect);//0 default, 1 InHole

	m_pVIBufferCom->Render();

	m_pShaderCom->End();

	
	return S_OK;
}

void CElectricBlock::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CElectricBlock::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CElectricBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta)
{
}

void CElectricBlock::TextureChange()
{
	if (m_iTextureNum == 0)
		m_iTextureNum = 1;
	else
		m_iTextureNum = 0;
}



HRESULT CElectricBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_ElectricBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, this)))
		return E_FAIL;

	return S_OK;
}

CElectricBlock * CElectricBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CElectricBlock*		pInstance = new CElectricBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CElectricBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CElectricBlock::Clone(void* pArg)
{
	CElectricBlock*		pInstance = new CElectricBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CElectricBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CElectricBlock::Free()
{
	__super::Free();
	
}

