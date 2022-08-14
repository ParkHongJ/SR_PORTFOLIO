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
	void Rotate_WarpBlock();//ž�� ��������� �ؽ��Ŀ��Բ� �ð�������� �����ž�.

private:
	_uint m_iWBNum;
	_uint m_iTextureNum =  0 ;
	_uint m_iShaderSelect = 0;
	_float m_fShaderTimer = 0.f;
	DIRECTION m_eDir = DIR_END;
	
	//�ڷ���Ʈ�� ��ġ
	_float3 m_vTeleportPos;

	//��Ż���� m_vTeleportPos ������ �Ÿ�
	_float m_fWarpDistance;

	//���
	CWarpBlock* m_pPartner = nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CWarpBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END