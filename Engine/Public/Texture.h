#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_TEXTURE2D, TYPE_CUBEMAP, TYPE_END };

private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Bind_Texture(_uint iTextureIndex);

public:
	virtual HRESULT Initialize_Prototype(TYPE eTextureType, const wstring& strTextureFilePath, _uint iNumTextures);
	HRESULT Initialize(void* pArg) override;

private:
	vector<LPDIRECT3DBASETEXTURE9>	m_Textures;
	typedef vector<LPDIRECT3DBASETEXTURE9> TEXTURES;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eTextureType, const wstring& strTextureFilePath, _uint iNumTextures = 1);
	CComponent* Clone(void* pArg) override;
	void Free() override;
};

END