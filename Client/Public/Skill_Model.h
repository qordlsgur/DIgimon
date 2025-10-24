#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)

class CSkill_Model final : public CGameObject
{
private:
	CSkill_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Model(const CSkill_Model& Prototype);
	virtual ~CSkill_Model() = default;

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
	CShader* m_pShaderCom = { nullptr };

	_float m_fTime{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CSkill_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END