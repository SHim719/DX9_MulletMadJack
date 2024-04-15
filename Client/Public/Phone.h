#pragma once
#include "Client_Defines.h"
#include "CUi.h"

BEGIN(Client)

class CPhone final : public CUi
{
protected:
	CPhone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPhone(const CPhone& rhs);
	virtual ~CPhone() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Active();
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg);
	virtual void Initialize_Set_Speed();

	virtual void Default_Set_LifeTime();
	virtual void Default_Set_Size();

	void Default_Set_Delay(_float fDelay) { m_fTimeDelay = fDelay; m_fTimeDelayOrigin = fDelay; }

private:
	void Set_Ui_Pos(POINT ptPos) { m_UiDesc.m_fX = (_float)ptPos.x; m_UiDesc.m_fY = (_float)ptPos.y; }
	void Set_Ui_Pos(_float fX, _float fY) { m_UiDesc.m_fX = fX; m_UiDesc.m_fY = fY; }

	void Set_Ui_Pos_Add(POINT ptPos) { m_UiDesc.m_fX += (_float)ptPos.x; m_UiDesc.m_fY += (_float)ptPos.y; }
	void Set_Ui_Pos_Add(_float fX, _float fY) { m_UiDesc.m_fX += fX; m_UiDesc.m_fY += fY; }

	void Set_Ui_Size(POINT ptSize) { m_UiDesc.m_fSizeX = (_float)ptSize.x; m_UiDesc.m_fSizeY = (_float)ptSize.y; }
	void Set_Ui_Size(_float fSizeX, _float fSizeY) { m_UiDesc.m_fSizeX = fSizeX; m_UiDesc.m_fSizeY = fSizeY; }

	_float AnimationDelay(_float fTimeDelta) { return m_fTimeDelay -= fTimeDelta; }
	void   AnimationDelayReset() { m_fTimeDelay = m_fTimeDelayOrigin; }

	void	Set_Divide(_float fDivide) { m_fDivide = fDivide; }
	_float  Get_Divide() { return m_fDivide; }

	//POINT Get_Texture_Info() { return { m_pTextureCom->Get_TextureWidth(), m_pTextureCom->Get_TextureHeight() }; }
	POINT Get_Texture_Info();

	_float2 Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed = 1);

protected:
	virtual HRESULT Add_Components(void* pArg);
	virtual HRESULT Add_Texture(void* pArg);

public:
	static CUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

private:
	_float m_fTimeDelayOrigin = 0.f;
	_float m_fTimeDelay = 0.f;
	_float m_fDivide = 1.f;

	_float3 m_fScale = { 0.f , 0.f, 1.f };
	_float3 m_fRotation = { 0.f, 0.f, 0.f };

	_float m_fLissajousTime = 0.f;


	//for Phone_BackGround
private:
	void Initialize_BackGround();
	void Default_Set_BackGroundSize();
	void Set_BackGround();
	void Set_BackGroundPos();
	void Set_BackGroundRotation();
	void Set_BackGroundTexture();
	HRESULT Render_BackGround();


private:
	CVIBuffer_Rect* m_pBackGroundVIBufferCom = { nullptr };
	CTransform* m_pBackGroundTransformCom = { nullptr };
	CTexture* m_pBackGroundTextureCom = { nullptr };
	_float m_fBackGroundChangeTime = { 0 };
	_uint m_iBackGroundTexture = { 0 };
	Ui_Pos_Size_Rotation m_BackGround = {};

//for phone number
private:
	void Intialize_PhoneNumber();
	void Default_Set_PhoneSize();
	void Set_PhoneNumber();
	void Set_PhonePos();
	void Set_PhoneRotation();
	void Synchronize_PlayerLife_Number();
	HRESULT Render_Number();

private:
	CVIBuffer_Rect* m_pFirstNumVIBufferCom = { nullptr };
	CTransform* m_pFirstNumTransformCom = { nullptr };
	CTexture* m_pFirstNumTextureCom = { nullptr };
	Ui_Pos_Size_Rotation m_FirstNumDesc = {};
	_uint m_iFirstNumTextureNum = { 0 };

private:
	CVIBuffer_Rect* m_pSecondNumVIBufferCom = { nullptr };
	CTransform* m_pSecondNumTransformCom = { nullptr };
	CTexture* m_pSecondNumTextureCom = { nullptr };
	Ui_Pos_Size_Rotation m_SecondNumDesc = {};
	_uint m_iSecondNumTextureNum = { 0 };
	_uint m_fLifeTime = { 15 };
	_float m_fTestDelta = { 0 };


// for Phone PlayerFace
private:
	void Initialize_Face();
	void Default_Set_FaceSize();
	void Set_Face(_float fTimeDelta);
	void Set_FacePos();
	void Set_FaceRotation();
	void Set_FaceTexture();
	HRESULT Render_Face();

private:
	CVIBuffer_Rect* m_pFaceVIBufferCom = { nullptr };
	CTransform* m_pFaceTransformCom = { nullptr };
	CTexture* m_pFaceTextureCom = { nullptr };
	_float m_fFaceChangeTime = { 0 };
	_uint m_iFaceTexture = { 0 };
	Ui_Pos_Size_Rotation m_Face = {};


public:
	virtual void Free() override;
};

END