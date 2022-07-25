#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.0f, 0.5f, 0.f));
	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_UP) & 0x8000)
	{
		//m_pTransformCom->Translate(_float3(0.f, 0.f, 1.f) * fTimeDelta * 3.f);
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}
	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Translate(_float3(0.f, 0.f, 1.f) *fTimeDelta*3.f);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Translate(_float3(0.f, 0.f, -1.f) *fTimeDelta*3.f);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);

		//m_pTransformCom->Translate(_float3(0.f, 0.f, -1.f) * fTimeDelta * 3.f);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{

		m_pTransformCom->Translate(_float3(-1.f, 0.f, 0.f) * fTimeDelta * 3.f);
		//m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		//m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pTransformCom->Translate(_float3(1.f, 0.f, 0.f) * fTimeDelta * 3.f);
	}
	
}

void CPlayer::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	m_pBoxCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------디버그일때 그리기-------------------------
	//m_pColliderCom->Render();
	//--------------------------------------------------------------

	_float3 temp = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	ImGui::Begin("Collider");
	_float3 tempMin = m_pBoxCom->GetMin();
	_float3 tempMax = m_pBoxCom->GetMax();

	_float4x4 matWorld;
	D3DXMatrixIdentity(&matWorld);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vScale = m_pTransformCom->Get_Scaled();
	memcpy(&matWorld.m[3][0], &vPos, sizeof(_float3));
	D3DXVec3TransformCoord(&tempMin, &tempMin, &matWorld);
	D3DXVec3TransformCoord(&tempMax, &tempMax, &matWorld);

	ImGui::SliderFloat3("tempmin", tempMin, -100.f, 100.f);
	ImGui::SliderFloat3("tempMax", tempMax, -100.f, 100.f);

	ImGui::End();

	ImGui::Begin("DirectX9 Texture Test");
	ImGui::Text("pointer = %p", my_texture);
	ImGui::Text("size = %d x %d", my_image_width, my_image_height);
	ImGui::Image((void*)my_texture, ImVec2(50, 50));
	ImGui::End();
	return S_OK;
}

HRESULT CPlayer::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	return S_OK;
}

HRESULT CPlayer::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_Box"), (CComponent**)&m_pBoxCom, this)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(my_texture);
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

bool CPlayer::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{

	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(m_pGraphic_Device, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

