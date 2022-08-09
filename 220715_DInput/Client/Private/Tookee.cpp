#include "stdafx.h"
#include "..\Public\Tookee.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Hong.h"
#include "Interaction_Block.h"
CTookee::CTookee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTookee::CTookee(const CTookee & rhs)
	: CGameObject(rhs)
{
}
HRESULT CTookee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTookee::Initialize(void * pArg)
{
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/* For.Toodee_Col */
	SetTag(L"Tookee");

	_float3 vPos = ObjInfo.vPos;
	vPos.z += 3.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	//Register Tookee
	CGameMgr::Get_Instance()->SetTookee(this);
	return S_OK;
}

void CTookee::Tick(_float fTimeDelta)
{
	//������
	m_eCurMode = CGameMgr::Get_Instance()->GetMode();

	//������� ������带 ���ؼ� ����
	if (m_eCurMode == m_ePreMode)
	{
		//��尡 �ȹٲ�
	}
	else
	{
		//��尡 �ٲ����
		if (m_eCurMode == CGameMgr::TOPDEE)
		{
			//���� �ٲ��尡 ž��� ����
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			//���� �ʿ���. ���𿡼� ž��� �ٲ� �ʹ� ���ٲ�
			vPos.x = _uint(vPos.x) + 0.5f;
			vPos.z = _uint(vPos.z) + 0.5f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}
		m_ePreMode = m_eCurMode;

		/* ��Ű������ �ʱ�ȭ */
		m_eCurState = TOOKEE_IDLE;
		//������ �ʱ�ȭ
		m_bJump = false;
		m_fJumpTime = 0.f;
	}
}

void CTookee::LateTick(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case CTookee::TOOKEE_LEFT:
		m_fSpeed = 5.f;
		m_pTransformCom->Set_Scale(_float3(-1.f, 1.f, 1.f));
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_iMinFrame = 13;
			m_iMaxFrame = 24;
		}
		break;
	case CTookee::TOOKEE_RIGHT:
		m_fSpeed = 5.f;
		m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_iMinFrame = 13;
			m_iMaxFrame = 24;
		}
		break;
	case CTookee::TOOKEE_UP:
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_DOWN:
		m_fSpeed = 5.f;
		break;
	case CTookee::TOOKEE_JUMP:
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_iMinFrame = 25;
			m_iMaxFrame = 29;
		}
		break;
	case CTookee::TOOKEE_IDLE:
		if (m_eCurMode == CGameMgr::TOODEE)
		{
			m_fSpeed = 0.f;
			m_iMinFrame = 0;
			m_iMaxFrame = 12;
		}
		else
			m_fSpeed = 5.f;
		break;
	default:
		break;
	}

	if (m_eCurMode == CGameMgr::TOODEE)
	{
		if (m_bJump && m_iMaxFrame - 1 < m_iTexIndex)
			m_iTexIndex = m_iMaxFrame;
		else {
			if (m_iMinFrame > m_iTexIndex || m_iMaxFrame - 1 < m_iTexIndex)
				m_iTexIndex = m_iMinFrame;
			else {
				m_fFrame += m_iMaxFrame * fTimeDelta;

				if (m_fFrame > (m_iMaxFrame / 8)) {
					++m_iTexIndex;
					m_fFrame = 0.f;
				}
			}
		}
	}
	

	/* TOODEE */
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		//To do TOODEE
		Jump(fTimeDelta);
		m_pTransformCom->Set_TransformDesc_Speed(m_fSpeed);
		m_pTransformCom->Go_Straight_2D(fTimeDelta);
	}
	/* TOPDEE */
	else
	{
		//To do TOPDEE
		_float fCollisionDist;

		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float3 vCurDir{ 0.f,0.f,0.f };
		if (m_eCurState == TOOKEE_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurState == TOOKEE_UP)
			vCurDir.z = 1.f;
		else if (m_eCurState == TOOKEE_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurState == TOOKEE_LEFT)
			vCurDir.x = -1.f;

		if (CGameMgr::Get_Instance()->Check_Not_Go(vPos, vCurDir, &fCollisionDist, false))
		{
			//Edit Hong
			if (fCollisionDist != 0.f)
			{
				if (m_eCurState == TOOKEE_LEFT)
				{
					vPos.x += fCollisionDist;
				}
				else if (m_eCurState == TOOKEE_RIGHT)
				{
					vPos.x -= fCollisionDist;
				}
				else if (m_eCurState == TOOKEE_UP)
				{
					vPos.z -= fCollisionDist;
				}
				else if (m_eCurState == TOOKEE_DOWN)
				{
					vPos.z += fCollisionDist;
				}
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			}
		}
	}

	m_pColliderCom->Add_CollisionGroup(CCollider::PLAYER, m_pBoxCom, m_pTransformCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTookee::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_bActive) {
		if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
			return E_FAIL;
	}

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//---------------------������϶� �׸���-------------------------
	_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxCom->Render(Matrix);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//--------------------------------------------------------------

	return S_OK;
}

void CTookee::OnTriggerEnter(CGameObject * other, _float fTimeDelta)
{
}

void CTookee::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDireciton)
{
	_float fBoxSize = 1.f;
	_float fMyLength = 1.5f;


	/* TOODEE */
	if (m_eCurMode == CGameMgr::TOODEE)
	{
		if (other->CompareTag(L"Box") || other->CompareTag(L"Wall")) {
			CTransform* TargetBox = (CTransform*)other->Get_Component(L"Com_Transform");
			Safe_AddRef(TargetBox);

			_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vBoxPos = TargetBox->Get_State(CTransform::STATE_POSITION);

			if (CCollider::DIR_UP == eDireciton) {
				if (0.2f < m_fJumpTime && m_bJump)
					m_bJump = false;
				m_fJumpTime = 0.f;
				m_eCurState = TOOKEE_IDLE;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vBoxPos.z + (fBoxSize * 0.5f)));
			}
			else if (CCollider::DIR_DOWN == eDireciton) {
				if (fMyLength > abs(vMyPos.z - vBoxPos.z))
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vMyPos.x, vMyPos.y, vMyPos.z - (fMyLength - abs(vMyPos.z - vBoxPos.z) - 0.001f)));
			}
			else if (CCollider::DIR_LEFT == eDireciton) {
				if (m_eCurState == TOOKEE_RIGHT)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				//need fix
				else if (m_fSpeed > 0.f && m_eCurState == TOOKEE_JUMP)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				else if (m_eCurState == TOOKEE_IDLE)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			}
			else if (CCollider::DIR_RIGHT == eDireciton) {
				if (m_eCurState == TOOKEE_LEFT)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				//need fix
				else if (m_fSpeed > 0.f && m_eCurState == TOOKEE_JUMP)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
				else if (m_eCurState == TOOKEE_IDLE)
					m_pTransformCom->Go_Straight_2D(-fTimeDelta);
			}

			Safe_Release(TargetBox);
		}
	}
	/* TOPDEE */
	else if (m_eCurMode == CGameMgr::TOPDEE && other->CompareTag(L"Box"))
	{
		//To do TOPDEE
		//�̰� ��ġ �񱳷ε� ����.
		CInteraction_Block* pInteraction_Block = dynamic_cast<CInteraction_Block*>(other);

		if (pInteraction_Block == nullptr)
			return;

		CTransform* pTransform = (CTransform*)(other->Get_Component(L"Com_Transform"));
		_float3 vOtherPos = pTransform->Get_State(CTransform::STATE_POSITION);//�ε��� ����.
		TopdeeIsPushed(vOtherPos);//��Ű�� �з����°�.

		if (vOtherPos.y != 0.5f)
			return;
		
		if (m_pBoxList == nullptr)
		{
			//if Collision We Must Check NextBox.
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			m_pBoxList = pGameInstance->Get_Instance()->GetLayer(m_iNumLevel, L"Layer_Cube");
			if (m_pBoxList == nullptr)
				return;
		}

		_float3 vCurDir = { 0.f,0.f,0.f };

		if (m_eCurState == TOOKEE_DOWN)
			vCurDir.z = -1.0f;
		else if (m_eCurState == TOOKEE_UP)
			vCurDir.z = 1.f;
		else if (m_eCurState == TOOKEE_RIGHT)
			vCurDir.x = 1.f;
		else if (m_eCurState == TOOKEE_LEFT)
			vCurDir.x = -1.f;

		vOtherPos += vCurDir;//�̰� �� �ڽ��� ���� üũ�ؾ��� �ڽ��� ��ġ.
		
		_uint iCount = 0;

		CInteraction_Block* pBlock = dynamic_cast<CInteraction_Block*>(other);
		//���� �浹�� ������ ���̶��
		if (pBlock == nullptr) 
			return;

		list<CGameObject*> PushList;
		_bool bCanPush = true;

		FindCanPushBoxes(vOtherPos, vCurDir, iCount, PushList, bCanPush);//list push back
		
		if (!bCanPush)
			return;
		
		_float fdist = 0.f;

		vOtherPos -= vCurDir;

		if (CGameMgr::Get_Instance()->Check_Not_Go(vOtherPos, vCurDir, &fdist, true)) {
			return;
		}

		vOtherPos += vCurDir;
		pBlock->Box_Push_More(fTimeDelta, vOtherPos, true);//First
														   //_uint iCount{ 0 };
		for (auto& iter = PushList.begin(); iter != PushList.end(); ++iter)
		{
			CInteraction_Block* pBlock = (CInteraction_Block*)(*iter);
			CTransform* pTransform = (CTransform*)pBlock->Get_Component(L"Com_Transform");
			_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
			pBlock->Box_Push_More(fTimeDelta, (vPos + vCurDir), true);
		}
		m_bPushBox = true;
	}
}

void CTookee::OnTriggerExit(CGameObject * other, _float fTimeDelta)
{
}

void CTookee::Move(STATE _eState, _float fTimeDelta)
{
	switch (_eState)
	{
	case CTookee::TOOKEE_LEFT:
		break;
	case CTookee::TOOKEE_RIGHT:
		break;
	case CTookee::TOOKEE_UP:
		break;
	case CTookee::TOOKEE_DOWN:
		break;
	case CTookee::TOOKEE_JUMP:
		break;
	case CTookee::TOOKEE_IDLE:
		break;
	case CTookee::TOOKEE_END:
		break;
	default:
		MSG_BOX(L"�߸��� �����Դϴ� : TOOKEE");
		break;
	}
}

void CTookee::Jump(_float fTimeDelta)
{
	_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float vGravityPower = -1.63f * m_fJumpTime * 0.5f;

	if (m_fDrop_Endline + abs(vGravityPower) <= fPos.z) {
		if (m_bJump)
			fPos.z += m_fJumpPower * fTimeDelta;

		fPos.z += vGravityPower;

		if (m_fJumpTime > m_fMaxJumpTime)
			m_fJumpTime = m_fMaxJumpTime;
		else
			m_fJumpTime += fTimeDelta;
	}
	if (m_fDrop_Endline + abs(vGravityPower) > fPos.z)
	{
		m_bJump = false;
		m_fJumpTime = 0.f;
		fPos.z = m_fDrop_Endline;
		m_eCurState = TOOKEE_IDLE;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
}

void CTookee::SetScale(_float3 _vScale)
{
	m_pTransformCom->Set_Scale(_vScale);
}

void CTookee::SetPosition(_float fTimeDelta, _float3 vDir)
{
	m_pTransformCom->Translate(vDir * fTimeDelta * m_fSpeed);
}

void CTookee::CreateEffect()
{
	//Hong Edit For Effect
	for (int i = 0; i < 10; i++) {
		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(-.5f, .5f);
		//random float

		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vPos2 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x += distr(eng);
		vPos.z += distr(eng);
		CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
			vPos,
			vPos - vPos2,
			CParticleMgr::PARTICLE);
	}
}

HRESULT CTookee::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 120);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CTookee::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CTookee::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer_Toodee_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_Tookee"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

	BoxColliderDesc.vPos = _float3(0.f, 0.f, 0.5f);
	BoxColliderDesc.vSize = _float3(0.5f, 0.5f, 1.f);
	BoxColliderDesc.fRadius = 1.f;

	BoxColliderDesc.bIsTrigger = true;

	/* For.Com_BoxCollider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), TEXT("Com_BoxCollider"), (CComponent**)&m_pBoxCom, this, &BoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CTookee * CTookee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTookee* pInstance = new CTookee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTookee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTookee::Clone(void * pArg)
{
	CTookee* pInstance = new CTookee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CTookee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTookee::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

void CTookee::TopdeeIsPushed(const _float3 _vPos)
{
	_float3 vTookeePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	_float fDist = D3DXVec3Length(&(vTookeePos - _vPos));
	
	fDist *= 0.2f;
	
	if (m_eCurState == TOOKEE_LEFT)
		vTookeePos.x += fDist;
	else if (m_eCurState == TOOKEE_RIGHT)
		vTookeePos.x -= fDist;
	else if (m_eCurState == TOOKEE_UP)
		vTookeePos.z -= fDist;
	else if (m_eCurState == TOOKEE_DOWN)
		vTookeePos.z += fDist;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTookeePos);

}

void CTookee::FindCanPushBoxes(_float3 _vNextBoxPos, _float3 vPushDir, _uint& iCountReFunc, list<CGameObject*>& PushList, _bool& bCanPush)
{
	//���°��� ���� �ڽ��� �ش�. ����Ʈ�� ��ܾ��ϴ� ������� 2���̴�.
	if (!bCanPush)
		return;
	auto& iter = m_pBoxList->begin();
	for (_uint i = 0; i < m_pBoxList->size(); ++i)
	{
		CTransform* pNextBlock = (CTransform*)(*iter)->Get_Component(L"Com_Transform");
		if (pNextBlock == nullptr)
			return;
		_float3 vNextBlockPos = pNextBlock->Get_State(CTransform::STATE_POSITION);
		if (vNextBlockPos.y < 0.f)
		{
			++iter;
			continue;
		}
		if (vPushDir.x == 0.f)
		{
			//Up or Down
			if ((_int)_vNextBoxPos.z == (_int)vNextBlockPos.z)//ã�����°���.
			{
				if ((_int)_vNextBoxPos.x == (_int)vNextBlockPos.x) {
					++iCountReFunc;

					_float3 vNextBoxPosFix{ ((_uint)_vNextBoxPos.x + 0.5f),((_uint)_vNextBoxPos.y + 0.5f) ,((_uint)_vNextBoxPos.z + 0.5f) };
					_float fdist = 0.f;
					if (CGameMgr::Get_Instance()->Check_Not_Go(vNextBoxPosFix, vPushDir, &fdist, true)) {//WallCheck
						bCanPush = false;
						return;
					}
					_vNextBoxPos += vPushDir;
					PushList.push_back(*iter);
					FindCanPushBoxes(_vNextBoxPos, vPushDir, iCountReFunc, PushList, bCanPush);
					break;
				}
			}
		}
		else if (vPushDir.z == 0.f)
		{
			if ((_int)_vNextBoxPos.x == (_int)vNextBlockPos.x)//ã�����°���.
			{
				if ((_int)_vNextBoxPos.z == (_int)vNextBlockPos.z) {
					_vNextBoxPos += vPushDir;
					++iCountReFunc;
					_float3 vNextBoxPosFix{ ((_uint)_vNextBoxPos.x + 0.5f),((_uint)_vNextBoxPos.y + 0.5f) ,((_uint)_vNextBoxPos.z + 0.5f) };
					_float fdist = 0.f;
					if (CGameMgr::Get_Instance()->Check_Not_Go(vNextBoxPosFix, vPushDir, &fdist, true)) {
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
