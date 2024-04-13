#pragma once
#include "Client_Defines.h"

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMapLoader : public CBase
{
	DECLARE_SINGLETON(CMapLoader)
private:
	CMapLoader();
	virtual ~CMapLoader() = default;

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	HRESULT Load_MapObject(const wstring& strFilePath, LEVEL eLevel);

public:
	virtual void Free() override;
};

END

