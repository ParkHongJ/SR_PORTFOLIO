#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"
#include "KeyMgr.h"
CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs, CTransform::TRANSFORMDESC * pArg)
	: CCamera(rhs, pArg)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	//m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.5f, 15.f, 8.7f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(14.5f, 16.7f, 7.9f));
	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if ((pGameInstance->Get_DIKState(DIK_W) & 0x80) && pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if ((pGameInstance->Get_DIKState(DIK_S) & 0x80) && pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if ((pGameInstance->Get_DIKState(DIK_A) & 0x80) && pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if ((pGameInstance->Get_DIKState(DIK_D) & 0x80) && pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if ((pGameInstance->Get_DIKState(DIK_RETURN) & 0x80) && pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), (fTimeDelta * 0.5f));
	}

	_long	MouseMove = 0;

	if (pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), MouseMove * fTimeDelta * 0.05f);
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * 0.05f);
		}
	}

	if (pGameInstance->Get_DIMKeyState(DIMK_WHEEL))
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			vPos += _float3(1.0f, 0.0f, 0.0f) * MouseMove * fTimeDelta * 0.5f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			vPos -= _float3(0.0f, 1.0f, 0.0f) * MouseMove * fTimeDelta * 0.5f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
	}
	
	//if (CKeyMgr::Get_Instance()->Key_Down('X'))
	//{
	//	m_bMove = !m_bMove;
	//}
	//if (m_bMove)
	//{
	//	m_pTransformCom->Set_State(
	//		CTransform::STATE_POSITION,
	//		Lerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
	//			_float3(14.5f, 16.7f, 0.f),
	//			fTimeDelta * m_fSpeed));
	//}
	//else
	//{
	//	m_pTransformCom->Set_State(
	//		CTransform::STATE_POSITION,
	//		Lerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
	//			_float3(14.5f, 16.7f, 7.9f),
	//			fTimeDelta * m_fSpeed));
	//}
	//m_pTransformCom->LookAt(_float3(14.5f, -1.f, 8.0f));
	Safe_Release(pGameInstance);
	__super::Tick(fTimeDelta);
}

void CCamera_Free::LateTick(_float fTimeDelta)
{

}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*		pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void* pArg)
{
	CCamera::CAMERADESC*		pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Free*		pInstance = new CCamera_Free(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
_float3 CCamera_Free::Lerp(_float3 vPos, _float3 vTargetPos, _float fTimeDelta)
{
	//a + (b - a) * t.
	return vPos + (vTargetPos - vPos) * fTimeDelta;
}

void CCamera_Free::Free()
{
	__super::Free();
	CKeyMgr::Get_Instance()->Destroy_Instance();
}

