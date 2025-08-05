#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	// 셰이더에 있는 전역 변수들에게 값을 넘기기 위해 
	// g_WorldMatrix, g_ViewMatrix, g_ProjMatrix등등 을 이용하기 위해서 만듬
	HRESULT Bind_Matrix(const _char* pConstanName, const _float4x4* pMatrix);

	// 위에 있는 함수와 마찬가지로 g_Texture에 값을 집어 넣기 위해서 만든 함수
	HRESULT Bind_SRV(const _char* pConstanName, ID3D11ShaderResourceView* pSRV);

public:
	// 셰이더 적용을 위해
	HRESULT Begin(_uint iPassIndex);

private:
	ID3DX11Effect* m_pEffect = { nullptr };

	// 셰이더에 있는 Pass의 갯수를 저장 해 두기 위해서 지정한 함수
	_uint                       m_iNumPasses = {};

	// 객체생성이 완료 된 Layout을 저장 해 두기 위해 만들어둔 vector
	vector<ID3D11InputLayout*>   m_InputLayouts;


public:
	// Create를 보면 이제 Device랑 Context말고 셰이더 파일을 받아오고 
	// D3D11_INPUT_ELEMENT_DESC랑 iNumElemenst를 넘겨줘서 원본은 생성 하는 동시에
	// 셰이더를 객체화 시켜준다.
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END