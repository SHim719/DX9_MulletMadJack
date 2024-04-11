#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_RectDeep;
class CTexture;
END


BEGIN(Client)
class CBorder : public CGameObject
{
private:
	CBorder(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBorder(const CBorder& rhs);
	virtual ~CBorder() = default;

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
	static CBorder* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END
