#pragma once

#include "Component.h"

/* 여러개의 점, 인덱스들을 할당하는 작업. */
/* 정점(Vertex)과 인덱스(Index)를 만들어서 저장한다. */
/* 정점,인덱스들을 활용하여 화면에 그려준다. */
/* Rect, Cube, Terrain의 부모로서의 역활을 수행하낟. */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	/* 정점 배열을 보관하는 컴 객체. */
	vector<_float3>					m_vecPositions;
	LPDIRECT3DVERTEXBUFFER9			m_pVB = { nullptr };

	/* 정점 하나의 크기. */
	_uint							m_iVertexStride = { 0 };
	_uint							m_iNumVertices = { 0 };
	_ulong							m_FVF = { 0 };
	D3DPRIMITIVETYPE				m_ePrimitiveType;
	_uint							m_iNumPrimitives = { 0 };

protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iIndexSizeofPrimitive = 0;
	D3DFORMAT						m_eIndexFormat ;

	_uint							m_iIndexStride = { 0 };
	_uint							m_iNumIndices = { 0 };

protected:
	_float3*						m_pVerticesPos = { nullptr };

public:
	const vector<_float3>& Get_Positions() const { return m_vecPositions; }

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	// Ray의 World 시작좌표, World 방향이 Input, 리턴 값으로 피킹된 위치의 월드 좌표와 Ray의 시작점으로부터 거리 값을 리턴
	virtual bool Intersect_Ray(class CTransform* pTransform, const _float3& vRayWorldPos, const _float3& vRayDir, OUT _float3* pHitWorldPos, OUT _float* pDist);

public:
	virtual void Scaling_Texcoord(const _float3& vScale) {}



public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END