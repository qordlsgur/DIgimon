#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{pDevice, pContext}
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	: CVIBuffer{Prototype}
	, m_iNumVerticesX{ Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ{ Prototype.m_iNumVerticesZ }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	// 지형을 만들 때는 이미지를 이용 하는데 이제 회색을 받아서 높낮이를 구분 하는데
	// 회색으로 하는 이유는 회색의 RGB값이 3개 다 똑같은 값이라서 그렇다.
	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	// 파일의 파일 정보를 담는다.
	BITMAPFILEHEADER			fh{};
	// 파일의 이미지 정보를 담는다.
	BITMAPINFOHEADER			ih{};

	// 그리고 비트맵을 읽어온다
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	// 이제 잃어온 이미지의 너비와 높이를 추출해서 맵의 크기를 만든다.
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	// X와 Z의 픽셀을 곱해서 Vertex의 갯수를 구한다.
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	// 위에서 구한 값으로 배열의 크기를 정한 다음에 그 배열의 값을 다 0으로 밀어버린다.
	_uint* pPixels = new _uint[m_iNumVertices];
	ZeroMemory(pPixels, sizeof(_uint) * m_iNumVertices);

	// 이제 픽셀의 값을 읽는다. 회색부터 하얀색까지 있어서 그거로 높이를 정한다.
	ReadFile(hFile, pPixels, sizeof(_uint) * m_iNumVertices, &dwByte, nullptr);

	m_iNumVertexBuffers = 1;

	// 버텍스의 사이즈는 구조체의 사이즈로 한다.
	m_iVertexStride = sizeof(VTXNORTEX);

	// 삼각형의 갯수는 일단 사각형을 그릴 때 점이 4개 필요한데
	// 첫번째 사각형을 그리면 오른쪽 사각형은 이미 점이 2개 있어서 x랑z값을 1씩 빼준다.
	// 위에도 마찬가지 그 다음에 *2는 사각형을 그리는데 삼각형이 2개 들어가서 2를 곱한다.
	// 마지막 3은 삼각형을 그리기 위해서는 인덱스가 3개 필요해서 3도 곱해준다.
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	// 인덱스 하나당 크기
	m_iIndexStride = 4;

	// 맵을 그릴땐 정점의 갯수가 6만천개가 넘어갈 수도 있어서 32로 해둔다.
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	// 삼각형 리스트로 그린다.
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	// 버텍스 버퍼를 만들기 위한 구조체이다.
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;


	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	// Z축
	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		// X축
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			// X축으로 점을 찍는데 가로줄을 쫙 깔아둔다.
			_uint		iIndex = i * m_iNumVerticesX + j;

			// 이제 Position을 잡아야 하는데 x랑 z는 구할 수 있는데 y값을 구하기 위해서 
			// 비트맵의 픽셀을 들고 오는데 기본적으로 비트맵의 알파값은 없거나 0이다.
			// 그리고 이미지의 색상이 회색 뿐이라서 RGB중 어떤 값을 들고와도 똑같다.
			// 그래서 그 값으로 높이의 편차를 정해준다.
			// 마지막의 Texcoord를 정해준다.
			m_pVertexPositions[iIndex] = pVertices[iIndex].vPosition =
				_float3(j, (pPixels[iIndex] & 0x000000ff) / 10.f, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f),
				i / (m_iNumVerticesZ - 1.f));
		}
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVertices;
#pragma endregion

#pragma region INDEX_BUFFER
	// 인덱스 버퍼도 설정 해준다.
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = {};

	// Z축
	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		// X축
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			// 인덱스를 설정 해준다.
			_uint		iIndex = i * m_iNumVerticesX + j;

			// 사각형을 그리기 위해서 점을 4개 찍어야 하는데 일단 0번의 위치가 좌상단이다.
			// 그래서 점의 3번은 제일 처음 시작점이고 2번은 그 옆이다.
			// 이제 X가 100이라고 하면 0~99가 처음부터 끝이고 100이 3번점 바로 위에 위치한다.
			// 그래서 0번점은 X를 더해주고 1번점은 0번 점 옆에 있어서 1을 더해준다.
			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			// 법선 벡터를 구하기 위해서 값을 저장할 변수를 만들어 준다.
			// 법선은 빛의 반사를 구하기 위해서 만들어 둔다.
			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			// 우선 처음 두 번째에서 첫 번째 점을 뺀다.
			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) -
				XMLoadFloat3(&pVertices[iIndices[0]].vPosition);

			// 그 후에 세 번째 에서 두 번째 점을 뺀다.
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) -
				XMLoadFloat3(&pVertices[iIndices[1]].vPosition);

			// 그리고 그 값들로 외적을 해서 법선을구한다.
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			// 이제 계산으로 나온 법선을 더해준다.
			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			// 두번째 삼각형도 마찬가지
			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) -
				XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) -
				XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		// 법선을 Normalize를 해준다.
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;
#pragma endregion

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVB)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIB)))
		return E_FAIL;

	// 끝나고 꼭 배열을 비워준다.
	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	Safe_Delete_Array(pPixels);
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CVIBuffer_Terrain::Free()
{
	__super::Free();


}
