#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CMaterial final : public CBase
{
private:
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial() = default;

public:
	HRESULT Initialize(const _char* pModelFilePath, MATERIAL_DATA& Data);
	HRESULT Bind_SRV(class CShader* pShader, const _char* pConstantName,  _uint iTextureIndex);
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	_uint									m_iNumSRVs = {};
	vector<ID3D11ShaderResourceView*>		m_SRVs[27];

public:

	static CMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, MATERIAL_DATA& Data);
	virtual void Free() override;
};

NS_END