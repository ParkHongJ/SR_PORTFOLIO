#include "stdafx.h"
#include "..\Public\WarpBlock.h"
#include "GameInstance.h"
#include "Hong.h"
#include "GameMgr.h"
#include "ParticleMgr.h"

CWarpBlock::CWarpBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteraction_Block(pGraphic_Device)
{
}

CWarpBlock::CWarpBlock(const CWarpBlock & rhs)
	: CInteraction_Block(rhs)
{
}

HRESULT CWarpBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWarpBlock::Initialize(void * pArg)
{
	// Warp Texture is 0: Up, 1: Right 2: Down, 3: Left    This GiJun is Warp Direction.
	CHong::OBJ_INFO ObjInfo;
	if (pArg != nullptr)
	{
		memcpy(&ObjInfo, pArg, sizeof(CHong::OBJ_INFO));
		m_iNumLevel = ObjInfo.iNumLevel;
	}
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetTag(L"WarpBlock");


	if (m_pTransformCom != nullptr && pArg != nullptr)
	{
		_float3 vPos;
		vPos = ObjInfo.vPos;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_eDir = (DIRECTION)ObjInfo.iDirection;
		m_iTextureNum = ObjInfo.iDirection;
	}
	else
	{
		_float3 vPos;
		memcpy(&vPos, pArg, sizeof(_float3));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	//텔레포트될 위치까지의 거리. 0.5로 주니까 딱붙는경우가 생겨서 매직넘버 붙임
	m_fWarpDistance = 0.9f;
	
	//초기 텔레포트 위치 설정
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	switch (m_eDir)
	{
	case CWarpBlock::DIR_UP:
		vPos.z += m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;
	case CWarpBlock::DIR_RIGHT:
		vPos.x += m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;
	case CWarpBlock::DIR_DOWN:
		vPos.z -= m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;
	case CWarpBlock::DIR_LEFT:
		vPos.x -= m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;

	default:
		break;
	}

	//게임매니저 등록
	if (FAILED(CGameMgr::Get_Instance()->RegisterWarpBlock(this)))
		E_FAIL;

	return S_OK;
}

void CWarpBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_fShaderTimer > 0.4f)
	{//쉐이더리셋
		m_fShaderTimer = 0.f;
		m_eShaderSelect = SHADER_DEFAULT;
	}
	//포탈을 들거나 미는중에도 텔레포트가 가능해야하기 때문에 틱마다 텔포의 위치를 조정해준다.
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	switch (m_eDir)
	{
	case CWarpBlock::DIR_UP:
		vPos.z += m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;
	case CWarpBlock::DIR_RIGHT:
		vPos.x += m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;
	case CWarpBlock::DIR_DOWN:
		vPos.z -= m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;
	case CWarpBlock::DIR_LEFT:
		vPos.x -= m_fWarpDistance;
		m_vTeleportPos = vPos;
		break;

	default:
		break;
	}
	if (m_eShaderSelect == SHADER_WARP)
		m_fShaderTimer += fTimeDelta;
	
}

void CWarpBlock::LateTick(_float fTimeDelta)
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
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pCollCom->Add_CollisionGroup(CCollider::INTEREACTION, m_pBoxCollider, m_pTransformCom);
}

HRESULT CWarpBlock::Render()
{
	if (!m_bActive)
		S_OK;

	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pTextureCom->Bind_Texture(m_pShaderCom, "g_Texture", m_iTextureNum);


	m_pShaderCom->Begin(m_eShaderSelect);//0 default, 1 InHole, 2 WarpOn

	m_pVIBufferCom->Render();

	m_pShaderCom->End();

	return S_OK;
}

void CWarpBlock::OnTriggerStay(CGameObject * other, _float fTimeDelta, _uint eDirection)
{
	//나 혹은 내 파트너의 능력이 비활성화 되어있다면
	if (!m_pPartner->m_bAbility || !m_bAbility || m_bTopdeeRaise ||m_pPartner == nullptr)
		return;

	//워프 방향으로 충돌했다면
	//이 부분은 한번만 호출됨!
	if (eDirection == m_eDir)
	{
		MakeSound(TEXT("potalInSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
		MakeEffect();

		CTransform* otherTransform = (CTransform*)other->Get_Component(L"Com_Transform");
		_float3 vPos = m_pPartner->GetTeleportPos();

		//이때 방향도 같이 주고싶다.
		if (CWarpBlock::DIR_DOWN == m_pPartner->GetDir()) {
			otherTransform->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y, vPos.z - 0.7f));
			MakeSound(TEXT("portalOutSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
			m_pPartner->MakeEffect();
		}
		else {
			otherTransform->Set_State(CTransform::STATE_POSITION, vPos);
			MakeSound(TEXT("portalOutSnd.wav"), C_FMOD::CHANNELID::EFFECT, SOUND_DEFAULT);
			m_pPartner->MakeEffect();
		}
	}
}

void CWarpBlock::Rotate_WarpBlock()
{
	_uint iDir = (_uint)m_eDir;
	++iDir;
	if (iDir == 4)
		iDir = 0;
	m_iTextureNum = iDir;
	m_eDir = (CWarpBlock::DIRECTION)iDir;
}

void CWarpBlock::MakeEffect(void)
{
	m_eShaderSelect = SHADER_WARP;
		
	for (int i = 0; i < 5; i++)
	{
		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(-.5f, .5f);
		_float3 vPos = m_vTeleportPos;
		_float3 vPos2 = vPos;
		vPos.x += distr(eng);
		vPos.z += distr(eng);
		CParticleMgr::Get_Instance()->ReuseObj(m_iNumLevel,
			vPos,
			vPos - vPos2,
			CParticleMgr::WARP);
	}
}

void CWarpBlock::MakeSound(_tchar * pTag, _uint ID, _float Volum)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayEffect(pTag, ID, Volum);

	Safe_Release(pGameInstance);
}

HRESULT CWarpBlock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iNumLevel, TEXT("Prototype_Component_Texture_WarpBlock"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, this)))
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

CWarpBlock * CWarpBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWarpBlock*		pInstance = new CWarpBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWarpBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWarpBlock::Clone(void* pArg)
{
	CWarpBlock*		pInstance = new CWarpBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CWarpBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWarpBlock::Free()
{
	__super::Free();
}

