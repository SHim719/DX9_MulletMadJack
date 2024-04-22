#pragma once


#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Elevator_L : public CVIBuffer
{
protected:
	CVIBuffer_Elevator_L(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Elevator_L(const CVIBuffer_Elevator_L& rhs);
	virtual ~CVIBuffer_Elevator_L() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Elevator_L* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
