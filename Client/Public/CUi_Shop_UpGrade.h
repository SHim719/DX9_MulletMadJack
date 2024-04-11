#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)

class CUi_Shop_UpGrade final : public CUi
{
private:
	CUi_Shop_UpGrade(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Shop_UpGrade(const CUi_Shop_UpGrade& rhs);
	virtual ~CUi_Shop_UpGrade() = default;


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


public:
	void Set_Pos(_float3 Position);
	void Set_Focusing(bool Focused) { m_bFocused = Focused; }
	void Set_Picked() { m_bPicked = true; }
	void Add_UniqueTextureIndex();
	void Set_UniqueTextureIndex(_uint _IndexNum);


private:
	class CUi_UpGrade_Select* m_pSelect = { nullptr };
	class CUi_UpGrade_Focus* m_pFocus = { nullptr };
	CTexture* m_pUniqueTexture = { nullptr };
	_uint m_iUniqueTextureIndex = { 0 };


private:
	bool m_bFocused = { false };
	bool m_bPicked = { false };


public:
	static CUi_Shop_UpGrade* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUi* Clone(void* pArg); // pArg -> _float3
	virtual void Free() override;
};

END
