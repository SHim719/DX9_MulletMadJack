#pragma once

#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_BoxObj : public CVIBuffer
{
private:
	CVIBuffer_BoxObj(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_BoxObj(const CVIBuffer_BoxObj& rhs);
	virtual ~CVIBuffer_BoxObj() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_BoxObj* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;
};
END
