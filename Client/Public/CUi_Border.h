#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CUi_Border final : public CUi
{
public:
	using UniqueDesc = struct UiUniqueDesc
	{
		_float m_fX = { 0 };
		_float m_fY = { 0 };
		_float m_fSizeX = { 0 };
		_float m_fSizeY = { 0 };
		_float3 m_Rotation = { 0, 0, 0 };
	};
protected:
	CUi_Border(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_Border(const CUi_Border& rhs);
	virtual ~CUi_Border() = default;

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


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


public:
	virtual void Enter(bool _Enter) override;


private:
	void Move(_float fTimeDelta);
	void Scaling(_float fTimeDelta);
	void Scaling_Move(_float fTimeDelta);


private:
	CVIBuffer_Rect* m_pUniqueVIBufferCom = { nullptr };
	CTransform* m_pUniqueTransformCom = { nullptr };
	UniqueDesc m_UniqueUiDesc = {};

public:
	static CUi_Border* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
