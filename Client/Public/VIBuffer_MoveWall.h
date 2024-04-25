#pragma once


#include "Client_Defines.h"
#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_MoveWall : public CVIBuffer
{
protected:
	CVIBuffer_MoveWall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_MoveWall(const CVIBuffer_MoveWall& rhs);
	virtual ~CVIBuffer_MoveWall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Tick(_float fTimeDelta);

private:
	_float m_fMoveSpeed = 2.5f;
	_float2 m_vTexcoords[4];

public:
	static CVIBuffer_MoveWall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
