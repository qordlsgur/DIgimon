#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_int Get_BoneIndex(const _char* pBoneName) const;
	_float4x4* Get_BoneMatrixPtr(const _char* pBoneName);
public:
	void Set_AnimationIndex(const _char* szAnimName, _bool isLoop = true);
	void Start_AnimationIndex(_int iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

public:
	virtual HRESULT Initialize_Prototype(MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_BoneMatrices(_uint iMeshIndex, class CShader* pShader, const _char* pConstantName);
	HRESULT Bind_Material(_uint iMeshIndex, class CShader* pShader, const _char* pConstantName, _uint iTextureIndex);
	_bool Play_Animation(_float fTimeDelta);
	virtual HRESULT Render(_uint iMeshIndex);

	void Load_Model(const _tchar* szFileName, NONANIM_DATA& Data);
	void Load_AnimModel(const _tchar* szFileName, ANIM_DATA& Data);

	void Set_Anim() { m_bAnimEnd = false; }


	_float Get_CurrentTrackPosition() { return m_fCurrentTrackPosition; }


private:
	NONANIM_DATA					m_Data;
	ANIM_DATA						m_AnimData;

	_float					m_fCurrentTrackPosition{};


	vector<LERP>			m_Lerp;
	MODEL					m_eType = {};
	_float4x4				m_PreTransformMatrix = {};

	_uint					m_iNumMeshes = {};
	vector<class CMesh*>	m_Meshes;

	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

	vector<class CBone*>		m_Bones;

	_int						m_iCurrentAnimIndex = { -1 };
	_int						m_iPreviousAnimIndex = { 0 };
	_int						m_iNumAnimations = {};
	vector<class CAnimation*>	m_Animations;
	vector<string>				m_AnimNames;

	_bool						m_isLoop = { false };
	_bool						m_isFinish = { false };
	_bool						m_Change_Anim = false;
	_bool						m_bAnimEnd = false;

private:
	// nonanim bin
	HRESULT Ready_BinMeshes();
	HRESULT Ready_Materials(const _char* pModelFilePath, NONANIM_DATA& m_Data);

	// anim bin
	HRESULT Ready_AnimBones(ANIM_DATA& mData);
	HRESULT Ready_BinAinmMeshes();
	HRESULT Ready_Materials(const _char* pModelFilePath, ANIM_DATA& m_Data);
	HRESULT Ready_Animations(ANIM_DATA& mData);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
virtual void Free(); public:
};

NS_END