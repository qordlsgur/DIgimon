#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)

class CBody_Omegamon final : public CPartObject
{
public:
	typedef struct tagBody_Player_Desc : public CPartObject::PARTOBJECT_DESC
	{

	}BODY_PLAYER_DESC;
private:
	CBody_Omegamon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Omegamon(const CBody_Omegamon& Prototype);
	virtual ~CBody_Omegamon() = default;

public:
	_float4x4* Get_BoneMatrixPtr(const _char* pBoneName);
	_bool isFinish_Att();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_Animation(const _char* szName, _bool bisLoop = true) override;

	void Time() { m_bDissolve = false; m_fTime = 0; }
	void Num(_int a) { num += a; }

private:
	CModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pDissolveTextureCom = { nullptr };

	_matrix RootBone{};
	_bool isCanon = false;
	_int num{};
private:
	void Canon();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Omegamon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END