#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Floor_Logo_Word final : public CUi
{
protected:
	CUi_Floor_Logo_Word(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Floor_Logo_Word(const CUi_Floor_Logo_Word& rhs);
	virtual ~CUi_Floor_Logo_Word() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	void Initialize_LogoNumVec();

protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


public:
	virtual void Enter(bool _Enter) override;


private:
	void Move(_float fTimeDelta);


private:
	vector<class CUi_Floor_Logo_Num*> m_pLogoNum;
	_float3 m_OriginPos = { 450, -5, 0 };

public:
	static CUi_Floor_Logo_Word* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END