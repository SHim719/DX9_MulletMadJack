#pragma once
#include "Client_Defines.h"
#include "CUi.h"


BEGIN(Client)
enum class BossType
{
	Beholder,
	Artemis,
	Apollon,
	End
};
class CUi_BossHpBar final : public CUi
{
protected:
	CUi_BossHpBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi_BossHpBar(const CUi_BossHpBar& rhs);
	virtual ~CUi_BossHpBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	void Render_Begin();
	void Render_BeginBeholder();
	void Render_BeginArtemis();
	void Render_BeginApollon();
	virtual HRESULT Render() override;
	void Render_Beholder();
	void Render_Artemis();
	void Render_Apollon();
	void Render_End();


protected:
	virtual HRESULT Initialize_Active() override;
	virtual void Initialize_Set_ActiveTime() override;
	virtual void Initialize_Set_Size() override;
	virtual void Initialize_Set_Speed() override;
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) override;


protected:
	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Add_Texture(void* pArg) override;


private:
	void Shaking(_float fTimeDelta);
	void Set_HpTextureIndex();
	void LockHpBar();
	void Decision_Type();
	void Check_BossDead();


public:
	void Set_Beholder(class CBeholder* pBeholder);
	void Set_Artemis(class CArtemis* pArtemis);
	void Set_Apollon(class CApollo* pApollon);


private:
	BossType m_eFocusType = { BossType::End };
	CVIBuffer_Rect* m_pBackGroundVIBufferCom = {nullptr};
	CTransform* m_pBackGroundTransformCom = { nullptr };
	CVIBuffer_Rect* m_pNameVIBufferCom = { nullptr };
	CTransform* m_pNameTransformCom = { nullptr };
	CTexture* m_pBackGroundTextureCom[(_uint)BossType::End] = { nullptr };
	CTexture* m_pNameTextureCom[(_uint)BossType::End] = { nullptr };
	Ui_Pos_Size_Rotation m_BackDesc = {};
	_bool m_bLockHpBar = { false };
	_float m_fShakingTime = {0};
	_float m_fShakingGap = { 0 };
	_float3 m_OriginPos = { 0, 280, 0.f };
	_bool m_bBigShaked = { false };
	_uint m_iOldPercent[(_uint)BossType::End] = {10};
	Ui_Pos_Size_Rotation m_NameDesc = {};


private:
	class CBeholder* m_pBeholder = { nullptr };
	class CArtemis* m_pArtemis = { nullptr };
	class CApollo* m_pApollon = { nullptr };


public:
	static CUi_BossHpBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
