#pragma once
#include "Client_Defines.h"
#include "CUi.h"

// for DrinkSoda and headshot and finished

BEGIN(Client)

class CUi_Special3Sec : public CUi
{
private:
	CUi_Special3Sec(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Special3Sec(const CUi_Special3Sec& rhs);
	virtual ~CUi_Special3Sec() = default;


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


private:
	void Move(_float fTimeDelta);


private:
	_float m_fMoveTime = { 0 };
	class CUi_SpecialHit_Part* m_pSpecialHit_Part = { nullptr };

public:
	static CUi_Special3Sec* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg) override; // None
	virtual void Free() override;
};

END
