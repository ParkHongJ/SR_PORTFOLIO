#include "stdafx.h"
#include "..\Public\Thunder_Cloud.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"
CThunder_Cloud::CThunder_Cloud(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CThunder_Cloud::CThunder_Cloud(const CThunder_Cloud & rhs)
	: CGameObject(rhs)
{
}

HRESULT CThunder_Cloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CThunder_Cloud::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{//18.5 0.5 9.5
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
		_float3 vPos;
		vPos = ObjInfo.vPos;
		if (FAILED(SetUp_Components()))
			return E_FAIL;
		m_pTransformCom_Cloud->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y/*+ 1.5f*/, vPos.z - 0.01f));

		m_pTransformCom_Shadow->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y - 0.3f /*+ 1.5f*/, vPos.z - 0.01f));
		m_pTransformCom_Shadow->Rotation(_float3(1.0f, 0.f, 0.f), D3DXToRadian(90.f));

		m_pTransformCom_Rain->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y /*+ 1.5f*/, vPos.z - 2.f));
		m_pTransformCom_RainTopdee->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y , vPos.z - 2.f));
		m_pTransformCom_Rain->Rotation(_float3(1.0f, 0.f, 0.f), D3DXToRadian(90.f));
		m_vToodeePos = _float3(vPos.x, vPos.y, vPos.z);
		vPos.y += 5.f;
		m_vTopdeePos = vPos;
		m_vShadow_TopdeePos = _float3{ vPos.x ,0.1f, vPos.z - 3.f };//그림자 보정값.
		m_vShadow_ToodeePos = _float3(m_vToodeePos.x - 0.1f, 0.1f, m_vToodeePos.z - 0.1f);
	}
	m_vTopdeeRainPos = { m_vShadow_TopdeePos.x, m_vShadow_TopdeePos.y + 4.5f, m_vShadow_TopdeePos.z+1.f };
	TopdeeRain_CloudInitMgr();
#pragma region Ray
	_float3 vCloudPos{ m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION) };//이건 투디일때로 시작.
	m_pColliderCom->AddRayList(_float3(m_vShadow_TopdeePos.x, 5.f, m_vShadow_TopdeePos.z)		, _float3(0.f, -1.f, 0.f));	//탑디일때의 레이는 -z방향
	m_pColliderCom->AddRayList(_float3(m_vShadow_TopdeePos.x - 1.f, 5.f, m_vShadow_TopdeePos.z)	, _float3(0.f, -1.f, 0.f));	//탑디일때의 레이는 
	m_pColliderCom->AddRayList(_float3(m_vShadow_TopdeePos.x + 1.f, 5.f, m_vShadow_TopdeePos.z)	, _float3(0.f, -1.f, 0.f));
	//----------------------------TopdeeRay End
	m_pColliderCom->AddRayList(_float3(vCloudPos.x, vCloudPos.y, vCloudPos.z+0.01f), _float3(0.f, 0.f, -1.f));
	m_pColliderCom->AddRayList(_float3(vCloudPos.x -1.f, vCloudPos.y, vCloudPos.z+0.01f), _float3(0.f, 0.f, -1.f));
	m_pColliderCom->AddRayList(_float3(vCloudPos.x + 1.f, vCloudPos.y, vCloudPos.z+0.01f), _float3(0.f, 0.f, -1.f));
#pragma endregion Ray	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayEffect(TEXT("rainSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_MAX);

	Safe_Release(pGameInstance);

	return S_OK;
}
void CThunder_Cloud::TopdeeRainPosMgr(_float fCamX)
{
	
	_float fDist{ m_vToodeePos.x - fCamX };

	_int iDist = (_int)floor(fDist + 0.5);
	if ((_int)m_vToodeePos.z == 5) {
		if (iDist == -10)
		{//4
			m_vTopdeeRainPos.x = 6.5f;
			m_vTopdeePos.x = 6.0f;
		}
		else if (iDist == -9)
		{//5
			m_vTopdeeRainPos.x = 7.5f;
			m_vTopdeePos.x = 7.0f;
		}
		else if (iDist == -8)
		{//6
			m_vTopdeeRainPos.x = 8.f;
			m_vTopdeePos.x = 7.5f;
		}
		else if (iDist == -7)
		{//7
			m_vTopdeeRainPos.x = 9.f;
			m_vTopdeePos.x = 8.5f;
		}
		else if (iDist == -6)
		{//8
			m_vTopdeeRainPos.x = 9.5f;
			m_vTopdeePos.x = 9.0f;
		}
		else if (iDist == -5)
		{//9
			m_vTopdeeRainPos.x = 10.5f;
			m_vTopdeePos.x = 10.0f;
		}
		else if (iDist == -4)
		{//10
			m_vTopdeeRainPos.x = 11.5f;
			m_vTopdeePos.x = 11.0f;
		}
		else if (iDist == -3)
		{//11
			m_vTopdeeRainPos.x = 12.0f;
			m_vTopdeePos.x = 11.5f;
		}
		else if (iDist == -2)
		{//12
			m_vTopdeeRainPos.x = 13.0f;
			m_vTopdeePos.x = 13.5f;
		}
		else if (iDist == 3)
		{//17
			m_vTopdeeRainPos.x = 17.0f;
			m_vTopdeePos.x = 17.5f;
		}
		else if (iDist == 4)
		{//18
			m_vTopdeeRainPos.x = 17.0f;
			m_vTopdeePos.x = 17.5f;
		}
		else if (iDist == 5)
		{//19
			m_vTopdeeRainPos.x = 18.5f;
			m_vTopdeePos.x = 19.f;
		}
		else if (iDist == 6)
		{//20
			m_vTopdeeRainPos.x = 19.5f;
			m_vTopdeePos.x = 20.f;
		}
		else if (iDist == 7)
		{//21
			m_vTopdeeRainPos.x = 20.5f;
			m_vTopdeePos.x = 21.f;
		}
		else if (iDist == 8)
		{//22
			m_vTopdeeRainPos.x = 21.f;
			m_vTopdeePos.x = 21.5f;
		}
		else if (iDist == 9)
		{//23
			m_vTopdeeRainPos.x = 22.f;
			m_vTopdeePos.x = 23.0f;
		}
		else if (iDist == 8)
		{//24
			m_vTopdeeRainPos.x = 23.f;
			m_vTopdeePos.x = 24.f;
		}
		else if (iDist == 8)
		{//25
			m_vTopdeeRainPos.x = 23.5f;
			m_vTopdeePos.x = 24.5f;
		}
		else if (iDist == 8)
		{//26
			m_vTopdeeRainPos.x = 24.5f;
			m_vTopdeePos.x = 25.5f;
		}

	}
	
}
void CThunder_Cloud::TopdeeRain_CloudInitMgr()
{
	if (m_iNumLevel == 5)
	{//스테이지5
		if ((_uint)m_vTopdeeRainPos.x == 18)
		{
			m_vTopdeeRainPos.x -= 0.5f;
		}
		else if ((_uint)m_vTopdeeRainPos.x == 6)
		{
			m_vTopdeeRainPos.x += 1.f;
		}

	}
}
void CThunder_Cloud::Tick(_float fTimeDelta)
{
	
	/*m_pTransformCom_RainTopdee->*/
	if (!m_bActive)
		return;
	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	_float4x4   CamWorldMatrix;
	D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
	_float3 vCameraPos{ (*(_float3*)&CamWorldMatrix.m[3][0]) };
	//TopdeeRainPosMgr(vCameraPos.x);
#pragma region animation
	//애니메이션 돌리고
	m_fFrame_Cloud += 6.f * fTimeDelta;
	if (m_fFrame_Cloud >= 6.0f)
		m_fFrame_Cloud = 0.f;

	m_fFrame_Rain += 13.0f * fTimeDelta;
	if (m_fFrame_Rain >= 13.0f)
		m_fFrame_Rain = 0.f;
#pragma endregion 
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)
	{
		if (m_fCam_CloudDist == -1000.f) {
			m_fCam_CloudDist = { (m_vTopdeePos.x - vCameraPos.x) };
			m_vCloudRealPos = _float3((m_fCam_CloudDist * 0.25f) + (m_vTopdeePos.x), m_vTopdeePos.y, m_vTopdeePos.z);
		}
		m_pTransformCom_Cloud->Set_State(
			CTransform::STATE_POSITION,
			MoveTowards(m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION),
				m_vTopdeePos,
				fTimeDelta * 15.f));
		//일단 5.5부터 시작.
		m_pTransformCom_Shadow->Set_State(
			CTransform::STATE_POSITION,
			MoveTowards(m_pTransformCom_Shadow->Get_State(CTransform::STATE_POSITION),
				m_vShadow_TopdeePos,
				fTimeDelta * 15.f));
		//탑디일때는 보정값을 줘야함. z값맞추면 이상해짐
		
		m_pTransformCom_RainTopdee->Rotation(_float3(1.0f, 0.f, 0.f), D3DXToRadian(40.f));
		m_pTransformCom_RainTopdee->Set_State(
			CTransform::STATE_POSITION,
			MoveTowards(m_pTransformCom_RainTopdee->Get_State(CTransform::STATE_POSITION),
				m_vTopdeeRainPos,
				fTimeDelta * 15.f));
	
		m_bEnabled = false;
	}
	else
	{
		m_pTransformCom_Cloud->Set_State(
			CTransform::STATE_POSITION,
			MoveTowards(m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION),
				m_vToodeePos,
				fTimeDelta * 15.f));

		m_pTransformCom_Shadow->Set_State(
			CTransform::STATE_POSITION,
			MoveTowards(m_pTransformCom_Shadow->Get_State(CTransform::STATE_POSITION),
				m_vShadow_ToodeePos,
				fTimeDelta * 15.f));
		m_pTransformCom_Rain->Rotation(_float3(1.0f, 0.f, 0.f), D3DXToRadian(90.f));
		m_pTransformCom_Rain->Set_State(
			CTransform::STATE_POSITION,
			MoveTowards(m_pTransformCom_Rain->Get_State(CTransform::STATE_POSITION),
				_float3(m_vToodeePos.x, m_vToodeePos.y, m_vToodeePos.z - 5.f),
				fTimeDelta * 15.f));

		m_bEnabled = true;
	}

}

void CThunder_Cloud::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	m_pRendererCom_Cloud->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	_float4x4   CamWorldMatrix;
	D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
	_float3 vCameraPos{ (*(_float3*)&CamWorldMatrix.m[3][0]) };
	_float3 vCloudPos{ m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION) };
	m_pTransformCom_Cloud->LookAt(_float3(vCloudPos.x,vCameraPos.y,vCameraPos.z));
	
	_float3 vScale{ m_pTransformCom_Cloud->Get_Scaled() };
	if (vCameraPos.z > vCloudPos.z)
		m_pTransformCom_Cloud->Set_Scaled(_float3(vScale.x, -vScale.y, vScale.z));
	
	m_fSndTime += fTimeDelta;

	if (m_fSndTime > 20.f) {
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlayEffect(TEXT("rainSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_MAX);

		Safe_Release(pGameInstance);

		m_fSndTime = 0;
	}
}

HRESULT CThunder_Cloud::Render()
{
	if (!m_bActive)
		return S_OK;
	
	if ((CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)&&(m_pTransformCom_Cloud->Get_State(CTransform::STATE_POSITION).y>4.5f))
	{
		if (FAILED(m_pTransformCom_RainTopdee->Bind_WorldMatrix()))
			return E_FAIL;
		if(FAILED(m_pTextureCom_RainTopdee->Bind_Texture((_uint)m_fFrame_Rain)))
			return E_FAIL;
		if (FAILED(Set_RenderState()))
			return E_FAIL;
		m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pVIBufferCom_RainTopdee->Render();
		m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTransformCom_Rain->Bind_WorldMatrix()))
			return E_FAIL;
		if (FAILED(m_pTextureCom_Rain->Bind_Texture((_uint)m_fFrame_Rain)))
			return E_FAIL;
		if (FAILED(Set_RenderState()))
			return E_FAIL;
		m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pVIBufferCom_Rain->Render();
		m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}
	\
	//================================================================RainEnd
	
	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;

	WorldMatrix = m_pTransformCom_Shadow->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom_Shadow->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom_Shadow->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom_Shadow->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pTextureCom_Shadow->Bind_Texture(m_pShaderCom_Shadow, "g_Texture", (_uint)m_fFrame_Cloud);


	m_pShaderCom_Shadow->Begin(0);

	m_pVIBufferCom_Shadow->Render();

	m_pShaderCom_Shadow->End();
	//===========================================================================shadowend
	if (FAILED(m_pTransformCom_Cloud->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Cloud->Bind_Texture((_uint)m_fFrame_Cloud)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom_Cloud->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
	return S_OK;
	
}

void CThunder_Cloud::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	if (!m_bActive)
		return;

}



HRESULT CThunder_Cloud::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CThunder_Cloud::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CThunder_Cloud::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom_Cloud, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 3.0f, 9.f, 0.f };
	//========================================================================================
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rain"), (CComponent**)&m_pVIBufferCom_Rain, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Rain"), TEXT("Com_Texture_Rain"), (CComponent**)&m_pTextureCom_Rain, this)))
		return E_FAIL;


	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_Rain"), (CComponent**)&m_pTransformCom_Rain, this, &TransformDesc)))
		return E_FAIL;

	//========================================================================================
	CVIBuffer_Rect::RECTDESC RectDescTopdee;
	RectDescTopdee.vSize = { 3.0f, 5.f, 0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rain_Topdee"), (CComponent**)&m_pVIBufferCom_RainTopdee, this, &RectDescTopdee)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Rain"), TEXT("Com_Texture_Rain_Topdee"), (CComponent**)&m_pTextureCom_RainTopdee, this)))
		return E_FAIL;


	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_RainTopdee"), (CComponent**)&m_pTransformCom_RainTopdee, this, &TransformDesc)))
		return E_FAIL;

//==================================================================Rain End
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Thunder_Cloud"), TEXT("Com_Texture_Cloud"), (CComponent**)&m_pTextureCom_Cloud, this)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_Cloud"), (CComponent**)&m_pTransformCom_Cloud, this, &TransformDesc)))
		return E_FAIL;
	
	CVIBuffer_Rect::RECTDESC RectDesc2;
	RectDesc2.vSize = { 3.0f, 1.f, 0.f };
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Cloud"), (CComponent**)&m_pVIBufferCom_Cloud, this, &RectDesc2)))
		return E_FAIL;
	
//=======================================================CloudEnd


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Thunder_Cloud_Shadow"), TEXT("Com_Texture_Shadow"), (CComponent**)&m_pTextureCom_Shadow, this)))
	return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_Shadow"), (CComponent**)&m_pTransformCom_Shadow, this, &TransformDesc)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Shadow"), (CComponent**)&m_pVIBufferCom_Shadow, this, &RectDesc2)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Shadow"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom_Shadow, this)))
		return E_FAIL;

//=======================================================ShadowEnd
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;
	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(3.f, 0.5f, 1.f);
	BoxColliderDesc.bIsTrigger = true;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;
	

	return S_OK;
}

CThunder_Cloud * CThunder_Cloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CThunder_Cloud* pInstance = new CThunder_Cloud(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CThunder_Cloud"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CThunder_Cloud::Clone(void * pArg)
{
	CThunder_Cloud* pInstance = new CThunder_Cloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CThunder_Cloud"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThunder_Cloud::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom_Cloud);
	Safe_Release(m_pVIBufferCom_Cloud);
	Safe_Release(m_pRendererCom_Cloud);
	Safe_Release(m_pTransformCom_Cloud);
	//===========================================
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pBoxCom);
	//====================================CloudEnd
	Safe_Release(m_pTextureCom_Rain);
	Safe_Release(m_pTransformCom_Rain);
	Safe_Release(m_pVIBufferCom_Rain);

	Safe_Release(m_pTextureCom_RainTopdee);
	Safe_Release(m_pTransformCom_RainTopdee);
	Safe_Release(m_pVIBufferCom_RainTopdee);
	//=====================================RainEnd

	Safe_Release(m_pTextureCom_Shadow);
	Safe_Release(m_pTransformCom_Shadow);
	Safe_Release(m_pVIBufferCom_Shadow);
	Safe_Release(m_pShaderCom_Shadow);
		
}
