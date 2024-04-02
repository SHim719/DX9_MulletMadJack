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
	virtual void Initalize_Set_Background();


private:
	class CUi_Background* m_pBackGround = { nullptr };


public:
	static CUi_SpecialHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi_Life* Clone(void* pArg) override; // parg -> especialhit
	virtual void Free() override;
};

END