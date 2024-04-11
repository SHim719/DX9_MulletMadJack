#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Real_ClearTime final : public CUi
{
private:
	CUi_Real_ClearTime(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Real_ClearTime(const CUi_Real_ClearTime& rhs);
	virtual ~CUi_Real_ClearTime() = default;


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
	void Initialize_Set_ClearTime();
	void Initialize_Set_BackGround();


private:
	void Initialize_Set_Time();
	void Scaling(_float fTimeDelta);
	void Cal_ClearTime();


private:
	class CUi_ClearTime_BackGround* m_pBackGround = { nullptr };

	_float m_fScaleTime = { 0 };
	vector<_uint> m_Number;


public:
	static CUi_Real_ClearTime* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> nullptr
	virtual void Free() override;
};

END
