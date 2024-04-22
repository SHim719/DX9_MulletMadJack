#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_GreenCross final : public CUi
{
private:
	CUi_GreenCross(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_GreenCross(const CUi_GreenCross& rhs);
	virtual ~CUi_GreenCross() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	void Begin_RenderState();
	virtual HRESULT Render() override;
	void End_RenderState();


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;

	virtual void Default_Set_Size() override;
	virtual void Default_Set_LifeTime() override;

	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


private:
	void Adjust_Color(_float fTimeDelta);
	void Adjust_Alpha(_float fTimeDelta);
	void Move(_float fTimeDelta);

private:
	_uint m_iAlpha = { 255 };
	_uint m_iColor = { 255 };
	_float m_fColorTime = { 0 };
	_float m_fAlphaTime = { 0 };
	_bool m_bColorMode = { true };
	
public:
	static CUi_GreenCross* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> Ui_Pos_Size_Rotation
	virtual void Free() override;
};

END