#pragma once
#include "Client_Defines.h"
#include "CUi_Life.h"

BEGIN(Client)

enum class eSpecialHit
{
	HEADSHOT,
	FINISHED,
	End
};
class CUi_SpecialHit : public CUi_Life
{
public:
	typedef struct SpecialHit
	{
		eSpecialHit Hit = eSpecialHit::HEADSHOT;
		_uint iCount = { 0 };
	}SpecialHit_Desc;
private:
	CUi_SpecialHit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_SpecialHit(const CUi_SpecialHit& rhs);
	virtual ~CUi_SpecialHit() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


protected:
	virtual void Default_Set_LifeTime() override;
	virtual void Default_Set_Size() override;


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


private:
	void Initialize_Set_Background();
	void Initialize_Set_Scale_Limit();
	void Initialize_Set_SpecialHit_Part();


private:
	void Move(_float fTimeDelta);
	void Scaling(_float fTimeDelta);


private:
	class CUi_Background* m_pBackGround = { nullptr };
	class CUi_SpecialHit_Part* m_pSpecialHit_Part = { nullptr };
	_float m_fMoveTime = { 0.f };
	_float m_fScaleTime = { 0.f };
	_float m_fScaleUpperLimit = { 0.f };
	_float m_fScaleDownLimit = { 0.f };


public:
	static CUi_SpecialHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg) override; // pArg -> SpecialHit_Desc
	virtual void Free() override;
};

END