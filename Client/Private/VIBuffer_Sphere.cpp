#include "VIBuffer_Sphere.h"
#include "Transform.h"

CVIBuffer_Sphere::CVIBuffer_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Sphere::CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_Sphere::Initialize_Prototype()
{
	_uint iNumStacks = 20;
	_uint iNumSlices = 20;

	m_iNumVertices = iNumStacks * iNumSlices;
	m_iVertexStride = sizeof(VTXNORMAL);
	m_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1; //| D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 0;

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORMAL* pVertices = { nullptr };

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */

	//m_vecPositions.resize();

	

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_float fDTheta = -2.f * D3DX_PI / 20.f;
	for (_uint i = 0; i < iNumStacks; ++i)
	{
		_float3 fStackStartPos = _float3(0.f, -0.5f, 0.f);
		D3DXVec3TransformCoord(&fStackStartPos, &fStackStartPos, D3DXMatrixRotationZ(&_float4x4(), fDTheta * (float)i));
		for (_uint j = 0; j < iNumSlices; ++j)
		{
			_float3 fPos;
			D3DXVec3TransformCoord(&fPos, &fStackStartPos, D3DXMatrixRotationY(&_float4x4(), fDTheta * (float)j));
			pVertices[i * iNumSlices + j].vPosition = fPos;
			pVertices[i * iNumSlices + j].vNormal = fPos;
			D3DXVec3Normalize(&pVertices[i * iNumSlices + j].vNormal, &pVertices[i * iNumSlices + j].vNormal);
			pVertices[i * iNumSlices + j].vTexcoord = { float(j) / iNumSlices, 1.0f - float(i) / iNumStacks };
		}
	}
	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < iNumStacks; i++) {

		const int offset = (iNumSlices + 1) * i;

		for (_uint j = 0; j < iNumSlices; j++) {

			pIndices[m_iNumPrimitives]._0 = (offset + j);
			pIndices[m_iNumPrimitives]._1 = (offset + j + iNumSlices + 1);
			pIndices[m_iNumPrimitives]._2 = (offset + j + 1 + iNumSlices + 1);
			m_iNumPrimitives++;

			pIndices[m_iNumPrimitives]._0 = (offset + j);
			pIndices[m_iNumPrimitives]._1 = (offset + j + 1 + iNumSlices + 1);
			pIndices[m_iNumPrimitives]._2 = (offset + j + 1);
			m_iNumPrimitives++;
		}
	}
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Sphere::Initialize(void* pArg)
{

	return S_OK;
}


CVIBuffer_Sphere* CVIBuffer_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Sphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_Sphere::Clone(void* pArg)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Sphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Sphere::Free()
{
	__super::Free();


}
