#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
//pArg = À§Ä¡(¿ÞÂÊ, ¿À¸¥ÂÊ, Áß¾Ó,), Ãþ¼ö
using SansGasterArg = struct SansGasterInfo
{
	SansGasterPos Pos = { SansGasterPos::End };
	_uint _floor = { 0 };
};
enum class SansGasterPos
{
	left,
	Middle,
	Right,
	End
};
class CSans_Gaster final : public CGameObject
{
private:
	CSans_Gaster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSans_Gaster(const CSans_Gaster& rhs);
	virtual ~CSans_Gaster() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	void PriorityTick(_float fTimeDelta) override;
	void Tick(_float fTimeDelta) override;
	void LateTick(_float fTimeDelta) override;
	HRESULT Render() override;


private:
	void OnTriggerEnter(CGameObject* pOther) override;


private:
	HRESULT Add_Components();


private:
	_float m_fLife = { 4.f };

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };


public:
	static CSans_Gaster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END