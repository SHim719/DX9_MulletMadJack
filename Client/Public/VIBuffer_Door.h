#pragma once

#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Door : public CVIBuffer
{
protected:
	CVIBuffer_Door(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Door(const CVIBuffer_Door& rhs);
	virtual ~CVIBuffer_Door() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Door* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END