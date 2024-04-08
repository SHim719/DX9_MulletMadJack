#pragma once
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectXY : public CVIBuffer_Rect
{
private:
	CVIBuffer_RectXY(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RectXY(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_RectXY() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Scaling_Texcoord(const _float3& vScale)	override;

public:
	static CVIBuffer_RectXY* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
};

END
