#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "CSans_Gaster.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Box;
END

BEGIN(Client)
enum class GasterLaserState
{
	Warning,
	Fire,
	End
};
using GasterLaserArg = struct GasterLaserInfo
{
	SansGasterFirePos FirePos;
	_float3 Pos = { 0, 0, 0 };
};
class CGasterLaser final : public CGameObject
{
private:
	CGasterLaser(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGasterLaser(const CGasterLaser& rhs);
	virtual ~CGasterLaser() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	void PriorityTick(_float fTimeDelta) override;
	void Tick(_float fTimeDelta) override;
	void LateTick(_float fTimeDelta) override;
	void BeginRenderState();
	HRESULT Render() override;
	void EndRenderState();


private:
	void OnTriggerEnter(CGameObject* pOther) override;


private:
	HRESULT Add_Components();
	HRESULT Add_Texture();
	void Initialize_Arg(void* pArg);
	void Arg_InitializeSetPosScale(SansGasterFirePos FirePos, _float3 Pos);


private:
	void AdjustAlpha(_float fTimeDelta);
	void AdjustScale(_float fTimeDelta);


private:
	GasterLaserState m_eState = { GasterLaserState::End };
	_float m_fLife = { 1.5f };
	_float m_fAlphaTime = { 0.f };
	_float m_fScaleTime = { 0.f };
	_float3 m_fScale = { 1, 1, 1 };
	_uint m_iAlpha = { 50 };


private:
	CVIBuffer_Box* m_pVIBufferCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };


public:
	static CGasterLaser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override; // pArg->_float3(Pos)
	virtual void Free() override;
};

END