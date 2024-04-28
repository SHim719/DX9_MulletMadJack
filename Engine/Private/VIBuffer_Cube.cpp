#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	/*정점은 몇개야. */
	m_iNumVertices = 8;

	/* 정점의크기는 몇바이트야. */
	m_iVertexStride = sizeof(VTXCUBE);

	m_FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	m_iNumPrimitives = 12;

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXCUBE* pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	m_pVerticesPos[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexcoord = m_pVerticesPos[0];

	m_pVerticesPos[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = m_pVerticesPos[1];

	m_pVerticesPos[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexcoord = m_pVerticesPos[2];

	m_pVerticesPos[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = m_pVerticesPos[3];

	m_pVerticesPos[4] = pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexcoord = m_pVerticesPos[4];

	m_pVerticesPos[5] = pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexcoord = m_pVerticesPos[5];

	m_pVerticesPos[6] = pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexcoord = m_pVerticesPos[6];

	m_pVerticesPos[7] = pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexcoord = m_pVerticesPos[7];

	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER

	m_iIndexStride = 2;
	m_iNumIndices = 36;
	m_eIndexFormat = D3DFMT_INDEX16;

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* +x */
	pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
	pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;

	/* -x */
	pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
	pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;

	/* +y */
	pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
	pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;

	/* -y */
	pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
	pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;

	/* +z */
	pIndices[24] = 5; pIndices[25] = 4; pIndices[26] = 7;
	pIndices[27] = 5; pIndices[28] = 7; pIndices[29] = 6;

	/* -z */
	pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
	pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;

	m_pIB->Unlock();

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
