#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Interaction_Block.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CWarpBlock final : public CInteraction_Block
{
public:
	enum DIRECTION { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_END };
	
private:
	CWarpBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWarpBlock(const CWarpBlock& rhs);
	virtual ~CWarpBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = nullptr;

public:
	virtual void OnTriggerExit(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerEnter(CGameObject* other, _float fTimeDelta) {};
	virtual void OnTriggerStay(CGameObject*	other, _float fTimeDelta, _uint eDirection) override;

public:
	void SetPartner(CWarpBlock* pPartner) { 
		m_pPartner = pPartner; 
	}
	_float3 GetTeleportPos() { return m_vTeleportPos; }
	_uint Get_Dir() { return m_eDir; }
	void Rotate_WarpBlock();//탑디가 들고있을때 텍스쳐와함께 시계방향으로 돌릴거야.

private:
	_uint m_iWBNum;
	_uint m_iTextureNum =  0 ;
	_uint m_iShaderSelect = 0;
	_float m_fShaderTimer = 0.f;
	DIRECTION m_eDir = DIR_END;
	
	//텔레포트할 위치
	_float3 m_vTeleportPos;

	//포탈블럭과 m_vTeleportPos 사이의 거리
	_float m_fWarpDistance;

	//깐부
	CWarpBlock* m_pPartner = nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CWarpBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END