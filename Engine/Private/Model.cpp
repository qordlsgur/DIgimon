#include "Model.h"

#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Material.h"
#include "Animation.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
	: CComponent{ Prototype }
	, m_eType{ Prototype.m_eType }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
	, m_iNumAnimations{ Prototype.m_iNumAnimations }
{
	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.push_back(pPrototypeBone->Clone());

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

	for (auto& pPrototypeAnim : Prototype.m_Animations)
		m_Animations.push_back(pPrototypeAnim->Clone());

}

_int CModel::Get_BoneIndex(const _char* pBoneName) const
{
	_int	iBoneIndex = {};

	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (true == pBone->Compare_Name(pBoneName))
				return true;

			++iBoneIndex;

			return false;
		});

	if (iter == m_Bones.end())
		return -1;

	return iBoneIndex;
}

void CModel::Set_AnimationIndex(_int iAnimIndex)
{
	if (-1 < m_iCurrentAnimIndex && m_iCurrentAnimIndex < m_iNumAnimations)
		m_iPreviousAnimIndex = m_iCurrentAnimIndex;

	m_iCurrentAnimIndex = iAnimIndex;
	m_Change_Anim = true;
	m_bAnimEnd = true;

}

HRESULT CModel::Initialize_Prototype(MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	_tchar		szPerfectPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, pModelFilePath, strlen(pModelFilePath),
		szPerfectPath, MAX_PATH);

	if (MODEL::NONANIM == eType)
	{
		Load_Model(szPerfectPath, m_Data);

		m_eType = eType;
		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

		if (FAILED(Ready_BinMeshes()))
			return E_FAIL;

		if (FAILED(Ready_Materials(pModelFilePath, m_Data)))
			return E_FAIL;
	}

	else
	{
		Load_AnimModel(szPerfectPath, m_AnimData);

		m_eType = eType;
		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

		Ready_AnimBones(m_AnimData);

		if (FAILED(Ready_BinAinmMeshes()))
			return E_FAIL;

		if (FAILED(Ready_Materials(pModelFilePath, m_AnimData)))
			return E_FAIL;

		if (FAILED(Ready_Animations(m_AnimData)))
			return E_FAIL;

		m_iCurrentAnimIndex = 0.f;
	}
	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Bind_BoneMatrices(_uint iMeshIndex, CShader* pShader, const _char* pConstantName)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(m_Bones, pShader, pConstantName);

}

HRESULT CModel::Bind_Material(_uint iMeshIndex, CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex]->Bind_SRV(pShader, pConstantName, iTextureIndex);

}

void CModel::Play_Animation(_float fTimeDelta)
{
	if (-1 == m_iCurrentAnimIndex ||
		m_iCurrentAnimIndex >= m_iNumAnimations)
		return;

	if (m_bAnimEnd == true)
	{
		if (m_Change_Anim == true)
		{
			m_Animations[m_iPreviousAnimIndex]->Save_TransformationMatrices(m_Bones, fTimeDelta, m_Lerp);
			m_Animations[m_iPreviousAnimIndex]->Reset();
			m_Animations[m_iCurrentAnimIndex]->CompareStringVectors(m_Lerp);
			m_Change_Anim = false;
		}
		/* 모든 뼈를 순회하면서 CombinedTransformationMatrix를 갱신한다. */

		m_Animations[m_iCurrentAnimIndex]->Change_TransformationMatrices(this, m_Bones, fTimeDelta, m_Lerp);

		for (auto& pBone : m_Bones)
		{
			pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		}
	}

	else
	{
		/* 내가 재생하고자하는 애니메이션(공격모션)이 이용하고 있는 뼈들의 상태 변환정보(TransformationMatrix)를 갱신해준다.*/
		m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, fTimeDelta);


		/* 모든 뼈를 순회하면서 CombinedTransformationMatrix를 갱신한다. */
		for (auto& pBone : m_Bones)
		{
			pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		}
	}
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Resources();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::Load_Model(const _tchar* szFileName, NONANIM_DATA& Data)
{
	ifstream in(szFileName, ios::binary);
	if (!in)
		return;

	in.read(reinterpret_cast<_char*>(&m_Data.mNumMeshes), sizeof(m_Data.mNumMeshes));
	in.read(reinterpret_cast<_char*>(&m_Data.mNumMaterials), sizeof(m_Data.mNumMaterials));

	_uint Mesh_Size = {};
	in.read(reinterpret_cast<_char*>(&Mesh_Size), sizeof(Mesh_Size));
	m_Data.m_Mesh.resize(Mesh_Size);

	for (auto& Mesh : m_Data.m_Mesh)
	{
		in.read(reinterpret_cast<_char*>(&Mesh.mName), sizeof(Mesh.mName));
		in.read(reinterpret_cast<_char*>(&Mesh.mMaterialIndex), sizeof(Mesh.mMaterialIndex));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumVertices), sizeof(Mesh.mNumVertices));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumIndices), sizeof(Mesh.mNumIndices));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumFaces), sizeof(Mesh.mNumFaces));

		_uint Vertex_Size = {};
		in.read(reinterpret_cast<_char*>(&Vertex_Size), sizeof(Vertex_Size));
		Mesh.mVertex.resize(Vertex_Size);

		for (auto& Vertex : Mesh.mVertex)
		{
			in.read(reinterpret_cast<_char*>(&Vertex.mVertices), sizeof(Vertex.mVertices));
			in.read(reinterpret_cast<_char*>(&Vertex.mNormals), sizeof(Vertex.mNormals));
			in.read(reinterpret_cast<_char*>(&Vertex.mTangents), sizeof(Vertex.mTangents));
			in.read(reinterpret_cast<_char*>(&Vertex.mTextureCoords), sizeof(Vertex.mTextureCoords));
		}

		_uint Index_Size = {};
		in.read(reinterpret_cast<_char*>(&Index_Size), sizeof(Index_Size));
		Mesh.mIndex.resize(Index_Size);

		for (auto& Index : Mesh.mIndex)
		{
			in.read(reinterpret_cast<_char*>(&Index.mIndices), sizeof(Index.mIndices));
		}
	}

	_uint Material_Size = {};
	in.read(reinterpret_cast<_char*>(&Material_Size), sizeof(Material_Size));
	m_Data.Material_Data.resize(Material_Size);

	for (auto& matData : m_Data.Material_Data)
	{
		in.read(reinterpret_cast<_char*>(&matData.Type_Count), sizeof(matData.Type_Count));

		_uint Type_Size = {};
		in.read(reinterpret_cast<_char*>(&Type_Size), sizeof(Type_Size));
		matData.Path.resize(Type_Size);

		for (auto& matType : matData.Path)
		{
			in.read(reinterpret_cast<_char*>(&matType.textureCount), sizeof(matType.textureCount));

			_uint String_Size = {};
			in.read(reinterpret_cast<_char*>(&String_Size), sizeof(String_Size));
			matType.szPerfectPath.resize(String_Size);

			for (auto& wstr : matType.szPerfectPath)
			{
				_uint length = {};
				in.read(reinterpret_cast<_char*>(&length), sizeof(length));
				wstr.resize(length);
				if (length > 0)
					in.read(reinterpret_cast<_char*>(&wstr[0]), length * sizeof(_tchar));
			}
		}
	}
}

void CModel::Load_AnimModel(const _tchar* szFileName, ANIM_DATA& Data)
{
	ifstream in(szFileName, ios::binary);
	if (!in)
		return;

	in.read(reinterpret_cast<_char*>(&m_AnimData.mNumMeshes), sizeof(m_AnimData.mNumMeshes));
	in.read(reinterpret_cast<_char*>(&m_AnimData.mNumMaterials), sizeof(m_AnimData.mNumMaterials));
	in.read(reinterpret_cast<_char*>(&m_AnimData.mNumAnimations), sizeof(m_AnimData.mNumAnimations));


	_uint Mesh_Size = {};
	in.read(reinterpret_cast<_char*>(&Mesh_Size), sizeof(Mesh_Size));
	m_AnimData.m_Mesh.resize(Mesh_Size);

	for (auto& Mesh : m_AnimData.m_Mesh)
	{
		in.read(reinterpret_cast<_char*>(&Mesh.mName), sizeof(Mesh.mName));
		in.read(reinterpret_cast<_char*>(&Mesh.mMaterialIndex), sizeof(Mesh.mMaterialIndex));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumVertices), sizeof(Mesh.mNumVertices));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumIndices), sizeof(Mesh.mNumIndices));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumFaces), sizeof(Mesh.mNumFaces));
		in.read(reinterpret_cast<_char*>(&Mesh.mNumBones), sizeof(Mesh.mNumBones));

		_uint Vertex_Size = {};
		in.read(reinterpret_cast<_char*>(&Vertex_Size), sizeof(Vertex_Size));
		Mesh.mVertex.resize(Vertex_Size);

		for (auto& Vertex : Mesh.mVertex)
		{
			in.read(reinterpret_cast<_char*>(&Vertex.mVertices), sizeof(Vertex.mVertices));
			in.read(reinterpret_cast<_char*>(&Vertex.mNormals), sizeof(Vertex.mNormals));
			in.read(reinterpret_cast<_char*>(&Vertex.mTangents), sizeof(Vertex.mTangents));
			in.read(reinterpret_cast<_char*>(&Vertex.mTextureCoords), sizeof(Vertex.mTextureCoords));
		}

		_uint Index_Size = {};
		in.read(reinterpret_cast<_char*>(&Index_Size), sizeof(Index_Size));
		Mesh.mIndex.resize(Index_Size);

		for (auto& Index : Mesh.mIndex)
		{
			in.read(reinterpret_cast<_char*>(&Index.mIndices), sizeof(Index.mIndices));
		}

		_uint Bone_Size = {};
		in.read(reinterpret_cast<_char*>(&Bone_Size), sizeof(Bone_Size));
		Mesh.mBone.resize(Bone_Size);

		for (auto& Bone : Mesh.mBone)
		{
			in.read(reinterpret_cast<_char*>(&Bone.mOffsetMatrix), sizeof(Bone.mOffsetMatrix));
			in.read(reinterpret_cast<_char*>(&Bone.mBoneIndex), sizeof(Bone.mBoneIndex));


			_uint Weight_Size = {};
			in.read(reinterpret_cast<_char*>(&Weight_Size), sizeof(Weight_Size));
			Bone.mWeight.resize(Weight_Size);

			for (auto& Weight : Bone.mWeight)
			{
				in.read(reinterpret_cast<_char*>(&Weight.mVertexId), sizeof(Weight.mVertexId));
				in.read(reinterpret_cast<_char*>(&Weight.x), sizeof(Weight.x));
				in.read(reinterpret_cast<_char*>(&Weight.y), sizeof(Weight.y));
				in.read(reinterpret_cast<_char*>(&Weight.z), sizeof(Weight.z));
				in.read(reinterpret_cast<_char*>(&Weight.w), sizeof(Weight.w));
			}
		}

	}

	_uint Material_Size = {};
	in.read(reinterpret_cast<_char*>(&Material_Size), sizeof(Material_Size));
	m_AnimData.Material_Data.resize(Material_Size);

	for (auto& matData : m_AnimData.Material_Data)
	{
		in.read(reinterpret_cast<_char*>(&matData.Type_Count), sizeof(matData.Type_Count));

		_uint Type_Size = {};
		in.read(reinterpret_cast<_char*>(&Type_Size), sizeof(Type_Size));
		matData.Path.resize(Type_Size);

		for (auto& matType : matData.Path)
		{
			in.read(reinterpret_cast<_char*>(&matType.textureCount), sizeof(matType.textureCount));

			_uint String_Size = {};
			in.read(reinterpret_cast<_char*>(&String_Size), sizeof(String_Size));
			matType.szPerfectPath.resize(String_Size);

			for (auto& wstr : matType.szPerfectPath)
			{
				_uint length = {};
				in.read(reinterpret_cast<_char*>(&length), sizeof(length));
				wstr.resize(length);
				if (length > 0)
					in.read(reinterpret_cast<_char*>(&wstr[0]), length * sizeof(_tchar));
			}
		}
	}

	_uint BoneData_Size = {};
	in.read(reinterpret_cast<_char*>(&BoneData_Size), sizeof(BoneData_Size));
	m_AnimData.Bone_Data.resize(BoneData_Size);

	for (auto& BoneData : m_AnimData.Bone_Data)
	{
		in.read(reinterpret_cast<_char*>(&BoneData.Bone_Name), sizeof(BoneData.Bone_Name));
		in.read(reinterpret_cast<_char*>(&BoneData.iParentIndex), sizeof(BoneData.iParentIndex));
		in.read(reinterpret_cast<_char*>(&BoneData.mTransformation), sizeof(BoneData.mTransformation));

	}

	_uint Animation_Size = {};
	in.read(reinterpret_cast<_char*>(&Animation_Size), sizeof(Animation_Size));
	m_AnimData.mAnimation.resize(Animation_Size);

	for (auto& Animation : m_AnimData.mAnimation)
	{
		in.read(reinterpret_cast<_char*>(&Animation.mName), sizeof(Animation.mName));
		in.read(reinterpret_cast<_char*>(&Animation.mfDuration), sizeof(Animation.mfDuration));
		in.read(reinterpret_cast<_char*>(&Animation.mfTicksPerSecond), sizeof(Animation.mfTicksPerSecond));
		in.read(reinterpret_cast<_char*>(&Animation.miNumChannels), sizeof(Animation.miNumChannels));

		_uint Channel_Size = {};
		in.read(reinterpret_cast<_char*>(&Channel_Size), sizeof(Channel_Size));
		Animation.mChannel.resize(Channel_Size);

		for (auto& Channel : Animation.mChannel)
		{
			in.read(reinterpret_cast<_char*>(&Channel.mName), sizeof(Channel.mName));
			in.read(reinterpret_cast<_char*>(&Channel.mNumScalingKeys), sizeof(Channel.mNumScalingKeys));
			in.read(reinterpret_cast<_char*>(&Channel.mNumRotationKeys), sizeof(Channel.mNumRotationKeys));
			in.read(reinterpret_cast<_char*>(&Channel.mNumPositionKeys), sizeof(Channel.mNumPositionKeys));

			_uint KeyFrame_Size = {};
			in.read(reinterpret_cast<_char*>(&KeyFrame_Size), sizeof(KeyFrame_Size));
			Channel.mKeyFrame.resize(KeyFrame_Size);

			for (auto& KeyFrame : Channel.mKeyFrame)
			{
				in.read(reinterpret_cast<_char*>(&KeyFrame.vScale), sizeof(KeyFrame.vScale));
				in.read(reinterpret_cast<_char*>(&KeyFrame.vRotation), sizeof(KeyFrame.vRotation));
				in.read(reinterpret_cast<_char*>(&KeyFrame.vTranslation), sizeof(KeyFrame.vTranslation));
				in.read(reinterpret_cast<_char*>(&KeyFrame.fTrackPosition), sizeof(KeyFrame.fTrackPosition));
			}
		}
	}
	int a = 10;
}

HRESULT CModel::Ready_BinMeshes()
{
	m_iNumMeshes = m_Data.mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, this, m_Data.m_Mesh[i], XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath, NONANIM_DATA& m_Data)
{
	/*  텍스쳐를 로드한다 .*/
	m_iNumMaterials = m_Data.mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_Data.Material_Data[i]);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(ANIM_DATA& mData)
{
	m_iNumAnimations = mData.mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(this, mData.mAnimation[i]);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Ready_AnimBones(ANIM_DATA& mData)
{
	size_t Size = mData.Bone_Data.size();
	for (int i = 0; i < Size; ++i)
	{
		CBone* pBone = CBone::Create(mData.Bone_Data[i], mData.Bone_Data[i].iParentIndex);
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.push_back(pBone);

	}

	return S_OK;
}

HRESULT CModel::Ready_BinAinmMeshes()
{
	m_iNumMeshes = m_AnimData.mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, this, m_AnimData.m_Mesh[i], XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath, ANIM_DATA& m_Data)
{
	/*  텍스쳐를 로드한다 .*/
	m_iNumMaterials = m_Data.mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_Data.Material_Data[i]);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

}
