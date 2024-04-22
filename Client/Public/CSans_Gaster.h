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
enum class SansGasterPos
{
	left,
	Middle,
	Right,
	End
};
enum class GasterState
{
	Go_Down,
	Laser,
	Go_Up,
	End
};
//pArg = À§Ä¡(¿ÞÂÊ, ¿À¸¥ÂÊ, Áß¾Ó,), Ãþ¼ö
using GasterArg = struct SansGasterInfo
{
	SansGasterPos Pos = { SansGasterPos::End };
	_uint _floor = { 0 };
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
	void Initialize_Arg(void* pArg);
	void PriorityTick(_float fTimeDelta) override;
	void Tick(_float fTimeDelta) override;
	void LateTick(_float fTimeDelta) override;
	void RenderBegin();
	HRESULT Render() override;
	void RenderEnd();


private:
	void OnTriggerEnter(CGameObject* pOther) override;


private:
	HRESULT Add_Components();
	HRESULT Add_Texture();
	void TextureSwitching(_float fTimeDelta);
	void SetStateLaser();

private:
	_float m_fLife = { 4.f };
	_float m_fTextureSwitching = { 0 };
	GasterState m_eState = { GasterState::End };


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