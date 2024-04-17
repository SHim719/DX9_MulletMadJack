#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CText_BackGround final : public CUi
{
private:
	CText_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	CText_BackGround(const CText_BackGround& rhs);
	virtual ~CText_BackGround() = default;


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
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;
	virtual void Initialize_Set_Speed() override;


public:
	void Set_Pos(_float3 Position);


public:
	static CText_BackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> Ui_Pos_Size_Rotation
	virtual void Free() override;
};

END
