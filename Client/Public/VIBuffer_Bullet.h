#pragma once
#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Bullet : public CVIBuffer
{
protected:
	CVIBuffer_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Bullet(const CVIBuffer_Bullet& rhs);
	virtual ~CVIBuffer_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

