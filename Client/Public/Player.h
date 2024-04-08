#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

//BEGIN(Engine)
//class CTexture;
//class CTransform;
//class CVIBuffer_Rect;
//END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	/* ������ �����ϱ����� �Լ�. */
	static CPlayer* Create();
	/* �纻�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override { return NULL; };
	virtual void Free() override;
};

END