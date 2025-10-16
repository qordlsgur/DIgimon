#include "Material.h"

#include "Shader.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize(const _char* pModelFilePath, MATERIAL_DATA& Data)
{
	for (size_t i = 0; i < Data.Type_Count; i++)
	{
		m_iNumSRVs = Data.Path[i].textureCount;

		m_SRVs[i].reserve(m_iNumSRVs);

		for (size_t j = 0; j < m_iNumSRVs; j++)
		{
			_tchar		szPerfectPath[MAX_PATH] = {};

			wcsncpy_s(szPerfectPath, MAX_PATH, Data.Path[i].szPerfectPath[i].c_str(), _TRUNCATE);

			ID3D11ShaderResourceView* pSRV = { nullptr };

			HRESULT			hr = {};


			hr = CreateWICTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_SRVs[i].push_back(pSRV);
		}
	}


	return S_OK;
}

HRESULT CMaterial::Bind_SRV(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName, m_SRVs[1][iTextureIndex]);
}

HRESULT CMaterial::Bind_SRVN(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName, m_SRVs[6][iTextureIndex]);
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, MATERIAL_DATA& Data)
{
	CMaterial* pInstance = new CMaterial(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pModelFilePath, Data)))
	{
		MSG_BOX("Failed to Created : CMaterial");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterial::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& SRVs : m_SRVs)
	{
		for (auto& pSRV : SRVs)
			Safe_Release(pSRV);
		SRVs.clear();
	}
}
