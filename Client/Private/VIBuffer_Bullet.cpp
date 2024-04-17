#include "VIBuffer_Bullet.h"
#include "Transform.h"

CVIBuffer_Bullet::CVIBuffer_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Bullet::CVIBuffer_Bullet(const CVIBuffer_Bullet& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_Bullet::Initialize_Prototype()
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

	_float4x4 ScaleMatrix;
	D3DXMatrixScaling(&ScaleMatrix, 0.05f, 0.05f, 1.f);

	_float fDTheta = -2.f * D3DX_PI / 20.f;
	for (_uint i = 0; i < iNumStacks; ++i)
	{
		_float3 fStackStartPos = _float3(0.f, -0.5f, 0.f);
		D3DXVec3TransformCoord(&fStackStartPos, &fStackStartPos, D3DXMatrixRotationZ(&_float4x4(), fDTheta * (float)i));
		for (_uint j = 0; j < iNumSlices; ++j)
		{
			_float3 fPos;
			D3DXVec3TransformCoord(&fPos, &fStackStartPos, D3DXMatrixRotationY(&_float4x4(), fDTheta * (float)j));
			D3DXVec3TransformCoord(&fPos, &fPos, &ScaleMatrix);
			pVertices[i * iNumSlices + j].vPosition = fPos;
			pVertices[i * iNumSlices + j].vNormal = fPos;
			D3DXVec3Normalize(&pVertices[i * iNumSlices + j].vNormal, &pVertices[i * iNumSlices + j].vNormal);
			pVertices[i * iNumSlices + j].vTexcoord = { float(j) / iNumSlices, 1.0f - float(i) / iNumStacks };
		}
	}
	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitives = 2 * iNumStacks * iNumSlices;

	int iIndex = 0;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < iNumStacks; i++) {

		const int offset = (iNumSlices + 1) * i;

		for (_uint j = 0; j < iNumSlices; j++) {

			pIndices[iIndex]._0 = (offset + j);
			pIndices[iIndex]._1 = (offset + j + iNumSlices + 1);
			pIndices[iIndex]._2 = (offset + j + 1 + iNumSlices + 1);

			iIndex += 1;

			pIndices[iIndex]._0 = (offset + j);
			pIndices[iIndex]._1 = (offset + j + 1 + iNumSlices + 1);
			pIndices[iIndex]._2 = (offset + j + 1);

			iIndex += 1;
		}
	}
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Bullet::Initialize(void* pArg)
{

	return S_OK;
}


CVIBuffer_Bullet* CVIBuffer_Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Bullet* pInstance = new CVIBuffer_Bullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Bullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_Bullet::Clone(void* pArg)
{
	CVIBuffer_Bullet* pInstance = new CVIBuffer_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Bullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Bullet::Free()
{
	__super::Free();


}
