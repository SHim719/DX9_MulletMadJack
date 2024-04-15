#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "CText.h"


BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
enum class TextType
{
	Tutorial,
	Shop,
	End
};
class CTextManager : public CBase
{
private:
	CTextManager();
	virtual ~CTextManager() = default;


public:
	void Initialize();
	void Initialize_ShopText();
	void Initialize_TutorialText();


public:
	void Print_Text(TextType type, _uint Number);


private:
	vector<CText::Text_Info> m_TutorialText;
	vector<CText::Text_Info> m_ShopText;
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CTextManager* Create();
	virtual void Free() override;
};

END