#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)
enum class RGBState
{
	Red,
	Yellow,
	Green,
	Blue,
	Purple,
	End
};
class CUi_DrinkSoda_Word final : public CUi
{
protected:
	CUi_DrinkSoda_Word(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_DrinkSoda_Word(const CUi_DrinkSoda_Word& rhs);
	virtual ~CUi_DrinkSoda_Word() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	void Render_Begin();
	virtual HRESULT Render() override;
	void Render_End();

protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void Scaling(_float fTimeDelta);
	void ChangeColor();


private:
	_float m_fScalingTime = { 0 };
	_float3 m_OriginScale = { 720, 90, 1 };
	RGBState m_eRGBstate = { RGBState::End };
	_uint m_Red = { 0 };
	_uint m_Green = { 0 };
	_uint m_Blue = { 0 };

private:
	CTexture* m_pBackGroundTextureCom = { nullptr };
	CTransform* m_pBackGroundTransformCom = { nullptr };
	CVIBuffer_Rect* m_pBackGroundVIBufferCom = { nullptr };
	Ui_Pos_Size_Rotation m_BackGround = {};


public:
	static CUi_DrinkSoda_Word* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
