#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CFadeObject final : public CGameObject
{
public:
	typedef struct tagBackGroundDesc
	{
		_uint		iSizeX;
		_uint		iSizeY;
	}BACKDESC;
private:
	CFadeObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFadeObject(const CFadeObject& rhs);
	virtual ~CFadeObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetOffset(_float3 _fOffset);

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float4x4				m_ProjMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	//투영 스페이스상의 반지름임.
	_float fRadian = 0.5f;
	_uint m_iPass = 3;

	//투영스페이스 상의 좌표
	_float2 m_fOffset;
private:
	HRESULT SetUp_Components();

public:
	static CFadeObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END