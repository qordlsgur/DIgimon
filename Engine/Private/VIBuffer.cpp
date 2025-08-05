#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent{ Prototype }
	, m_pVB{ Prototype.m_pVB }
	, m_pIB{ Prototype.m_pIB }
	, m_pVertexPositions{ Prototype.m_pVertexPositions }
	, m_iNumVertices{ Prototype.m_iNumVertices }
	, m_iVertexStride{ Prototype.m_iVertexStride }
	, m_iNumIndices{ Prototype.m_iNumIndices }
	, m_iIndexStride{ Prototype.m_iIndexStride }
	, m_iNumVertexBuffers{ Prototype.m_iNumVertexBuffers }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_ePrimitive{ Prototype.m_ePrimitive }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Bind_Resources()
{
	// 그림을 그리기 전에 장치에 바인딩 해주는 작업이다.
	// 9와 마찬가지로 11도 무조건 세팅을 해 줘야 된다.
	// 이 함수는 이제 정점을 다 찍었으면 셰이더랑 연결을 해줘야 하고 세팅까지 다 해줘야
	// 그림이 그려져서 그거를 설정 해주는 함수이다.
	// 이 함수는 이제 버텍스 버퍼가 만약 여러개라고 하면 원래는 배열이 아니라
	// 밑에 함수에서 직접 하나하나 다 추가를 해준다고 하면 11 와서는 이제 
	// 배열안에 저장을 해서 한번에 가능해짐
	ID3D11Buffer* VertexBuffers[] = {
		m_pVB,
	};

	// 
	_uint		VertexStrides[] = {
		m_iVertexStride,
	};

	_uint		Offsets[] = {
		0,
	};

	// 밑에 함수 3개는 이제 만드는게 아니라 기능을 사용 하므로 Device가 아니라 Context를 사용
	// 스타스 슬롯(0번), 버텍스 버퍼의 갯수, 배열의 주소, 배열안에 있는 친구들 1개당 크기
																																	// Offset(0)
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, VertexBuffers,
		VertexStrides, Offsets);
	// 인덱스 버퍼는 1개만 됨, 정점의 개수가 65536개 넘어가면 32 아니면 16으로 해준다.
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	// 뭐로 그릴지 삼각형등등
	m_pContext->IASetPrimitiveTopology(m_ePrimitive);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	// 기능을 사용 하므로 Device가 아닌 Context를 이용해서 그린다.
	// 이제 그림을 그릴 때 Index의 개수, 0번부터, 0번 가장 앞에서
	// 원래는 위에 있는 Primitive가 합쳐져 있었지만 11에서는 따로 떨어짐
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

CComponent* CVIBuffer::Clone(void* pArg)
{
	return nullptr;
}

void CVIBuffer::Free()
{
	__super::Free();

	// 이제 정점을 찍을 때 그 정점들의 위치를 저장 하기 위해서 선언을 하고
	// 얕은 복사를 해서 자울 때 원본까지 다 지워지면 터질 가능성이 있어서 그거를 방지
	// 하기 위해서 원본이 지워질때만 지워지게 해놨다.
	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertexPositions);

	// 버텍스 버퍼를 초기화 해줌
	Safe_Release(m_pVB);
	// 인덱스 버퍼를 초기화 해줌
	Safe_Release(m_pIB);
}
