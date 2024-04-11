#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer;
class CTexture;   
END

BEGIN(Client)

class CSodaMachine_Banner : public CGameObject
{
private:
	CSodaMachine_Banner(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSodaMachine_Banner(const CSodaMachine_Banner& rhs);
	virtual ~CSodaMachine_Banner() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	HRESULT Add_Components();

private:
	CVIBuffer* m_pVIBuffer = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

public:
	static CSodaMachine_Banner* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END