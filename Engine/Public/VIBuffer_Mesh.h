#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Mesh : public CVIBuffer
{
protected:
	CVIBuffer_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Mesh(const CVIBuffer_Mesh& rhs);
	virtual ~CVIBuffer_Mesh() = default;

public:
	HRESULT Initialize_Prototype(const wstring& strObjFilePath);
	HRESULT Initialize(void* pArg) override;

public:
	bool Intersect_Ray(class CTransform* pTransform, const _float3& vRayWorldPos, const _float3& vRayDir, OUT _float3* pHitWorldPos, OUT _float* pDist) override;

public:
	static CVIBuffer_Mesh* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring& strObjFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
