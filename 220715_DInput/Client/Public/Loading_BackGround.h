#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CLoading_BackGround final : public CGameObject
{
private:
	CLoading_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_BackGround(const CLoading_BackGround& rhs);
	virtual ~CLoading_BackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTexture_BackGroundCom = nullptr;
	CRenderer*				m_pRenderer_BackGroundCom = nullptr;
	CTransform*				m_pTransform_BackGroundCom = nullptr;
	CVIBuffer_Rect*			m_pVIBuffer_BackGroundCom = nullptr;

	CTexture*				m_pTexture_CloudCom = nullptr;
	CRenderer*				m_pRenderer_CloudCom = nullptr;
	CTransform*				m_pTransform_CloudCom = nullptr;
	CVIBuffer_Rect*			m_pVIBuffer_CloudCom = nullptr;

	CTexture*				m_pTexture_InsideCom = nullptr;
	CRenderer*				m_pRenderer_InsideCom = nullptr;
	CTransform*				m_pTransform_InsideCom = nullptr;
	CVIBuffer_Rect*			m_pVIBuffer_InsideCom = nullptr;

private:
	_float4x4				m_ProjMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY, m_fTimeDelta{0.f};
	_uint					m_iCloudFrame{0};

private:
	HRESULT SetUp_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CLoading_BackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END