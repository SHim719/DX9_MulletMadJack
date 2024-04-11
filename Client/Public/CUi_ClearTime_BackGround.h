#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_ClearTime_BackGround final : public CUi
{
private:
	CUi_ClearTime_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_ClearTime_BackGround(const CUi_ClearTime_BackGround& rhs);
	virtual ~CUi_ClearTime_BackGround() = default;


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
	void Default_Set_LifeTime();
	void Default_Set_Size();

	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;
	void Initialize_Set_Division();


public:
	void Set_Pos(_float3 Position);
	void Scaling(_float fTimeDelta);


private:
	vector<class CUi_Time_Division*> m_pDivision;
	_float m_fScaleTime = {0};


public:
	static CUi_ClearTime_BackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> Ui_Pos_Size_Rotation
	virtual void Free() override;
};

END
