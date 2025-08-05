#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer& Prototype)
	: CVIBuffer{ Prototype }
{

}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	// 바인딩 할때 사용할 버텍스 버퍼의 갯수
	m_iNumVertexBuffers = 1;
	// 버텍스의 갯수(사각형은 0,1,2,3 4개가 필요함)
	m_iNumVertices = 4;
	// 버텍스 1개의 크기
	m_iVertexStride = sizeof(VTXPOSTEX);

	// 인덱스의 갯수이다.
	m_iNumIndices = 6;
	// 인덱스 바이트의 크기 이다. 65536개수가 안넘으면 2 넘으면 4로 설정 한다.
	m_iIndexStride = 2;

	// 위의 65536개가 넘는지 안넘는지를 설정 해주는 Format이다.
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	// 이거는 삼각형으로 그릴지 사각형으로 그릴지 등등을 정해주는 거다.
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	// 밑에 구조체 2개는 Device에 있는 CreateBuffer를 설정 하는건데 버텍스랑 인덱스랑
	// 둘다 똑같은 CreateBuffer를 사용해서 그거를 구분 해주기 위해서 설정을 해줘야 한다.
	// 이제 버텍스 버퍼를 만들기 위해서 설정을 해줘야하는 구조체중 1개이다.
	// 첫번째 구조체는 정점을 만들기 위한 정점들의 구조체를 던져준다.
	// 이제 버퍼의 속성/형식/용도를 정의 하는 버퍼 자체의 틀 이라고 생각하면 된다.
	D3D11_BUFFER_DESC		VBDesc{};
	// 첫번째는 정점 하나의 크기 * 정점의 갯수를 넣은 총 합 이라고 생각하면 좋다.
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;

	// 두번째는 정적으로 만들지 동적으로 만들지 등등을 정해준다.
	VBDesc.Usage = D3D11_USAGE_DEFAULT;

	// 이 메모리 공간은 어떤 형식으로 채워지는지 정해준다. 버텍스라서 Vertex로 설정함.
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 버텍스의 크기
	VBDesc.StructureByteStride = m_iVertexStride;

	// 밑에 2개는 정적으로 만들땐 필요가 없다.
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	// 버텍스 버퍼의 정점을 정해주는 구조체를 초기화 해준다.
	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	// 버텍스 버퍼의 정점들의 위치를 저장해 주기 위한 구조체이다. 마찬가지로 초기화 해줌
	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	// 첫번째는 정점의 위치 두번째는 정점의 TEXCOORD의 위치이다.
	// 뒤에 붙어있는 Position은 이제 버텍스 정점의 위치를 저장 해주는거다.
	// 그 이유는 이제 11은 정적으로 생성을 하면 Lock, Unlock을 사용 못함
	// 그래서 정점들의 위치를 받기 위해서 슬쩍 적어서 같이 저장을 해둠.
	m_pVertexPositions[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.0f);

	m_pVertexPositions[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.0f);

	m_pVertexPositions[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f);

	m_pVertexPositions[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.0f);

	// 두번쨰 구조체는 버퍼를 처음 만들 때 채워 넣을 초기 데이터이다. (내용)
	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	// 구조체를 넣어준다.
	InitialVBData.pSysMem = pVertices;

	// 이제 위에서 만든 구조체 2개를 순서대로 넣고 m_pVB에 저장을 해준다.
	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVB)))
		return E_FAIL;

	// 버퍼를 정적으로 생성하면 lock, unlock가 안됨 그래서 동적으로 해주는게 좋음
	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	// 인덱스도 버텍스와 마찬가지인데 이제 일부만 다르다.
	D3D11_BUFFER_DESC		IBDesc{};

	// 인덱스의 크기 * 갯수
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	// 정적으로 생성
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	// 인덱스라서 INDEX를 넣어줌 (버텍스는 VERTEX)
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// 인덱스의 갯수
	IBDesc.StructureByteStride = m_iIndexStride;
	// 밑의 2개는 마찬가지로 정적이라 필요없다.
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	// 인덱스를 저장하기 위한 변수
	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	// 삼각형이 2개 필요한데 1번 삼각형의 정점 위치
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	// 2번 삼각형의 정점 위치
	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	// 위에 버텍스와 마찬가지로 pIndices로 만듬
	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	// 똑같이 위의 구조체 2개를 이용해서 m_pIB에 넣어줌
	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIB)))
		return E_FAIL;

	// 그리고 꼭 해제를 해줘야 누수가 안생김
	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();


}
