#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_Stage2 final : public CLevel
{
private:
	CLevel_Stage2(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Stage2() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

};

END