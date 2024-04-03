#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Core_Camera.h"
#include "Level_GamePlay.h"
#include "Level_Manager.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CFPS_Camera final : public CCoreCamera
{
private:
	CFPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFPS_Camera(const CFPS_Camera& rhs);
	virtual ~CFPS_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual void Update_Matrix() {};
	virtual void Update_Info() {};

private:
	HRESULT Add_Components();
	_float2				m_vOldMousePos = { 0.0f, 0.f };
	_float4x4			m_BillboardMatrix;

public:
	_float4x4		Get_Billboard_Matrix() { return m_BillboardMatrix; }

public:
	static CFPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END