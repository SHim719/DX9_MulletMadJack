#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Floor_Logo_Num final : public CUi
{
private:
	CUi_Floor_Logo_Num(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Floor_Logo_Num(const CUi_Floor_Logo_Num& rhs);
	virtual ~CUi_Floor_Logo_Num() = default;


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
	virtual void Default_Set_Size();


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


public:
	void Set_Pos(_float3 Position);
	void Set_LevelTexture();


public:
	static CUi_Floor_Logo_Num* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> Ui_Pos_Size_Rotation
	virtual void Free() override;
};

END

