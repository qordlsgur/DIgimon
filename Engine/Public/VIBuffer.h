#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT Bind_Resources();
	virtual HRESULT Render();

protected:
	// DX9에서는 LPDRIECT3DVETREXBUFFER9이렇게 선언을 하고 VertexBuffer를 만들고
	// LPDURECT3DINDEXBUFFER9을 또 선언 해서 IndexBuffer를 만들었지만
	// 11로 와서는 이제 ID3D11Buffer로 Vertex랑 Index2개를 한번에 다 만들 수 있다.
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

	_float3* m_pVertexPositions = {};

	// 정점의 갯수이다. 
	_uint                    m_iNumVertices = {};
	// 정점의 크기이다.
	_uint                    m_iVertexStride = {};

	// 안덱스의 갯수이다.
	_uint                    m_iNumIndices = {};
	// 인덱스의 크기이다.
	_uint                    m_iIndexStride = {};

	// 버텍스 버퍼의 갯수이다.
	_uint                    m_iNumVertexBuffers = {};
	// 인덱스 버퍼를 만들 때 정점의 갯수가 6만5천개 정도 안넘으면 16으로 하고 넘으면 32로
	DXGI_FORMAT              m_eIndexFormat = {};
	// 이제 삼격형으로 그릴지 리스트로 그릴지 사각형으로 그릴지 정하는 거다.
	D3D11_PRIMITIVE_TOPOLOGY m_ePrimitive = {};

public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END