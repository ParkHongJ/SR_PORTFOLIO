#include "stdafx.h"
#include "..\Public\MyBox.h"

#include "GameInstance.h"

CMyBox::CMyBox(LPDIRECT3DDEVICE9 pGraphic_Device) :	CGameObject(pGraphic_Device)
{
}

CMyBox::CMyBox(const CMyBox & rhs) : CGameObject(rhs)
{
}

HRESULT CMyBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMyBox::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;



	return S_OK;
}

void CMyBox::Tick(_float fTimeDelta)
{
	int a = 10;

}

void CMyBox::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMyBox::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Render();
	// 리스트박스
	ImGui::Begin("Cube");
	const char* items[] = { "Position", "Scale" };
	static int item_current_idx{ 0 }, Radio_Text_Or_Slider{ 0 }; // Here we store our selection data as an index.
	
	ImGui::RadioButton("TextMode", &Radio_Text_Or_Slider, 0); ImGui::SameLine();
	ImGui::RadioButton("SliderMode", &Radio_Text_Or_Slider, 1); 
	
	ImGui::Spacing();
	_float3 vPosition{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) }, vScale{ m_pTransformCom->Get_Scaled() };
	if (ImGui::BeginListBox("listbox_Cube"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				item_current_idx = n;

			if (is_selected) {
				if (n == 0)
				{
					if (Radio_Text_Or_Slider == 1) {
						ImGui::SliderFloat("floatx", &vPosition.x, -100.0f, 100.0f);         
						ImGui::SliderFloat("floaty", &vPosition.y, -100.0f, 100.0f);         
						ImGui::SliderFloat("floatz", &vPosition.z, -100.0f, 100.0f);         
					}
					else if (Radio_Text_Or_Slider == 0)
					{
						ImGui::InputFloat("floatx",&vPosition.x);
						ImGui::InputFloat("floaty",&vPosition.y);
						ImGui::InputFloat("floatz",&vPosition.z);
					}
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
				}
				else if (n == 1)
				{
					if (Radio_Text_Or_Slider == 1) {
						ImGui::SliderFloat("ScaleX", &vScale.x, -10.0f, 10.0f);
						ImGui::SliderFloat("ScaleY", &vScale.y, -10.0f, 10.0f);
						ImGui::SliderFloat("ScaleZ", &vScale.z, -10.0f, 10.0f);
					}
					else if (Radio_Text_Or_Slider == 0)
					{
						ImGui::InputFloat("ScaleX", &vScale.x);
						ImGui::InputFloat("ScaleY", &vScale.y);
						ImGui::InputFloat("ScaleZ", &vScale.z);
					}
					m_pTransformCom->Set_Scale(vScale);
				}
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
	ImGui::End();

	return S_OK;


}

HRESULT CMyBox::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Cube"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	

	return S_OK;
}

CMyBox * CMyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMyBox* pInstance = new CMyBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMyBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMyBox::Clone(void * pArg)
{
	CMyBox*		pInstance = new CMyBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CMyBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyBox::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}

