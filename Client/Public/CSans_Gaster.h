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
enum class SansGasterFirePos
{
	Straight,
	BackWard,
	Left,
	Right,
	Down,
	Up,
	End
};
enum class GasterState
{
	Go_Down,
	Go_Up,
	Go_Left,
	Go_Right,
	Go_Straight,
	Go_BackWard,
	Laser,
	End
};

using GasterArg = struct SansGasterInfo
{
	SansGasterPos Pos = { SansGasterPos::End };
	SansGasterFirePos FirePos = { SansGasterFirePos::End };
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

	void Set_ArgStraightPos(SansGasterPos Pos);
	void Set_ArgBackWardPos(SansGasterPos Pos);
	void Set_ArgLeftPos(SansGasterPos Pos);
	void Set_ArgRightPos(SansGasterPos Pos);
	void Set_ArgDownPos(SansGasterPos Pos);
	void Set_ArgUpPos(SansGasterPos Pos);

private:
	_float m_fLife = { 4.f };
	_float m_fTextureSwitching = { 0 };
	_float3 m_OriginPos = { 0, 0, 0 };
	GasterState m_eState = { GasterState::End };
	SansGasterFirePos m_eFirePos = { SansGasterFirePos::End };

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