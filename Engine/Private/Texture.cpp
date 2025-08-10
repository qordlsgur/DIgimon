#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CTexture::CTexture(const CTexture& Prototype)
	: CComponent{ Prototype }
	, m_iNumSRVs{ Prototype.m_iNumSRVs }
	, m_SRVs{ Prototype.m_SRVs }
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

// 가장 중요한 텍스쳐를 저장하는 함수이다.
// CreateDDSTextureFromFile: 이거는 dds로 만든 텍스쳐를 해주는 함수
// CreateWICTextureFromFile: 이거는 이제 dds나 tga가 아닌 PNG, JPG같은 친구들을 만들어 주는 함수이다.
HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	// 일단 Texture가 1개가 아닌 여러개 받을 수 있어서 그 개수를 저장해둔다.
	m_iNumSRVs = iNumTextures;

	// 그리고 확자명이 뭔지 저장을 하기 위해서 받아올 친구들 저장 해둔다.
	_tchar			szEXT[MAX_PATH] = {};

	// 이 함수는 api에서 파일을 다 분리 해주는 함수인데 파일 확장자만 추출을 해준다.
	// 일단 앞에서부터 위치, 드라이브 경로, 크기, 디렉토리 경로, 크기, 파일이름, 크기
	// 마지막 2개는 이제 확장자명이랑 문자열 최대 크기를 받아온다.
	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

	// 폴더 이름을 받아오기 위해서
	_tchar			szFullPath[MAX_PATH] = {};

	for (_uint i = 0; i < m_iNumSRVs; ++i)
	{
		// 원래는 로드를 할 때 
		// 
		// 
		// 로 먼저 선언을 하고 그 다음에 
		// ID3D11ShaderResourcView를 만들어야 하는데 이거를 외부 라이브러리가
		// 알아서 해줌
		// 그래서 ID3D11ShaderResourcView만 선언을 해줌
		ID3D11ShaderResourceView* pSRV = { nullptr };
		/*m_pDevice->CreateShaderResourceView();*/

		// 이거를 선언한 이유는 이제 실행결과를 저장 하기 위해서 선언을 했다.
		HRESULT			hr = { };

		// 이제 이미지의 개수를 %d로 받아 오는데 그 개수가 몇개인지 int형으로 받아온다.
		wsprintf(szFullPath, pTextureFilePath, i);

		// 이제 확장자명이 뭔지에 따라서 생성을 다르게 해준다.
		// 이거는 dds일 때 생성을 해주는 함수이다. 
		// Device를 받아오고 이름, 그리고 nullptr은 위에서 말 한 것 처럼 원래는
		// ID3DTexture2D를 생성 하고 Shader를 한다고 했는데 어차피 외부 라이브러리를
		// 사용 하면 이제 알아서 만들어 주기 때문에 그녕 nullptr로 해도 상관 없다.
		if (false == lstrcmp(szEXT, TEXT(".dds")))
			hr = CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		// 이거도 말 한것 처럼 외부 라이브러리가 다른건 다 받아 지는데 이제 tga만 안받아져서
		// tga로 들어오면 터트려 버린다.
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
			hr = E_FAIL;

		// 이제 그게 아니면 다른 모든건 WIC로 생성을 해준다.
		else
			hr = CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		// 이제 위에서 생성된게 성공인지 실패인지 확인을 해준다.
		if (FAILED(hr))
			return E_FAIL;

		// 생성에 성공 하면 이제 Vector에 집어 넣는다.
		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}


HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

// 셰이더에 이미지를 던기기 위해서 설정을 해놓음
HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);
}

// 일단 다른 친구들과 다르게 Texture는 일단 저장된 파일 위치랑 Texture의 개수를 
// 저장 해야해서 기본적인 Device랑 Context를 받아오고 위치랑 개수도 받아온다.
CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}


// 이제 종료를 하면 다 지워 줘야 해서 Vector를 비워주기 위해서 for문을 돌려서 싹 비워주고
// 마지막에 clear를 해준다.
void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();

}