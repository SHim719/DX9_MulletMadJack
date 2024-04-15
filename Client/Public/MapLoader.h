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
	HRESULT Load(const wstring& strFilePath, LEVEL eLevel);

private:
	HRESULT Load_Monster(HANDLE hFile, LEVEL eLevel);
	HRESULT Load_Trigger(HANDLE hFile, LEVEL eLevel);
	HRESULT Load_MapObject(HANDLE hFile, LEVEL eLevel);

public:
	virtual void Free() override;
};

END

