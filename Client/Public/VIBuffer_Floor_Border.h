#pragma once

#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Floor_Border : public CVIBuffer
{
protected:
	CVIBuffer_Floor_Border(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Floor_Border(const CVIBuffer_Floor_Border& rhs);
	virtual ~CVIBuffer_Floor_Border() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Floor_Border* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END