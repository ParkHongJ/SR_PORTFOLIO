#include "..\Public\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCamera::CCamera(const CCamera & rhs, CTransform::TRANSFORMDESC* pArg)
	: CGameObject(rhs)
	, m_pTransformCom((CTransform*)rhs.m_pTransformCom->Clone(pArg))
{

}

HRESULT CCamera::Initialize_Prototype()
{
	m_pTransformCom = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	//_float3 vPos = { 16.0f,5.0f,9.0f };
	//_float3 vLook = { 16.0f,0.f,9.0f };

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	//m_pTransformCom->LookAt(vLook);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);
	m_pTransformCom->LookAt(m_CameraDesc.vAt);


	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	_float4x4		ViewMatrix, ProjMatrix;

	ViewMatrix = m_pTransformCom->Get_WorldMatrixInverse();	
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);

	/*if (GetKeyState('X') & 0x8000)
	{
		test = !test;
	}

	if (test)
	{
		D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	}
	else
		D3DXMatrixOrthoLH(&ProjMatrix, 34.f, 18.f, m_CameraDesc.fNear, m_CameraDesc.fFar);
*/
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);
}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
