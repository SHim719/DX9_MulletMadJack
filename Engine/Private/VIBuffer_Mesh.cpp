#include "VIBuffer_Mesh.h"
#include "Transform.h"
#include <fstream>


CVIBuffer_Mesh::CVIBuffer_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Mesh::CVIBuffer_Mesh(const CVIBuffer_Mesh& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_Mesh::Initialize_Prototype(const wstring& strObjFilePath)
{
	m_iNumVertices = 0;
	m_iVertexStride = sizeof(VTXNORMAL);
	m_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 0;

	vector<_float3> vertices;
	vector<_float2> texcoords;
	vector<_float3> normals;
	vector<_uint>	vertexIndices;
	vector<_uint>	uvIndices;
	vector<_uint>	normalIndices;

	ifstream in(strObjFilePath.c_str());
	if (!in) {
		return E_FAIL;
	}

	while (in) {
		string lineHeader;
		in >> lineHeader;
		if (lineHeader == "v") {
			_float3 vertex;
			in >> vertex.x >> vertex.y >> vertex.z;
			//vertex.z = vertex.z;
			vertices.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			_float2 uv;
			in >> uv.x >> uv.y;
			uv.y = 1.f - uv.y;
			texcoords.push_back(uv);
		}
		else if (lineHeader == "vn") {
			_float3 normal;
			in >> normal.x >> normal.y >> normal.z;
			//normal.z = -normal.z;
			normals.push_back(normal);
		}
		else if (lineHeader == "f") {
			char a;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			for (int i = 0; i < 3; i++)
			{
				in >> vertexIndex[i] >> a >> uvIndex[i] >> a >> normalIndex[i];
				vertexIndices.push_back(vertexIndex[i] - 1);
				uvIndices.push_back(uvIndex[i] - 1);
				normalIndices.push_back(normalIndex[i] - 1);
			}
			m_iNumPrimitives += 1;
		}
	}

	m_iNumVertices = (_uint)vertices.size();

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORMAL* pVertices = { nullptr };

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */

	m_vecPositions.resize(m_iNumVertices);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < vertices.size(); ++i)
		pVertices[i].vPosition = m_vecPositions[i] = vertices[i];

	for (_uint i = 0; i < vertexIndices.size(); ++i)
	{
		pVertices[vertexIndices[i]].vNormal = normals[normalIndices[i]];
		pVertices[vertexIndices[i]].vTexcoord = texcoords[uvIndices[i]];
	}

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	_uint iIndicesCnt = 0;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < vertexIndices.size(); i += 3)
	{
		pIndices[iIndicesCnt]._0 = vertexIndices[i];
		pIndices[iIndicesCnt]._1 = vertexIndices[i + 1];
		pIndices[iIndicesCnt]._2 = vertexIndices[i + 2];
		iIndicesCnt++;
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Mesh::Initialize(void* pArg)
{
	return S_OK;
}

bool CVIBuffer_Mesh::Intersect_Ray(CTransform* pTransform, const _float3& vRayWorldPos, const _float3& vRayDir, OUT _float3* pHitWorldPos, OUT _float* pDist)
{
	_float4x4 Inverse_WorldMatrix = pTransform->Get_WorldMatrix_Inverse();
	_float3 vRayLocalPos = *D3DXVec3TransformCoord(&_float3(), &vRayWorldPos, &Inverse_WorldMatrix);
	_float3 vRayLocalDir = *D3DXVec3TransformNormal(&_float3(), &vRayDir, &Inverse_WorldMatrix);

	for (size_t i = 0; i < m_vecPositions.size(); i += 3)
	{
		_float fU, fV;
		if (D3DXIntersectTri(&m_vecPositions[0], &m_vecPositions[1], &m_vecPositions[2], &vRayLocalPos, &vRayLocalDir, &fU, &fV, pDist))
		{
			*pHitWorldPos = m_vecPositions[0] + fU * (m_vecPositions[1] - m_vecPositions[0]) + fV * (m_vecPositions[2] - m_vecPositions[0]);
			D3DXVec3TransformCoord(pHitWorldPos, pHitWorldPos, &pTransform->Get_WorldMatrix());
			return true;
		}

		if (D3DXIntersectTri(&m_vecPositions[0], &m_vecPositions[2], &m_vecPositions[3], &vRayLocalPos, &vRayLocalDir, &fU, &fV, pDist))
		{
			*pHitWorldPos = m_vecPositions[0] + fU * (m_vecPositions[2] - m_vecPositions[0]) + fV * (m_vecPositions[3] - m_vecPositions[0]);
			D3DXVec3TransformCoord(pHitWorldPos, pHitWorldPos, &pTransform->Get_WorldMatrix());
			return true;
		}
	}
	
	return false;
}

CVIBuffer_Mesh* CVIBuffer_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring& strObjFilePath)
{
	CVIBuffer_Mesh* pInstance = new CVIBuffer_Mesh(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(strObjFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Mesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Mesh::Clone(void* pArg)
{
	CVIBuffer_Mesh* pInstance = new CVIBuffer_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Mesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Mesh::Free()
{
	__super::Free();

}
