#pragma once
#include "Client_Defines.h"
#include "Pawn.h"
#include "Animation.h"

BEGIN(Client)

enum class SANSSTATE
{
	STATE_IDLE,
	STATE_PATTERN1,
	STATE_PATTERN2,
	STATE_PATTERN3,
	STATE_PATTERN4,
	STATE_DEATH,
	STATE_END
};
enum class SANSPatternSTATE
{
	READY,
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	End
};

class CSans : public CPawn
{
private:

	enum class PatternTimeGAP
	{
		SMALL,
		DEFAULT,
		BIG,
	};
private:
	CSans(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSans(const CSans& rhs);
	virtual ~CSans() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();


private:
	void SubPatternTime(_float fTimeDelta);
	void Set_PatternState(SANSPatternSTATE State);


private:
	void Add_SansState();
	void State_Pattern();
	

private:
	void Initialize_PatternTime();
	void Set_PatternTimeGap(PatternTimeGAP Gap) { m_ePatternTimeGap = Gap; }


private:
	SANSSTATE m_eState = { SANSSTATE::STATE_END };
	SANSPatternSTATE m_ePatternState = { SANSPatternSTATE::End };
	PatternTimeGAP m_ePatternTimeGap = { PatternTimeGAP::DEFAULT };
	_float m_fPatternTime = { 0 };
	_float m_fTest = { 0 };


private:
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CRigidbody* m_pRigidbody = { nullptr };
	class CBone_Spawner* m_pBoneSpawner = { nullptr };


public:
	static CSans* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END