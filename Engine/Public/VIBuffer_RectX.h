#pragma once
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectX : public CVIBuffer_Rect
{
private:
	CVIBuffer_RectX(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RectX(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_RectX() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Scaling_Texcoord(const _float3& vScale)	override;

public:
	static CVIBuffer_RectX* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
};

END
