#include "Mesh.h"

#include "Bone.h"
#include "Model.h"
#include "Shader.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CMesh::CMesh(const CMesh& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CMesh::Initialize_Prototype(MODEL eType, const CModel* pModel, MESH& Data, _fmatrix PreTransformMatrix)
{
	strcpy_s(m_szName, Data.mName);

	m_iMaterialIndex = Data.mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = Data.mNumVertices;

	m_iNumIndices = Data.mNumIndices;

	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	if (MODEL::NONANIM == eType)
		Ready_VertexBuffer_For_NonAnim(Data, PreTransformMatrix);


#pragma endregion

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;


	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = {};

	for (size_t i = 0; i < Data.mNumFaces; i++)
	{

		pIndices[iNumIndices++] = Data.mIndex[i].mIndices[0];
		pIndices[iNumIndices++] = Data.mIndex[i].mIndices[1];
		pIndices[iNumIndices++] = Data.mIndex[i].mIndices[2];
	}


	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(MODEL eType, const CModel* pModel, ANIM_MESH& Data, _fmatrix PreTransformMatrix)
{
	strcpy_s(m_szName, Data.mName);

	m_iMaterialIndex = Data.mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = Data.mNumVertices;

	m_iNumIndices = Data.mNumIndices;

	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	if (MODEL::ANIM == eType)
		Ready_VertexBuffer_For_Anim(pModel, Data);


#pragma endregion

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;


	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = {};

	for (size_t i = 0; i < Data.mNumFaces; i++)
	{

		pIndices[iNumIndices++] = Data.mIndex[i].mIndices[0];
		pIndices[iNumIndices++] = Data.mIndex[i].mIndices[1];
		pIndices[iNumIndices++] = Data.mIndex[i].mIndices[2];
	}


	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(const vector<CBone*>& Bones, CShader* pShader, const _char* pConstantName)
{
	if (m_iNumBones >= 512)
		return E_FAIL;


	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* 최종적으로 렌더링하기위한 뼈의 행렬(CombinedTransformationMatrix). */
		XMStoreFloat4x4(&m_pBoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
	}

	if (0 == m_iNumBones)
		return S_OK;

	return pShader->Bind_Matrices(pConstantName, m_pBoneMatrices, m_iNumBones);

}

HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(MESH& Mesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &Mesh.mVertex[i].mVertices, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &Mesh.mVertex[i].mNormals, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix)));

		memcpy(&pVertices[i].vTangent, &Mesh.mVertex[i].mTangents, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix)));

		memcpy(&pVertices[i].vTexcoord, &Mesh.mVertex[i].mTextureCoords, sizeof(_float2));
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(const CModel* pModel, ANIM_MESH& Mesh)
{
	m_iVertexStride = sizeof(VTXANIMMESH);
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &Mesh.mVertex[i].mVertices, sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &Mesh.mVertex[i].mNormals, sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &Mesh.mVertex[i].mTangents, sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &Mesh.mVertex[i].mTextureCoords, sizeof(_float2));
	}

	m_iNumBones = Mesh.mNumBones;

	m_OffsetMatrices.reserve(m_iNumBones);

	m_pBoneMatrices = new _float4x4[0 == m_iNumBones ? 1 : m_iNumBones];
	ZeroMemory(m_pBoneMatrices, sizeof(_float4x4) * m_iNumBones);

	_float4x4		OffsetMatrix;
	XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		memcpy(&OffsetMatrix, &Mesh.mBone[i].mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		_int		iBoneIndex = Mesh.mBone[i].mBoneIndex;
		if (-1 == iBoneIndex)
			return E_FAIL;

		m_OffsetMatrices.push_back(OffsetMatrix);
		m_BoneIndices.push_back(iBoneIndex);

		/* 이 뼈는 몇개의 정점에게 영향을 주는가? */
		for (size_t j = 0; j < Mesh.mBone[i].mWeight.size(); j++)
		{

			if (0.f == pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.x)
			{
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendIndex.x = i;
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.x = Mesh.mBone[i].mWeight[j].x;
			}

			else if (0.f == pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.y)
			{
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendIndex.y = i;
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.y = Mesh.mBone[i].mWeight[j].y;
			}

			else if (0.f == pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.z)
			{
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendIndex.z = i;
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.z = Mesh.mBone[i].mWeight[j].z;
			}

			else
			{
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendIndex.w = i;
				pVertices[Mesh.mBone[i].mWeight[j].mVertexId].vBlendWeight.w = Mesh.mBone[i].mWeight[j].w;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		Mesh.mNumBones = m_iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));

		m_OffsetMatrices.push_back(OffsetMatrix);
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVB)))
		return E_FAIL;


	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const CModel* pModel, MESH& Data, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModel, Data, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const CModel* pModel, ANIM_MESH& Data, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModel, Data, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CMesh::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pBoneMatrices);


}
