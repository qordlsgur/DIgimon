#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(MODEL eType, const class CModel* pModel, MESH& Data, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize_Prototype(MODEL eType, const class CModel* pModel, ANIM_MESH& Data, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_BoneMatrices(const vector<class CBone*>& Bones, class CShader* pShader, const _char* pConstantName);

	_float4x4* Get_matrix() { return m_pBoneMatrices; }
	void  Set_matrix(_float4x4* matrix) { m_pBoneMatrices = matrix; }

private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iMaterialIndex = {};
	_uint				m_iNumBones = { };
	vector<_int>		m_BoneIndices;
	_float4x4* m_pBoneMatrices = { nullptr };
	vector<_float4x4>	m_OffsetMatrices;

private:
	HRESULT Ready_VertexBuffer_For_NonAnim(MESH& Mesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_VertexBuffer_For_Anim(const class CModel* pModel, ANIM_MESH& Mesh);


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const class CModel* pModel, MESH& Data, _fmatrix PreTransformMatrix);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const class CModel* pModel, ANIM_MESH& Data, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END