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

//Public Camera Event Order
public:
	void Camera_Shake_Order(_float fShakePower, _float fShakeTime) { m_fShakePower = fShakePower; m_fShakeTime = fShakeTime; }

//Private Camera Event List


private:
	void Camera_Shake(_float fTimeDelta, _float m_fShakePower, _float& m_fShakeTime);

//Custom Camera Event Variable
private:
	_float m_fShakeTime = 0.f;
	_float m_fShakePower = 0.f;

private:
	_float4x4			m_ViewMatrix;
	_float4x4			m_ProjMatrix;
	_float4x4			m_BillboardMatrix;

public:
	_float4x4		Get_Billboard_Matrix() { return m_BillboardMatrix; }
	

private:
	void Mouse_Ray();
	


protected:
	virtual HRESULT Add_Components() override;
	virtual void	Key_Input(_float fTimeDelta) override;
	virtual void    Camera_Event(_float fTimeDelta) override;
	//virtual void Update_Matrix() {};
	//virtual void Update_Info() {};

public:
	static CFPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END