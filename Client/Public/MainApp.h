#pragma once


#include "Client_Defines.h"
#include "Base.h"

/* 1.게임의 전체적인 흐름을 관리한다. */
/* 2.DirectX를 통해 기초적인 렌더링을 하기위한 준비과정(디바이스를 초기화)을 수행한다.*/

//class C클래스이름
//{
//private: or protected:
//	생성자;
//	소멸자;
//
//public:
//	변수;
//public: 
//	함수;
//protected:
//	변수;
//protected:
//	함수;
//private:
//	변수;
//private:
//	함수;
//
//public:
//	생성(Create)관련한 함수();
//	소멸(Free)관련된 함수();
//};

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() { }

public:
	HRESULT Initialize();
	
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*				m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };

private:
	HRESULT Open_Level(LEVEL eStartLevelID);
	HRESULT Ready_Prototype_GameObjects();
	HRESULT Ready_Prototype_Components();

	//Custom Prototype
	HRESULT Ready_Prototype_Ui_Life();

	HRESULT Ready_Static_Texture_Prototype();

	HRESULT Ready_Active_Ui_Texture();
	HRESULT Ready_Active_Ui();
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END

