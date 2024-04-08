#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Box : public CVIBuffer
{
private:
	CVIBuffer_Box(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Box(const CVIBuffer_Box& rhs);
	virtual ~CVIBuffer_Box() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Box* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;
};
END
