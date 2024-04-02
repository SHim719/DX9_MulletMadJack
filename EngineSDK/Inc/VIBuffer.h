#pragma once

#include "Component.h"

/* �������� ��, �ε������� �Ҵ��ϴ� �۾�. */
/* ����(Vertex)�� �ε���(Index)�� ���� �����Ѵ�. */
/* ����,�ε������� Ȱ���Ͽ� ȭ�鿡 �׷��ش�. */
/* Rect, Cube, Terrain�� �θ�μ��� ��Ȱ�� �����ϳ�. */

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
	/* ���� �迭�� �����ϴ� �� ��ü. */
	LPDIRECT3DVERTEXBUFFER9			m_pVB = { nullptr };

	/* ���� �ϳ��� ũ��. */
	_uint							m_iVertexStride = { 0 };
	_uint							m_iNumVertices = { 0 };
	_ulong							m_FVF = { 0 };
	D3DPRIMITIVETYPE				m_ePrimitiveType;
	_uint							m_iNumPrimitives = { 0 };

protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iIndexSizeofPrimitive = 0;
	D3DFORMAT						m_eIndexFormat;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END