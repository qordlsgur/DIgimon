#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CShader::CShader(const CShader& Prototype)
	: CComponent{ Prototype }
	, m_pEffect{ Prototype.m_pEffect }
	, m_iNumPasses{ Prototype.m_iNumPasses }
	, m_InputLayouts{ Prototype.m_InputLayouts }
{
	Safe_AddRef(m_pEffect);

	for (auto& pInputLayout : m_InputLayouts)
		Safe_AddRef(pInputLayout);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	// VIBuffer에서 안만들고 Shader에서 Layout을 만들어 줘야 함
	_uint iHlslFlag = {};

	// 이제 Flag를 설정 하는데 우선 디버그 모드에는 최적화를 생략 해준다.
	// 기본적으로 코드에서 만약 for(int i = 0;i<999;i++)라는 for문이 있고 안에 내용이 없으면
	// 알아서 이 친구를 실헹 안하고 해버린다. 이런걸 스스로 최적화 못하게 막아버린다.
#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		// 릴리즈 모드일때는 그냥 가장 기본적인 D3DCOMPILE_OPTIMZATION_LEVEL1를 사용함 
		// 이 친구는 설정 없이 기본적으로 해주는거다.
#else
	iHlslFlag = D3DCOMPILE_OPTIMZATION_LEVEL1;
#endif

		// 이 함수는 셰이더를 객체화 시키기 위해서 사용하는 함수이다.
		// 첫 번째 인자는 셰이더 파일의 경로 이다.
		// 두 번째 인자는 이제 define을 하는데 세번째 인자에서 같이 해줄 수 있으므로 그냥 nullptr
		// 세 번째 인자는 Include 인데 셰이더 파일에서 Include할 수 있게 해줌
		// D3D_COMPILE_STANDARE_FILE_INCLUDE를 안하면 셰이더 파일에서 Include 를 사용 못함
		// 네 번째 인자는 Flag를 설정 해준다 위에 참고
		// 다섯 번째 인자는 pass에 버전을 정의 해놔서 0으로 설정함
		// 여섯 번쨰 인자는 Device를 던져줌
		// 일곱 번쨰 인자는 m_pEffect
		// 여덟 번째는 Blob인데 지금은 사용을 거의 안해서 nullptr로 해도 상관 없음
		if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
			return E_FAIL;

	// Blob은 옛날 13 버전에는 셰이더가 디버그를 돌려도 에러메세지가 출력이 안되서
	// 에러 메세지를 Blob에 넣어서 출력을 했는데 지금은 디버그창에 떠서 필요 없음

	// 태크니컬은 1개만 정의 함.
	// 이 안에 pass가 몇개 있는지 알아야함
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);

	// 구조체를 정의 함
	D3DX11_TECHNIQUE_DESC TechniqueDesc{};

	// 위의 구조체를 받아옴
	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	// 구조체 안에 pass의 갯수도 있어서 그 갯수를 저장 해놓음
	m_iNumPasses = TechniqueDesc.Passes;

	// pass 갯수만큼 for문을 돌려야 하는데 원래 pass는 여러개 지정이 가능하다.
	// 그러면 결국 pass에 맞는 친구가 있어야 하는데 그거를 방지하기 위해서 1개의 셰이더에서
	// 1개의 일만 하기 위해서 이름으로 구분을 해둔다.
	for (size_t i = 0; i < m_iNumPasses; i++)
	{
		// Layout을 저장 해 두기 위한 변수
		ID3D11InputLayout* pInputLayout = { nullptr };

		// 셰이더 안에 어떤 VS_IN을 받는지 함수 파라미터를 받음
		// 그리고 pass를 여러개 정의해서 여러개 받을 수 있음
		// 하지만 Effect한태 바로 정보를 꺼내 오는게 말이 안됨
	  // 그래서 Effect안에 있는 pass까지 들어가서 검사를 해야함
		// pass의 번호를 꺼내와서 그 친구를 대입
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == pPass)
			return E_FAIL;

		// pass에 구조체를 넣음
		D3DX11_PASS_DESC	PassDesc{};
		if (FAILED(pPass->GetDesc(&PassDesc)))
			return E_FAIL;

		// 이제 Layout을 만들기 위해서 함수를 선언 해줌
		if (FAILED(m_pDevice->CreateInputLayout(
			// 이 친구는 받아온 Elements
			pElements,

			// 이 친구는 그 Elements의 갯수
			iNumElements,

			/*쉐이더에서 받아줄수 있는 정점의 정보*/
			PassDesc.pIAInputSignature,

			/*쉐이더에서 받아줄수 있는 정점의 멤버변수갯수*/
			PassDesc.IAInputSignatureSize,

			&pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}
	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pConstanName, const _float4x4* pMatrix)
{
	// 우선 셰이더 안에 받은 이름이 있나 없나 검사를 해줌
	// 그래서 전역 변수의 핸들을 얻어옴
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstanName);
	if (nullptr == pVariable)
		return E_FAIL;

	// 그래서 받은 친구가 행렬인지 아닌지 확인을 함
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	// 맞으면 행렬을 넘김 float으로 형 변환
	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));
}

HRESULT CShader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	// 이 함수는 텍스쳐를 넣을 때 이제 그 텍스쳐가 전역 변수로 있으면 그거를 넘겨주기 위함
	// 그래서 전역 변수의 핸들을 얻어옴
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	// 그게 진짜로 ShaderResource인지 확인 해줌
	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	// 맞으면 넘겨줌
	return pSRVariable->SetResource(pSRV);
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	// Begin은 이제 Index값을 받아서 그 값에 맞는 pass를 실행 시켜줌
	if (iPassIndex >= m_iNumPasses)
		return E_FAIL;

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	/* Apply를 반드시 호출해야만 쉐이더로 그려진다. */
	/* Apply이전에 쉐이더에 전달할 모든 데이터들을 다 던져놓아야한다. */
	// 이거 이후에 값을 던지는건 불가능하다.
	// 그래서 미리 값을 다 던져야 함.
	m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);

	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);

	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	m_InputLayouts.clear();
}
