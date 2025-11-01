#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)

class CLeomonSkill2_Part2 final : public CPartObject
{
public:
	typedef struct tagBody_Player_Desc : public CPartObject::PARTOBJECT_DESC
	{

	}BODY_PLAYER_DESC;
private:
	CLeomonSkill2_Part2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLeomonSkill2_Part2(const CLeomonSkill2_Part2& Prototype);
	virtual ~CLeomonSkill2_Part2() = default;

public:
	_float4x4* Get_BoneMatrixPtr(const _char* pBoneName);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTexture1Com = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_matrix RootBone{};

private:


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CLeomonSkill2_Part2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END