#include "stdafx.h"
#include "..\Public\Topdee.h"

#include "GameMgr.h"
#include "Block.h"
#include "Wall.h"
CTopdee::CTopdee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{
}

CTopdee::CTopdee(const CTopdee & rhs)
	: CLandObject(rhs)
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

	/* For.Topdee_Col */
	SetTag(L"Topdee");
	
#pragma region WhiteRect
	_float3 vPreLoaderPos = m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION);

	vPreLoaderPos.y += 1.f;
	m_pTransform_PreLoader_Com->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransform_PreLoader_Com->Set_State(CTransform::STATE_POSITION, _float3(vPreLoaderPos));
#pragma endregion WhiteRect

	/* For.Portal_Data */
	CGameMgr::Get_Instance()->Set_Object_Data(L"Topdee_Portal", &m_bPortal);
	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)pArg);
	}

	return S_OK;
}

void CTopdee::Tick(_float fTimeDelta)
{
	
	if (!m_bActive)
		return;
	if (m_bPushBox)
		m_fPushBoxDelayTimer += fTimeDelta;

	_float TopdeeSpeed = m_pTransformCom->Get_Speed();
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	ClearCheck(fTimeDelta);
	KKK_IsRaise(fTimeDelta, 1);
	Topdee_PreLoader_Pos_Mgr();
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)
	{
		_float3 vTargetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		if (CGameMgr::Get_Instance()->Key_Pressing(DIK_UP))
		{
			Move_Frame(DIR_UP);
			m_vTargetDir = { 0.f, 0.f, 1.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed *fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_DOWN))
		{
			Move_Frame(DIR_DOWN);
			m_vTargetDir = { 0.f, 0.f, -1.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_LEFT))
		{
			Move_Frame(DIR_LEFT);
			m_vTargetDir = { -1.f, 0.f, 0.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (CGameMgr::Get_Instance()->Key_Pressing(DIK_RIGHT))
		{
			Move_Frame(DIR_RIGHT);
			m_vTargetDir = { 1.f, 0.f, 0.f };
			vTargetPos = m_vTargetDir * TopdeeSpeed * fTimeDelta;
			m_pTransformCom->Translate(vTargetPos);
			m_bPress = true;
		}
		else if (CGameMgr::Get_Instance()->Key_Down(DIK_Z))
		{//박스들기.
			KKK_DropBox(fTimeDelta);
			KKK_FindBox(fTimeDelta);
			m_bPress = true;
		}
		else
			m_bPress = false;
	}
	else
		Not_My_Turn_Texture();
	if (!m_bPress)	//no keyInput Go Lerp
		Go_Lerp(fTimeDelta);
	
	Safe_Release(pGameInstance);
}

void CTopdee::ClearCheck(_float fTimeDelta)
{
	if (m_bPortal)
	{
		if (CGameMgr::Get_Instance()->Get_Object_Data(L"Topdee_Portal")) {
			m_eCurState = STATE_CLEAR;
			m_iFrame = 17;
			m_fClear_Timer = fTimeDelta;
		}
	}
	else return;
	
	if(m_eCurState == STATE_CLEAR)
	{
		m_fClear_Timer += fTimeDelta;
		m_iFrame += _uint(m_fClear_Timer);
	}
}

void CTopdee::Go_Lerp(_float fTimeDelta)
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vFinalPosition;

	vFinalPosition.x = _int(vCurPosition.x) + 0.5f;
	if(CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE)
		vFinalPosition.y = m_MyTurnY;
	else
		vFinalPosition.y = m_NotMyTurnY;
	vFinalPosition.z = _int(vCurPosition.z) + 0.5f;

	vCurPosition = vCurPosition + (vFinalPosition - vCurPosition) * (fTimeDelta * 5);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
}

void CTopdee::Topdee_PreLoader_Pos_Mgr()
{
	_float3 vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_int iPreLoaderPosX{_int(vCurPosition.x)}, iPreLoaderPosY{ _int(vCurPosition.y) }, iPreLoaderPosZ{ _int(vCurPosition.z) };
	if (m_eCurDir == DIR_DOWN) 
		--iPreLoaderPosZ;
	else if (m_eCurDir == DIR_UP) 
		++iPreLoaderPosZ;
	else if (m_eCurDir == DIR_LEFT) 
		--iPreLoaderPosX;
	else if (m_eCurDir == DIR_RIGHT)
		++iPreLoaderPosX;
	
	m_pTransform_PreLoader_Com->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//_float iPreLoaderPosY = m_OriPreLoaderY;
	m_pTransform_PreLoader_Com->Set_State(CTransform::STATE_POSITION, _float3(_float(iPreLoaderPosX)+ 0.5f, _float(iPreLoaderPosY), _float(iPreLoaderPosZ) + 0.5f));
}

void CTopdee::Not_My_Turn_Texture()
{
	if (m_eCurDir == DIR_DOWN) 
		m_iFrame = 14;
	else if (m_eCurDir == DIR_UP) 
		m_iFrame = 16;
	else if (m_eCurDir == DIR_LEFT) 
		m_iFrame = 15;
	else if (m_eCurDir == DIR_RIGHT)
		m_iFrame = 13;
}

void CTopdee::KKK_IsRaise(_float fTimeDelta, _char KKK_NotOverride)
{
	if (m_pRaiseObject == nullptr)
		return;
	if (m_fRaising_Box_DelayTimer == 15000.f) {
		//falling
		KKK_DropBox(fTimeDelta);
		return;
	}
	m_fRaising_Box_DelayTimer += fTimeDelta;
	_float3 vfinalPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vfinalPos.y += 1.f;
	m_pRaiseObject->KKK_Is_Raise(vfinalPos);
}

void CTopdee::Move_Frame(const TOPDEE_DIRECTION& _eInputDirection)
{
	/*topdee ??? ?? 
	left
	 0, 1 Topdee idle
	 2, 3 Topdee Left
	 4, 5 Topdee Up
	 6, 7 Topdee Right
	 8~12 Jump
	 17 ~ 33 Clear
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
			}

			else if (m_eCurDir == DIR_UP) {
				m_iFirstFrame = 4;
			}

			else if (m_eCurDir == DIR_LEFT) {
				m_iFirstFrame = 2;
			}

			else if (m_eCurDir == DIR_RIGHT)
			{
				m_iFirstFrame = 6;
			}
			m_iFrame = m_iFirstFrame;
			m_bMoveFrame = false;
		}
	}

	
}

void CTopdee::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
#pragma region BillBoard
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	//========================================================================
	m_pRenderer_PreLoader_Com->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	//========================================================================
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//vPos.y = 0.5f;
#pragma endregion BillBoard
#pragma region Collision_Obstacle
	_float fCollisionDist;
	if (CGameMgr::Get_Instance()->Check_Not_Go(vPos, &fCollisionDist,false))
	{
		if (m_eCurDir == DIR_LEFT)
			vPos.x -= fCollisionDist;
		else if (m_eCurDir == DIR_RIGHT)
			vPos.x += fCollisionDist;
		else if (m_eCurDir == DIR_UP)
		 	vPos.z += fCollisionDist;
		else if (m_eCurDir == DIR_DOWN)
			vPos.z -= fCollisionDist;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
#pragma endregion Collision_Obstacle	
	m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER,m_pBoxCom, m_pTransformCom);


}

HRESULT CTopdee::Render()
{
	if (!m_bActive)
		S_OK;
#pragma region Debug_Collider
	Set_ColliderState();
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pBoxCom->Render(Matrix);
	Reset_ColliderState();
#pragma endregion Debug
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_Texture(m_iFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;
#pragma region PreLoader
	if (CGameMgr::Get_Instance()->GetMode() == CGameMgr::TOPDEE) {
		if (FAILED(m_pTransform_PreLoader_Com->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pTexture_PreLoader_Com->Bind_Texture(0)))
			return E_FAIL;

		if (FAILED(Set_RenderState()))
			return E_FAIL;
		m_pBuffer_PreLoader_Com->Render();
		if (FAILED(Reset_RenderState()))
			return E_FAIL;
	}
#pragma endregion PreLoader

	return S_OK;
}
void CTopdee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
	
	
}

void CTopdee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{// 여기사 나오는 디렉션은 밀려 나가는 디렉션임.
	if (other->CompareTag(L"Portal"))
	{
		m_bPortal = true;

	}
	else if (other->CompareTag(L"Pig"))
	{

	}
	else if (other->CompareTag(L"Box"))
	{
		if (!other->IsEnabled())
			return;
		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//부딪힌 상자.
		TopdeeIsPushed(vOtherPos);
	/*	if (m_pRaiseObject != nullptr)
			return;*/
		if (!m_bPushBox) {//처음들어올때.
			m_fPushBoxDelayTimer += fTimeDelta;
		}
		else if ((m_bPushBox) && (m_fPushBoxDelayTimer < 0.5f)) { //처음 실행되었고
			return;
		}
		else if((m_bPushBox)&&(m_fPushBoxDelayTimer > 0.5f)){
			m_fPushBoxDelayTimer = 0.f;
			m_bPushBox = false;
		}
		
		if (KKK_m_pBoxList == nullptr)
		{//if Collision We Must Check NextBox.
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CLayer* pLayer = pGameInstance->KKK_GetBox();
			if (pLayer == nullptr)
				return;
			KKK_m_pBoxList = pLayer->KKK_Get_List();
		}
		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurDir == DIR_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurDir == DIR_UP)
			vCurDir.z = 1.f;
		else if (m_eCurDir == DIR_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurDir == DIR_LEFT)
			vCurDir.x = -1.f;
		vOtherPos += vCurDir;//이게 민 박스의 다음 체크해야할 박스의 위치.
		//9.5 8.5 7.5
		_uint iCount = 0;
		CBlock* pBlock = dynamic_cast<CBlock*>(other);
		if (pBlock == nullptr)		//지금미는 블록이 벽이니?
			return;
		list<CGameObject*> PushList;
		_bool bCanPush{ true };
		FindCanPushBoxes(vOtherPos, vCurDir, iCount, PushList, bCanPush);
		if (!bCanPush)
			return;
		_float fdist{ 0.f };
		if (CGameMgr::Get_Instance()->Check_Not_Go(vOtherPos, &fdist,true)) {
			return;
		}
		pBlock->Box_Push_More(fTimeDelta, vOtherPos,vCurDir);
		for (auto& iter = PushList.begin(); iter != PushList.end(); ++iter)
		{
			CBlock* pBlock= (CBlock*)(*iter);
			CTransform* pTransform = (CTransform*)pBlock->Get_Component(L"Com_Transform");
			pBlock->Box_Push_More(fTimeDelta, ((pTransform->Get_State(CTransform::STATE_POSITION) + vCurDir)), vCurDir);
		}
		m_bPushBox = true;
	}
}

void CTopdee::TopdeeIsPushed(const _float3 _vOtherPos)
{
	_float3 vTopdeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fDist = D3DXVec3Length(&(vTopdeePos - _vOtherPos));
	fDist *= 0.25f;
	if (m_eCurDir == DIR_LEFT)
		vTopdeePos.x += fDist;
	else if (m_eCurDir == DIR_RIGHT)
	 	vTopdeePos.x -= fDist;
	else if (m_eCurDir == DIR_UP)
		vTopdeePos.z -= fDist;
	else if (m_eCurDir == DIR_DOWN)
		vTopdeePos.z += fDist;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTopdeePos);
}

void CTopdee::FindCanPushBoxes(_float3 _vNextBoxPos,_float3 vPushDir, _uint& iCountReFunc, list<CGameObject*>& PushList, _bool& bCanPush)
{//들어온값은 다음 박스에 해당.
	if (!bCanPush)
		return;
	auto& iter = KKK_m_pBoxList->begin();
	for (_uint i = 0; i < KKK_m_pBoxList->size(); ++i)
	{
		CTransform* pNextBlock = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
		_float3 vNextBlockPos = pNextBlock->Get_State(CTransform::STATE_POSITION);
		if (vPushDir.x == 0.f)
		{//Up or Down
			if ((_int)_vNextBoxPos.z == (_int)vNextBlockPos.z)//찾으려는값임.
			{
				if ((_int)_vNextBoxPos.x == (_int)vNextBlockPos.x) {
					_vNextBoxPos += vPushDir;
					++iCountReFunc;
					
					_float3 vNextBoxPosFix{ ((_uint)_vNextBoxPos.x + 0.5f),((_uint)_vNextBoxPos.y + 0.5f) ,((_uint)_vNextBoxPos.z + 0.5f) };
					_float fdist{ 0.f };
					if (CGameMgr::Get_Instance()->Check_Not_Go(vNextBoxPosFix, &fdist,true)){
						bCanPush = false;
						return;
					}
					PushList.push_back(*iter);
					FindCanPushBoxes(_vNextBoxPos, vPushDir, iCountReFunc,PushList, bCanPush);
					break;
				}
			}
				
		}
		else if (vPushDir.z == 0.f)
		{
			if ((_int)_vNextBoxPos.x == (_int)vNextBlockPos.x)//찾으려는값임.
			{
				if ((_int)_vNextBoxPos.z == (_int)vNextBlockPos.z) {
					_vNextBoxPos += vPushDir;
					++iCountReFunc;
					_float3 vNextBoxPosFix{ ((_uint)_vNextBoxPos.x + 0.5f),((_uint)_vNextBoxPos.y + 0.5f) ,((_uint)_vNextBoxPos.z + 0.5f) };
					_float fdist{ 0.f };
					if (CGameMgr::Get_Instance()->Check_Not_Go(vNextBoxPosFix, &fdist,true)) {
						bCanPush = false;
						return;
					}
					PushList.push_back(*iter);
					FindCanPushBoxes(_vNextBoxPos, vPushDir, iCountReFunc, PushList, bCanPush);
					break;
				}
			}
		}
		++iter;
	}
}

void CTopdee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
	m_bPortal = false;
	m_eCurState = STATE_IDLE;
}

HRESULT CTopdee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	//?? ??? ??????
	return S_OK;
}

HRESULT CTopdee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CTopdee::Set_ColliderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return S_OK;
}

HRESULT CTopdee::Reset_ColliderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
		return E_FAIL;

	//=================================================================
	/* For.PreLoader */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	/* For.Com_Renderer_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer_PreLoader"), (CComponent**)&m_pRenderer_PreLoader_Com, this)))
		return E_FAIL;
	/* For.Com_VIBuffer_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_PreLoader"), (CComponent**)&m_pBuffer_PreLoader_Com, this)))
		return E_FAIL;
	/* For.Com_Transform_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_PreLoader"), (CComponent**)&m_pTransform_PreLoader_Com, this, &TransformDesc)))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee_PreLoader"), TEXT("Com_Texture_PreLoader"), (CComponent**)&m_pTexture_PreLoader_Com, this)))
		return E_FAIL;
	//=================================================================
	

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, this, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, this)))
		return E_FAIL;

	CBoxCollider::BOXDESC BoxColliderDesc;
	ZeroMemory(&BoxColliderDesc, sizeof(BoxColliderDesc));

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	BoxColliderDesc.fRadius = 0.5f;
	BoxColliderDesc.bIsTrigger = true;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;
	return S_OK;
}

void CTopdee::KKK_FindBox(_float fTimeDelta)
{
	if (m_pRaiseObject != nullptr)
		return;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (KKK_m_pBoxList == nullptr) {

		CLayer* pLayer = pGameInstance->KKK_GetBox();
		if (pLayer == nullptr)
			return;
		KKK_m_pBoxList = pLayer->KKK_Get_List();
	}
	_float3 vTopdeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vTopdeePos.y += 1.f;

	auto& iter = (*KKK_m_pBoxList).begin(); 
	bool bMove{false};
	_float3 vPreLoaderPos = m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION);
	for (_uint i = 0; i < (*KKK_m_pBoxList).size(); ++i)
	{
		bMove = (*iter)->KKK_Go_Lerp_Raise(vTopdeePos, fTimeDelta, vPreLoaderPos);
		if (bMove)
			break;
		++iter;
	}

	if (!bMove)
		m_pRaiseObject = nullptr;
	else {
		m_fRaising_Box_DelayTimer = fTimeDelta;
		m_pRaiseObject = (*iter);
		m_pRaiseObject->SetEnabled(false);
	}

	Safe_Release(pGameInstance);
}

void CTopdee::KKK_DropBox(_float fTimeDelta)
{
	if (m_pRaiseObject == nullptr)
		return;
	if (_int(m_fRaising_Box_DelayTimer) < 1)
		return;
	
	if (m_vBoxDropPos == _float3(-1.f, -1.f, -1.f)) {
		m_fRaising_Box_DelayTimer = 15000.f;
		m_vBoxDropPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vBoxDropPos.y = 0.5f;
		if (m_eCurDir == DIR_UP)
			m_vBoxDropPos.z += 1.f;
		else if (m_eCurDir == DIR_LEFT)
			m_vBoxDropPos.x -= 1.f;
		else if (m_eCurDir == DIR_RIGHT)
			m_vBoxDropPos.x += 1.f;
		else if (m_eCurDir == DIR_DOWN)
			m_vBoxDropPos.z -= 1.f;
	}
	
	if (m_pRaiseObject->KKK_Go_Lerp_Drop(m_vBoxDropPos, fTimeDelta,false)) {
		_uint iLayerHoleNum{ 0 };
		if (CGameMgr::Get_Instance()->Check_Box_Down(m_pTransform_PreLoader_Com->Get_State(CTransform::STATE_POSITION), &iLayerHoleNum, &m_eHoleLevel))
		{
			CLayer* pLayer = CGameInstance::Get_Instance()->Get_Layer(L"Layer_Hole", m_eHoleLevel);
			if (pLayer == nullptr)
				return;
			list<CGameObject*>*pList = pLayer->KKK_Get_List();
			auto& iter = pList->begin();
			for (_uint i = 0; i < iLayerHoleNum; ++i)
				++iter;
			((CTransform*)(*iter)->Get_Component(L"Com_Transform"))->Set_State(CTransform::STATE_POSITION, _float3{ -100.f,-100.f,-100.f });	//it Makes Dont Make Bomb
			m_pRaiseObject->KKK_Go_Lerp_Drop(_float3(0.f,0.f,0.f), fTimeDelta, true);//it can Make Drop More.
		}
		m_pRaiseObject->SetEnabled(true);
		m_pRaiseObject = nullptr;
		m_fRaising_Box_DelayTimer = 0.f;
		m_vBoxDropPos = _float3(-1.f, -1.f, -1.f);
	}
}

CTopdee * CTopdee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTopdee*		pInstance = new CTopdee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		
		(TEXT("Failed To Created : CTopdee"));
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
	m_pRaiseObject = nullptr;
	__super::Free();
	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

	Safe_Release(m_pBuffer_PreLoader_Com);
	Safe_Release(m_pTransform_PreLoader_Com);
	Safe_Release(m_pRenderer_PreLoader_Com);
	Safe_Release(m_pTexture_PreLoader_Com);

}
