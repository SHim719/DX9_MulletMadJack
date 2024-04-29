#pragma once
#include "Client_Defines.h"
#include "CUi.h"
#include "CUi_DrinkSoda_Word.h"

BEGIN(Client)

class CUi_Ultimate_Finish final : public CUi
{
protected:
	CUi_Ultimate_Finish(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Ultimate_Finish(const CUi_Ultimate_Finish& rhs);
	virtual ~CUi_Ultimate_Finish() = default;

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


private:
	CTexture* m_pBackGroundTextureCom = { nullptr };
	CTransform* m_pBackGroundTransformCom = { nullptr };
	CVIBuffer_Rect* m_pBackGroundVIBufferCom = { nullptr };
	Ui_Pos_Size_Rotation m_BackGround = {};
	RGBState m_eRGBstate = { RGBState::End };
	_uint m_Red = { 0 };
	_uint m_Green = { 0 };
	_uint m_Blue = { 0 };

public:
	static CUi_Ultimate_Finish* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
