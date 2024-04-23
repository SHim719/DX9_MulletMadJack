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
//pArg = pos_and_size, appear_floor, speed
enum class SansBonePos
{
	Left,
	Middle,
	Right,
	End
};

using SansBoneArg = struct SansBoneInfo
{
	SansBonePos Size = { SansBonePos::End };
	_uint floor = { 0 };
	_float fSpeed = { 0 };
};

class CSans_Bone final : public CGameObject
{
private:
	CSans_Bone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSans_Bone(const CSans_Bone& rhs);
	virtual ~CSans_Bone() = default;

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


private:
	void Move(_float fTimeDelta);


private:
	_float m_fLife = { 5.f };


private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };


public:
	static CSans_Bone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override; //pArg -> SansBoneArg
	virtual void Free() override;
};

END