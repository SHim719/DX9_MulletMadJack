#pragma once


#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Elevator_R : public CVIBuffer
{
protected:
	CVIBuffer_Elevator_R(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Elevator_R(const CVIBuffer_Elevator_R& rhs);
	virtual ~CVIBuffer_Elevator_R() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Elevator_R* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
