#include "stdafx.h"
#include "..\Public\Portal.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "Hong.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPortal::CPortal(const CPortal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void * pArg)
{

	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float3 vPos;
	vPos = ObjInfo.vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(5.f, 0.5f, 2.f));
	m_Tag = L"Portal";

	/* For.Portal_Data */
	CGameMgr::Get_Instance()->Set_Object_Data(L"Portal_Clear", &m_bClear);
	CGameMgr::Get_Instance()->Set_Object_Data(L"Portal_NextLevel", &m_bNextLevel);

	return S_OK;
}

void CPortal::Tick(_float fTimeDelta)
{
	m_fFrame += 11.0f * fTimeDelta;

	if (m_fFrame >= 11.0f)
		m_fFrame = 0.f;

#pragma region For.Toodee_Topdee_Portal_Spr
	if (CGameMgr::Get_Instance()->Get_Object_Data(L"Toodee_Portal")
		&& CGameMgr::Get_Instance()->Get_Object_Data(L"Topdee_Portal")) {
		if (!m_bNextLevelSound) {
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			pGameInstance->PlayEffect(TEXT("winSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
			Safe_Release(pGameInstance);
			m_bNextLevelSound = true;
		}

		m_fFrame_Time += 5.f * fTimeDelta;

		if (m_fFrame_Time > 0.5f) {
			if (m_fFrame_For_TopToo > 17)
				m_bNextLevel = true;

			++m_fFrame_For_TopToo;
			m_fFrame_Time = 0.f;
		}
	}
	else { 
		m_fFrame_For_TopToo = 0.f;
	}
#pragma endregion
}

void CPortal::LateTick(_float fTimeDelta)
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pColliderCom->Add_CollisionGroup(CCollider::PORTAL, m_pBoxCom, m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	
}

HRESULT CPortal::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture((_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

#pragma region For.Toodee_Topdee_Portal_Spr
	if (CGameMgr::Get_Instance()->Get_Object_Data(L"Toodee_Portal")
		&& CGameMgr::Get_Instance()->Get_Object_Data(L"Topdee_Portal")) {
		if (!m_bClear) {
			if(CGameMgr::GAMEMODE::TOODEE != CGameMgr::Get_Instance()->GetMode())
				CGameMgr::Get_Instance()->Player_Active();
			m_bClear = true;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			_float3(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x,
				1.f,
				m_pTransformCom->Get_State(CTransform::STATE_POSITION).z));

		if (FAILED(m_pTextureCom_for_Toodee->Bind_Texture((_uint)m_fFrame_For_TopToo)))
			return E_FAIL;

		if (FAILED(Set_RenderState()))
			return E_FAIL;

		m_pVIBufferCom_for_Toodee->Render();

		if (FAILED(Reset_RenderState()))
			return E_FAIL;

		if (FAILED(m_pTextureCom_For_Topdee->Bind_Texture((_uint)m_fFrame_For_TopToo)))
			return E_FAIL;

		if (FAILED(Set_RenderState()))
			return E_FAIL;

		m_pVIBufferCom_For_Topdee->Render();

		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}
#pragma endregion

	//---------------------디버그일때 그리기-------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------

	return S_OK;
}

void CPortal::OnTriggerExit(CGameObject * other, float fTimeDelta)
{
	int a = 10;
}

void CPortal::OnTriggerEnter(CGameObject * other, float fTimeDelta)
{
	int a = 10;
}

void CPortal::OnTriggerStay(CGameObject * other, float fTimeDelta, _uint eDireciton)
{
	int a = 10;
}

HRESULT CPortal::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CPortal::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CPortal::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	CVIBuffer_Rect::RECTDESC RectDesc;
	RectDesc.vSize = { 2.f,2.f,0.f };
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this,&RectDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Portal"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	BoxColliderDesc.bIsTrigger = true;
	BoxColliderDesc.fRadius = 1.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

#pragma region For.TooTop_Portal_Spr
	RectDesc.vSize = { 6.f,6.f,0.f };

	/* For.Com_VIBuffer_for_Topdee */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Topdee"), (CComponent**)&m_pVIBufferCom_For_Topdee, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture_for_Topdee */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"), TEXT("Com_Texture_Topdee"), (CComponent**)&m_pTextureCom_For_Topdee, this)))
		return E_FAIL;

	RectDesc.vSize = { 6.f, 6.f, 0.f };

	/* For.Com_VIBuffer_for_Toodee */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Toodee"), (CComponent**)&m_pVIBufferCom_for_Toodee, this, &RectDesc)))
		return E_FAIL;

	/* For.Com_Texture_for_Toodee */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"), TEXT("Com_Texture_Toodee"), (CComponent**)&m_pTextureCom_for_Toodee, this)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPortal* pInstance = new CPortal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	
#pragma region For.TooTop_Portal_Spr
	Safe_Release(m_pVIBufferCom_For_Topdee);
	Safe_Release(m_pTextureCom_For_Topdee);

	Safe_Release(m_pVIBufferCom_for_Toodee);
	Safe_Release(m_pTextureCom_for_Toodee);
#pragma endregion
}
