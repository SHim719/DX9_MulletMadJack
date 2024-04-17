#pragma once
#include "Client_Defines.h"
#include "CUi.h"
#include "CText.h"
BEGIN(Client)

class CUi_LoadingBackGround final : public CUi
{
private:
	CUi_LoadingBackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_LoadingBackGround(const CUi_LoadingBackGround& rhs);
	virtual ~CUi_LoadingBackGround() = default;


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
	virtual void Default_Set_Size() override;

	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


private:
	_float m_fTextureSwitchingTime = { 0 };
	
//for loadingbar

public:
	void Set_BarRatio(_float Ratio);


private:
	void Render_Bar();
	void Render_Text();
	void Initialize_Bar();
	void Adjust_LoadingBar(_float fTimeDelta);


private:
	CVIBuffer_Rect* m_pBarVIBufferCom = { nullptr };
	CTransform* m_pBarTransformCom = { nullptr };
	CTexture* m_pBarTextureCom = { nullptr };
	_float3 m_Origin_BarScale = { 500, 20, 1 };
	_float3 m_Render_BarScale = { 0, 20, 1 };
	_float m_fLoadingTime = { 0 };
	_float m_fNew_Ratio = { 0.1f };
	_float m_fOld_Ratio = { 0 };
	CText::Text_Info m_PrintPercent = {};
public:
	static CUi_LoadingBackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> Ui_Pos_Size_Rotation
	virtual void Free() override;
};

END