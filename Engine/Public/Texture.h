#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	// 텍스쳐를 저장 하기 위해서는 이제 텍스쳐가 있는 폴더 이름이랑 이미지의 개수가 필요함
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;

public:
	// 셰이더에 이미지를 넘기기 위해서 설정 해둠
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, _uint iTextureIndex);

private:
	// 텍스처의 개수
	_uint											m_iNumSRVs = { 0 };
	// 텍스쳐를 저장하기 위해서 vector로 해놓았다. 이미지를 출력 할 때도 어차피 순차적으로 
	// 출력을 한다.
	// 그리고 이제 안에는 ID311ShaderResourceView로 지정을 해뒀는데
	// 왜 D3D11Texture2D로 안한거냐 우선 ID3D11Texture2D는 텍스처 리소스 그 자체이다.
	// 하지만 우리는 텍스쳐의 픽셀값이나 이런걸 얻어서 그거를 버퍼에다가 셰이더를 사용해서
	// 넣어주는 작업을 해 줘야 하는데 그러면 그 작업을 하기 위해서는 View가 들어간
	// 친구들 사용 해 줘야 한다. 아까도 말 한것처럼 셰이더를 이용 할 거니까 
	// ID3D11ShderResiurceVIew로 해줘야 사용이 가능하다.
	vector<ID3D11ShaderResourceView*>				m_SRVs;

public:
public:
	// 그래서 생성을 할 때도 다른 친구들과 달리 Device랑 Context를 받고 또 파일 이름이랑
	// 개수를 받아서 그 폴더에 이미지가 몇개 있는지 개수를 넣어준다.
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END