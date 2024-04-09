#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

enum class Part
{
	F,
	L,
	First_O,
	Second_O,
	R,
	Number1,
	Number2,
	End
};
class CUi_Floor :  public CUi
{
protected:
	CUi_Floor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Floor(const CUi_Floor& rhs);
	virtual ~CUi_Floor() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Active();
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void Cal_Blink(_float fTimeDelta);
	bool Cal_Blink_Render(_float BlinkGap);
	void SelectPart(_uint Number);
	void Add_Time(_float fTimeDelta);
	void Calculate_Call();
	void Dead_Check();


private:
	list<class CUi_Floor_Part*> m_pPartlist;
	Part ePart = { Part::End };
	_float m_fCallTime = {0.f};
	_uint m_iCallCount = { 0 };


public:
	static CUi_Floor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END

