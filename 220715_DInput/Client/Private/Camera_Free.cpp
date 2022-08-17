#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameMgr.h"
#include "GameInstance.h"
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

	m_vLookPos = _float3(14.5f, -1.f, 8.0f);
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(15.5f, 15.f, 8.7f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(14.5f, 16.7f, 7.9f));

	//이거 수정해라
	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_long	MouseMove = 0;

	if (pGameInstance->Get_DIMKeyState(DIMK_RBUTTON))
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), (_float)MouseMove * fTimeDelta * 0.05f);
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), (_float)MouseMove * fTimeDelta * 0.05f);
		}
	}

	if (pGameInstance->Get_DIMKeyState(DIMK_WHEEL))
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			vPos += _float3(1.0f, 0.0f, 0.0f) * (_float)MouseMove * fTimeDelta * 0.5f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			vPos -= _float3(0.0f, 1.0f, 0.0f) * (_float)MouseMove * fTimeDelta * 0.5f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
	}

	CGameMgr* pGameMgr = CGameMgr::Get_Instance();
	Safe_AddRef(pGameMgr);
	if (pGameMgr->GetMode() == CGameMgr::TOPDEE)
	{
		_float3 vPos = m_vLookPos;
		vPos.y = 16.7f;
		vPos.z = 0.f;
		m_pTransformCom->Set_State(
			CTransform::STATE_POSITION,
			Lerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
				vPos,
				fTimeDelta * m_fSpeed));
	}
	else
	{
		_float3 vPos = m_vLookPos;
		vPos.y = 16.7f;
		vPos.z = 7.9f;
		m_pTransformCom->Set_State(
			CTransform::STATE_POSITION,
			Lerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
				vPos,
				fTimeDelta * m_fSpeed));
	}
	if (pGameMgr->Key_Pressing(DIK_A))
	{
		m_vLookPos.x -= m_fSpeed * fTimeDelta;
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= m_fSpeed * fTimeDelta;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	if (pGameMgr->Key_Pressing(DIK_D))
	{
		m_vLookPos.x += m_fSpeed * fTimeDelta;
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x += m_fSpeed * fTimeDelta;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	//런모드라면
	if (CGameMgr::Get_Instance()->GetGameMode())
	{
		list<CGameObject*>* pListToodee = CGameInstance::Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Toodee");
		list<CGameObject*>* pListTopdee = CGameInstance::Get_Instance()->GetLayer(m_iNumLevel, L"Layer_topdee");
		if (pListTopdee ==nullptr)
		{
			MSG_BOX(L"pListTopdee가 널입니다");
			return;
		}
		if (pListToodee == nullptr)
		{
			MSG_BOX(L"pListToodee가 널입니다");
			return;
		}

		CGameObject* pTopdee = pListTopdee->front();
		CGameObject* pToodee = pListToodee->front();
		if (pTopdee == nullptr || pToodee == nullptr)
			return;

		CTransform* pTopdeeTrans = (CTransform*)(pTopdee->Get_Component(L"Com_Transform"));
		_float3 vTopdeePos = pTopdeeTrans->Get_State(CTransform::STATE_POSITION);

		CTransform* pToodeeTrans = (CTransform*)(pToodee->Get_Component(L"Com_Transform"));
		_float3 vToodeePos = pToodeeTrans->Get_State(CTransform::STATE_POSITION);


		_float3 vTarget;
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		vTarget = vPos;

		_float4x4 ViewMatrix;
		_float4x4 ProjMatrix;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
		_float3 vTopdeeProjPos = vTopdeePos;
		_float3 vToodeeProjPos = vToodeePos;
		D3DXVec3TransformCoord(&vTopdeeProjPos, &vTopdeeProjPos, &ViewMatrix);
		D3DXVec3TransformCoord(&vTopdeeProjPos, &vTopdeeProjPos, &ProjMatrix);

		D3DXVec3TransformCoord(&vToodeeProjPos, &vToodeeProjPos, &ViewMatrix);
		D3DXVec3TransformCoord(&vToodeeProjPos, &vToodeeProjPos, &ProjMatrix);

		_float vMiddleOffset = abs((vTopdeePos.x - vToodeePos.x) * 0.5f);
		//만약 탑디가 투디보다 앞에있냐
		if (vTopdeePos.x > vToodeePos.x)
		{
			vToodeePos.x += vMiddleOffset;
			vTarget.x = vToodeePos.x;
		}
		else
		{
			//투디가 탑디보다 앞에있냐
			vTopdeePos.x += vMiddleOffset;
			vTarget.x = vTopdeePos.x;
		}

		if (vTarget.x <= LeftOffset || vTarget.x >= RightOffset)
		{
			//offset이동
			_float3 vPosition = Lerp(vPos, vTarget, fTimeDelta * m_fSpeed);
			m_vLookPos.x += vPosition.x - vPos.x;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}
	m_pTransformCom->LookAt(m_vLookPos);

	Safe_Release(pGameMgr);
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

_float CCamera_Free::Lerp(_float fPos, _float fTargetPos, _float fTimeDelta)
{

	return fPos + (fTargetPos - fPos) * fTimeDelta;
}

void CCamera_Free::Free()
{
	__super::Free();
}

