#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUI_UltimatePicture final : public CUi
{
protected:
	CUI_UltimatePicture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_UltimatePicture(const CUI_UltimatePicture& rhs);
	virtual ~CUI_UltimatePicture() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Active();
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg);
	virtual void Initialize_Set_Speed();

	virtual void Default_Set_LifeTime();
	virtual void Default_Set_Size();

private:
	void Set_Ui_Pos(POINT ptPos) { m_UiDesc.m_fX = (_float)ptPos.x; m_UiDesc.m_fY = (_float)ptPos.y; }
	void Set_Ui_Pos(_float fX, _float fY) { m_UiDesc.m_fX = fX; m_UiDesc.m_fY = fY; }

	void Set_Ui_Pos_Add(POINT ptPos) { m_UiDesc.m_fX += (_float)ptPos.x; m_UiDesc.m_fY += (_float)ptPos.y; }
	void Set_Ui_Pos_Add(_float fX, _float fY) { m_UiDesc.m_fX += fX; m_UiDesc.m_fY += fY; }

	void Set_Ui_Size(POINT ptSize) { m_UiDesc.m_fSizeX = (_float)ptSize.x; m_UiDesc.m_fSizeY = (_float)ptSize.y; }
	void Set_Ui_Size(_float fSizeX, _float fSizeY) { m_UiDesc.m_fSizeX = fSizeX; m_UiDesc.m_fSizeY = fSizeY; }

protected:
	virtual HRESULT Add_Components(void* pArg);
	virtual HRESULT Add_Texture(void* pArg);

public:
	static CUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

private:
	_float3 m_fScale = { 0.f , 0.f, 1.f };
	_float3 m_fRotation = { 0.f, 0.f, 0.f };
	_float3 m_vPosition = { 0.f, 0.f, 0.f };
	
	size_t m_iNowIdx = 0;
	
	_float m_fTimeAcc = 0.f;
	vector<_float3> m_vecTargetPosition;
	vector<_float> m_vecTargetTime;
public:
	virtual void Free() override;
};

END