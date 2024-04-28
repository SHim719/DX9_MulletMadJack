#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CSkyBox final : public CGameObject
{
private:
	CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END