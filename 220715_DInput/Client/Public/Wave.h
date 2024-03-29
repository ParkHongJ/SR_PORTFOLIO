#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CCollider;
class CBoxCollider;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)
class CWave : public CGameObject
{
private:
	CWave(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWave(const CWave& rhs);
	virtual ~CWave() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void OnTriggerExit(CGameObject* other, float fTimeDelta);
	virtual void OnTriggerEnter(CGameObject* other, float fTimeDelta);
	virtual void OnTriggerStay(CGameObject*	other, float fTimeDelta, _uint eDireciton);
private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CBoxCollider* m_pBoxCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CShader*	m_pShaderCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

public:
	static CWave* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

