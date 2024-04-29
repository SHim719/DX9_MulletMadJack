#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CNoise_Filter final : public CUi
{
protected:
	CNoise_Filter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CNoise_Filter(const CNoise_Filter& rhs);
	virtual ~CNoise_Filter() = default;

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
	virtual HRESULT Initialize_Active() override;
	virtual HRESULT Initialize_InActive() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;

protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;

private:
	_uint m_iTextureIdx = 0;
	_float m_fNowTimeAcc = 0.f;
	_float m_fFrameGap = 0.1f;

public:
	static CNoise_Filter* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
